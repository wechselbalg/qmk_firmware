/* ==========================================================================
   Die Status-LED (Liatris-NeoPixel an GP25)

   Jede Liatris-Haelfte hat eine eigene WS2812 auf der Platine. Sie traegt die
   Zustaende, die bewusst NICHT auf die Tastenmatrix sollen -- Caps Word,
   Layer Lock, Jiggler, Mac-Modus -- statt die Layer-Farbe noch einmal zu
   wiederholen (Michael, 2026-08-04). Die Matrix sagt "wo bin ich", die
   Status-LED "was ist gerade an".

   Ist nichts davon an und der Host ein PC, bleibt sie **aus**. Sie leuchtet
   also nur, wenn es etwas zu sagen gibt -- das macht ihr Leuchten selbst zur
   ersten Information.

   ---------------------------------------------------------------------------
   ## Warum hier ein eigener Treiber steht

   QMKs WS2812-Treiber fuer den RP2040
   (platforms/chibios/drivers/vendor/RP/RP2040/ws2812_vendor.c) ist eindeutig
   Single-Instance: ein WS2812_DI_PIN, ein statisches WS2812_BUFFER, eine
   State Machine, ein DMA-Kanal, ein Semaphor. Eine zweite Kette ist dort
   nicht vorgesehen, und GP0 ist von der Per-Key-Kette belegt.

   Der Ausweg: **PIO0 hat vier State Machines, der Treiber belegt genau eine.**
   Wir nehmen eine zweite, mit einer eigenen Kopie desselben
   6-Instruktionen-Programms (PIO-Instruktionsspeicher: 32 Woerter) und
   `sideset` auf GP25. Fuer *eine* LED braucht es weder DMA noch Interrupt --
   ein pio_sm_put_blocking() mit einem 32-Bit-Wort, und das auch nur, wenn
   sich die Farbe aendert.

   Belegung auf diesem Board: Per-Key-Kette GP0 -> PIO0 (keyboard.json sagt
   "D3", promicro_to_rp2040_ce/_pin_defs.h bildet D3 auf 0U ab),
   Split-Serial -> PIO1 (SERIAL_PIO_USE_PIO1), Status-LED GP25 -> PIO0, zweite
   SM. Es bleiben zwei SMs frei.

   Wenn keine State Machine mehr frei ist, tut diese Datei schlicht nichts --
   eine Status-LED ist kein Grund, eine Tastatur nicht booten zu lassen.

   Das PIO-Programm und die Timing-Rechnung darunter sind aus dem
   QMK-Treiber uebernommen (Copyright 2022 Stefan Kerkmann, GPL-2.0-or-later);
   sie leiten sich aus denselben WS2812_T*-Werten ab, damit beide Ketten
   garantiert dieselben Pulsbreiten fahren.

   ---------------------------------------------------------------------------
   ## Was der Split beitraegt

   Die Zustaende entstehen alle auf dem Master -- QMK verarbeitet dort alle
   Key-Events. Die Peripherie-Haelfte weiss von sich aus nichts davon: QMK
   synchronisiert `layer_state` und die RGB-Matrix-Config, aber weder Caps
   Word noch Layer Lock, Jiggler oder den Mac-Modus.

   Deshalb eine eigene Transaktion (SPLIT_TRANSACTION_IDS_USER) mit **einem
   Byte Statusflags** fuer alle vier. Sie geht nur raus, wenn sich etwas
   aendert -- plus ein Auffrischen im Sekundentakt, damit eine verlorene
   Uebertragung nicht dauerhaft eine falsche Anzeige hinterlaesst.
   ========================================================================== */
/*
Reihenfolge wie im QMK-Treiber, und aus demselben Grund: die pico-sdk-Header
muessen VOR allem anderen kommen. quantum.h zieht rp2040.h/ChibiOS herein, und
danach scheitert schon pico/assert.h an einem Makro-Namenskonflikt, den wir
auf keiner der beiden Seiten kontrollieren.
*/
#include "ws2812.h"

#include "hardware/timer.h"
#include "hardware/clocks.h"
#include <hal.h>
#include "hardware/pio.h"

#include "wechselbalg.h"
#include "util.h"
#include "caps_word.h"
#include "layer_lock.h"
#include "transactions.h"

#ifdef WB_STATUS_LED

/* ---- Stellschrauben ---------------------------------------------------- */

/*
WB_STATUS_VAL_OFFSET -- Helligkeit relativ zur Tastenbeleuchtung, in denselben
Einheiten wie rgb_matrix_get_val() (0..255). **Vorzeichenbehaftet**: negativ
dunkler, positiv heller.

Die Platinen-LED ist ohne Abdeckung verbaut und hat damit nichts, was sie
streut -- bei gleicher nomineller Helligkeit blendet sie, sobald die Tasten
angenehm eingestellt sind (Hardware-Befund 2026-08-04, deshalb der deutlich
negative Default). Ein einzelner Helligkeitsschritt, wie ihn KMK abzog, reicht
dafuer nicht.

Unter Null bleibt eine Reststufe stehen, damit ein dunkel gefahrenes Board die
Zustandsanzeige nicht stillschweigend verliert. Wer die Matrix dauerhaft sehr
dunkel faehrt, setzt den Offset entsprechend kleiner.

WB_STATUS_IDLE_DIV -- der Mac-Hinweis zusaetzlich noch einmal geteilt. Er ist
ein Dauerzustand und soll leiser sein als die Momentzustaende darueber.

Beides ueberschreibbar per OPT_DEFS in der Keymap-rules.mk.
*/
#    ifndef WB_STATUS_VAL_OFFSET
#        define WB_STATUS_VAL_OFFSET (-24)
#    endif
#    ifndef WB_STATUS_IDLE_DIV
#        define WB_STATUS_IDLE_DIV 2
#    endif

/*
Der Pin der Onboard-WS2812 auf dem splitkb Liatris. Numerisch und nicht GP25:
CONVERT_TO=liatris zieht das Pro-Micro-Pinmapping heran, in dem die
GPxx-Namen nicht existieren -- derselbe Fall wie die Power-LED an 24U.
*/
#    ifndef WB_STATUS_LED_PIN
#        define WB_STATUS_LED_PIN 25U
#    endif

/* ---- Palette ----------------------------------------------------------- */

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} wb_status_color_t;

#    define WB_SC(r_, g_, b_) ((wb_status_color_t){(r_), (g_), (b_)})

// Momentzustaende, absteigend nach Dringlichkeit -- Caps Word dauert einen
// Augenblick und muss jetzt gesehen werden, der Jiggler laeuft stundenlang.
#    define WB_SC_CAPS_WORD  WB_SC(255, 255, 255)
#    define WB_SC_LAYER_LOCK WB_SC(  0, 255, 255)  // dasselbe Cyan wie die Taste
#    define WB_SC_JIGGLER    WB_SC(170, 255,   0)

/*
Ruhe: kein Zustand an. Dann leuchtet die LED nur noch, wenn der Mac-Modus an
ist -- am PC ist sie aus (Michael, 2026-08-04).

Damit ist "sie leuchtet ueberhaupt" schon die halbe Information, und das
Board ist im Normalfall dunkel statt dauerhaft mit einem Punkt zu leuchten,
der nichts sagt.
*/
#    define WB_SC_MAC WB_SC(0, 120, 255)
#    define WB_SC_OFF WB_SC(0, 0, 0)

/* ---- Statusflags, einmal fuer beide Haelften --------------------------- */

enum wb_status_flag {
    WB_STATUS_CAPS_WORD  = 1 << 0,
    WB_STATUS_LAYER_LOCK = 1 << 1,
    WB_STATUS_JIGGLER    = 1 << 2,
    WB_STATUS_MAC        = 1 << 3,
};

// Auf der Peripherie das zuletzt Empfangene, auf dem Master ungenutzt.
static uint8_t wb_status_received = 0;

/*
Layer Lock meldet sich hier, statt dass wir is_layer_locked() fuer jeden Layer
einzeln abfragen: der Hook bekommt die komplette Bitmaske und wird genau dann
gerufen, wenn sich etwas aendert.
*/
static bool wb_layer_locked = false;

bool layer_lock_set_user(layer_state_t locked_layers) {
    wb_layer_locked = (locked_layers != 0);
    return true;
}

static uint8_t wb_status_flags_local(void) {
    uint8_t flags = 0;
    if (is_caps_word_on()) flags |= WB_STATUS_CAPS_WORD;
    if (wb_layer_locked) flags |= WB_STATUS_LAYER_LOCK;
#    ifdef WB_JIGGLER
    if (wb_is_jiggling()) flags |= WB_STATUS_JIGGLER;
#    endif
    if (WB_HOST_IS_MAC()) flags |= WB_STATUS_MAC;
    return flags;
}

static void wb_status_sync_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    if (in_len == sizeof(uint8_t)) {
        wb_status_received = *(const uint8_t *)in_data;
    }
}

/* ---- PIO-Treiber fuer genau eine WS2812 -------------------------------- */

/* Timing-Rechnung und Programm aus ws2812_vendor.c, siehe Kopf der Datei. */
#    define WB_PIO_DELAY(delay, opcode) (((delay & 0xF) << 8U) | opcode)

#    define WB_PIO_T1L (WS2812_T1L / 50)
#    define WB_PIO_T1L_A (MAX(CEILING(WB_PIO_T1L, 2) - 1, 0))
#    define WB_PIO_T1L_B (MAX(WB_PIO_T1L / 2 - 1, 0))
#    define WB_PIO_T0L (MAX(WS2812_T0L / 50 - WB_PIO_T1L, 0))
#    define WB_PIO_T0L_A (MAX(WB_PIO_T0L - 1, 0))
#    define WB_PIO_T0H (WS2812_T0H / 50)
#    define WB_PIO_T0H_A MAX(WB_PIO_T0H - 1, 0)
#    define WB_PIO_T1H (MAX(WS2812_T1H / 50 - WB_PIO_T0H, 0))
#    define WB_PIO_T1H_A (MAX((CEILING(WB_PIO_T1H, 2) - 1), 0))
#    define WB_PIO_T1H_B (MAX((WB_PIO_T1H / 2) - 1, 0))

static const uint16_t wb_ws2812_instructions[] = {
    WB_PIO_DELAY(WB_PIO_T1L_A, 0x6021), //  0: out    x, 1            side 0
    WB_PIO_DELAY(WB_PIO_T1L_B, 0xa042), //  1: nop                    side 0
    WB_PIO_DELAY(WB_PIO_T0H_A, 0x1025), //  2: jmp    !x, 5           side 1
    WB_PIO_DELAY(WB_PIO_T1H_A, 0xb042), //  3: nop                    side 1
    WB_PIO_DELAY(WB_PIO_T1H_B, 0x1000), //  4: jmp    0               side 1
    WB_PIO_DELAY(WB_PIO_T0L_A, 0xa042), //  5: nop                    side 0
};

static const pio_program_t wb_ws2812_program = {
    .instructions = wb_ws2812_instructions,
    .length       = ARRAY_SIZE(wb_ws2812_instructions),
    .origin       = -1,
};

static int wb_status_sm = -1;

static void wb_status_led_hw_init(void) {
    /* Idempotent -- der Per-Key-Treiber hat PIO0 in ws2812_init() schon aus
       dem Reset geholt, aber wir wollen nicht davon abhaengen. */
    hal_lld_peripheral_unreset(RESETS_ALLREG_PIO0);

    palSetLineMode(WB_STATUS_LED_PIN, PAL_RP_PAD_SLEWFAST | PAL_RP_GPIO_OE | PAL_MODE_ALTERNATE_PIO0);

    wb_status_sm = pio_claim_unused_sm(pio0, false);
    if (wb_status_sm < 0) {
        return;  // keine State Machine frei -- dann eben ohne Status-LED
    }

    const uint offset = pio_add_program(pio0, &wb_ws2812_program);

    pio_sm_set_consecutive_pindirs(pio0, wb_status_sm, WB_STATUS_LED_PIN, 1, true);

    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_wrap(&config, offset, offset + ARRAY_SIZE(wb_ws2812_instructions) - 1);
    sm_config_set_sideset_pins(&config, WB_STATUS_LED_PIN);
    sm_config_set_sideset(&config, 1, false, false);
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_TX);
    // 24 Bit, MSB zuerst, Autopull -- eine LED, kein Weiss-Kanal.
    sm_config_set_out_shift(&config, false, true, 24);
    // Eine Instruktion pro 50 ns, das ist die Zeitaufloesung des Programms.
    sm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (20.0f * MHZ));

    pio_sm_init(pio0, wb_status_sm, offset, &config);
    pio_sm_set_enabled(pio0, wb_status_sm, true);
}

static void wb_status_led_write(wb_status_color_t color) {
    if (wb_status_sm < 0) {
        return;
    }
    // GRB, linksbuendig in den oberen 24 Bit -- so liest das Schieberegister.
    pio_sm_put_blocking(pio0, wb_status_sm, ((uint32_t)color.g << 24) | ((uint32_t)color.r << 16) | ((uint32_t)color.b << 8));
}

/* ---- Was angezeigt wird ------------------------------------------------ */

static wb_status_color_t wb_status_color(uint8_t flags, bool *is_idle) {
    *is_idle = false;
    if (flags & WB_STATUS_CAPS_WORD) return WB_SC_CAPS_WORD;
    if (flags & WB_STATUS_LAYER_LOCK) return WB_SC_LAYER_LOCK;
    if (flags & WB_STATUS_JIGGLER) return WB_SC_JIGGLER;

    *is_idle = true;
    return (flags & WB_STATUS_MAC) ? WB_SC_MAC : WB_SC_OFF;
}

static uint8_t wb_status_val(void) {
    if (!rgb_matrix_is_enabled()) {
        return 0;  // RM_TOGG schaltet die Status-LED mit ab
    }
    const int16_t val = (int16_t)rgb_matrix_get_val() + (WB_STATUS_VAL_OFFSET);
    if (val < 1) {
        return 1;  // nie ganz weg, solange die Matrix an ist
    }
    if (val > UINT8_MAX) {
        return UINT8_MAX;
    }
    return (uint8_t)val;
}

void wb_status_led_init(void) {
    wb_status_led_hw_init();
    transaction_register_rpc(WB_SYNC_STATUS, wb_status_sync_handler);
}

void wb_status_led_task(void) {
    static uint8_t  last_flags = 0xFF;
    static uint8_t  last_val   = 0xFF;
    static uint32_t last_sync  = 0;

    uint8_t flags;
    if (is_keyboard_master()) {
        flags = wb_status_flags_local();
        /* Nur bei Aenderung senden, plus ein Auffrischen im Sekundentakt --
           eine verlorene Uebertragung soll keine dauerhaft falsche Anzeige
           hinterlassen. Jeden Durchlauf zu senden wuerde den Matrix-Scan
           ausbremsen. */
        if (flags != wb_status_received || timer_elapsed32(last_sync) > 1000) {
            if (transaction_rpc_send(WB_SYNC_STATUS, sizeof(flags), &flags)) {
                wb_status_received = flags;
                last_sync          = timer_read32();
            }
        }
    } else {
        flags = wb_status_received;
    }

    const uint8_t val = wb_status_val();
    if (flags == last_flags && val == last_val) {
        return;  // nichts zu tun -- die LED haelt ihren Wert von selbst
    }
    last_flags = flags;
    last_val   = val;

    bool                    idle  = false;
    const wb_status_color_t color = wb_status_color(flags, &idle);
    const uint16_t          scale = idle ? (val / WB_STATUS_IDLE_DIV) : val;

    wb_status_led_write(WB_SC((uint8_t)(color.r * scale / 255), (uint8_t)(color.g * scale / 255), (uint8_t)(color.b * scale / 255)));
}

#endif  // WB_STATUS_LED
