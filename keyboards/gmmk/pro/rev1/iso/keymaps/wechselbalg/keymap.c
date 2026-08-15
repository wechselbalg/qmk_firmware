/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "wechselbalg.h"
#include "sendstring_german.h"

#include QMK_KEYBOARD_H
#include "tap_dance_actions.h"  // Zentrale Definition
#ifdef WB_SIDEBAR_STATUS
#    include "status_state.h"
#endif

// Einfacher Include statt direkter Definition
tap_dance_action_t tap_dance_actions[] = {
    WECHSELBALG_TAP_DANCE_ACTIONS
};

// #include "print.h"
// clang-format off

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     /        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right

    // [_BASE] = LAYOUT_wrapper(
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    //     _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
    // ),

    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backspace. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.

    [_QWERT] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________,  KC_PSCR,  KC_MUTE,
        KC_GRV,  _______________________________NUMBERS________________________________, KC_MINS, KC_EQL ,  KC_BSPC,  KC_DEL,
        ________________________________________QWERTY_1________________________________________, KC_RBRC,            KC_HOME,
        ________________________________________QWERTY_2________________________________________, SYM_HSH,  KC_ENT,   KC_END,
        ________________________________________QWERTY_3________________________________________,           KC_UP,    MO__ADJ,
        ________________________________________7_THUMBS________________________________________, KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

#ifdef WB_LAYOUT_COLEMAKDH
    /*
    Zeilenweise identisch mit _QWERT -- nur die Buchstabenbloecke unterscheiden
    sich. Die COLMAK_*-Bloecke sind die 12/13-breiten ISO-Varianten aus
    wrappers.h; warum die letzte Taste in Reihe 1 und 2 dort von den
    Split-Haelften abweicht, steht im Kommentar an ihrer Definition.
    */
    [_COLEMAKDH] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________,  KC_PSCR,  KC_MUTE,
        KC_GRV,  _______________________________NUMBERS________________________________, KC_MINS, KC_EQL ,  KC_BSPC,  KC_DEL,
        ________________________________________COLMAK_1________________________________________, KC_RBRC,            KC_HOME,
        ________________________________________COLMAK_2________________________________________, SYM_HSH,  KC_ENT,   KC_END,
        ________________________________________COLMAK_3________________________________________,           KC_UP,    MO__ADJ,
        ________________________________________7_THUMBS________________________________________, KC_LEFT,  KC_DOWN,  KC_RGHT
    ),
#endif

    [_SYM] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________, _______,  _______,
        ________________________________________SYMBOL__0_______________________________________, ___NO__, _______,  _______,
        ________________________________________SYMBOL__1_______________________________________, N3_SLSH,           _______,
        ________________________________________SYMBOL__2_______________________________________, _______, _______,  _______,
        ________________________________________SYMBOL__3_______________________________________,          _______,  _______,
        ________________________________________7_THUMBS________________________________________, _______, _______,  _______
    ),

    [_NUM] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,  KC_F13,   _______,
        ________________________________________NUMBER__0_______________________________________, ___NO__,  _______,  _______,
        ________________________________________NUMBER__1_______________________________________, ___NO__,            _______,
        ________________________________________NUMBER__2_______________________________________, _______,  _______,  _______,
        ________________________________________NUMBER__3_______________________________________,           _______,  _______,
        ________________________________7_NUMBER__THUMBS________________________________________, _______,  _______,  _______
    ),

    [_NAV] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,   KC_F13,  _______,
        ______________________________________NAVIGATION__0_____________________________________, ___NO__,  _______,  _______,
        ______________________________________NAVIGATION__1_____________________________________, N3_SLSH,            UC_NEXT,
        ______________________________________NAVIGATION__2_____________________________________, _______,  _______,  UC_PREV,
        ______________________________________NAVIGATION__3_____________________________________,           MS_UP  ,  _______,
        __________________________________7_NAVIGATION__THUMBS__________________________________, MS_LEFT,  MS_DOWN,  MS_RGHT
    ),

    [_GAMING] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________,  _______,  _______,
        KC_GRV,  _______________________________NUMBERS________________________________, KC_MINS, _______,  _______,  _______,
        ________________________________________QWERTY_1________________________________________, _______,            _______,
        ________________________________________GAMING_2________________________________________, _______,  _______,  _______,
        ________________________________________GAMING_3________________________________________,           _______,  _______,
        ____________________________________7_GAMING__THUMBS____________________________________, _______,  _______,  _______
    ),

    [_ADJUST] = LAYOUT_wrapper(
        // Die obere rechte Ecke ist auf _ADJUST komplett "Beleuchtung":
        //   Print-Position -> SIDEBAR (Lichtbalken: Animation <-> Status)
        //   Encoder-Druck  -> RGB_TOG (Beleuchtung ganz aus/an)
        // passend dazu, dass die Encoder-Drehung hier die Helligkeit regelt.
        // Auf allen anderen Layern bleiben beide _______ und fallen damit auf
        // KC_PSCR bzw. KC_MUTE der Basis-Layer durch.
        QK_BOOT, ________________________________________FN_KEYS_________________________________________,  A_SIDEBR, RGB_TOG,
        ________________________________________ADJUST__0_______________________________________, _______,  QK_BOOT,  _______,
        ________________________________________ADJUST__1_______________________________________, _______,            _______,
        ________________________________________ADJUST__2_______________________________________, FN_EXIT,  _______,  _______,
        ________________________________________ADJUST__3_______________________________________,           RGB_MOD,  _______,
        ________________________________________7_THUMBS________________________________________, RGB_SPD, RGB_RMOD,  RGB_SPI
    ),

};

/*
Handedness fuer CHORDAL_HOLD (users/wechselbalg/config.h).

Uebernimmt exakt die Tabelle, die QMK sonst aus keyboard.json errechnet
(nachpruefbar mit `qmk generate-keyboard-c -kb gmmk/pro/rev1/iso`) -- hier
aber explizit, damit sie sichtbar und aenderbar ist statt von einer Heuristik
abzuhaengen.

Entscheidend ist die '*' in der letzten Reihe: das ist die Leertaste, und
damit NAV_SPC aus dem 7_THUMBS-Wrapper -- der einzige Tap-Hold der
Daumenreihe. Ohne die Ausnahme wuerde jede Kombination "Space halten +
Taste derselben Hand" den Space als Tap abschliessen, statt den _NAV-Layer
zu erreichen.

Die uebrigen Tap-Holds (NAV_TAB, NUM__UE, SYM__AE) liegen auf den
Aussenspalten und sind mit der normalen L/R-Regel korrekt bedient.
*/
#ifdef CHORDAL_HOLD
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_wrapper(

  'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', '*', 'R', 'R', 'R', 'R', 'R', 'R'
);
#endif

// clang-format on

/*
Der Drehencoder oben rechts.

Bis 2026-08-06 stand hier ein `encoder_map` hinter `#if ENCODER_MAP_ENABLE` --
und das war in der rules.mk auskommentiert. `ENCODER_ENABLE` ist aber ueber
keyboard.json an: die Drehung wurde also eingelesen und dann verworfen, der
Knopf drehte ins Leere. Nur sein *Druck* tat etwas (KC_MUTE aus Reihe 0).

Jetzt derselbe Aufbau wie beim **rechten** Sofle-Encoder
(keyboards/sofle_choc/keymaps/wechselbalg/keymap.c), damit zwischen den Boards
nicht noch eine Variante entsteht -- mit der Lautstaerke im Basis-Layer
(Michael, 2026-08-06):

  Basis-Layouts (_QWERT/_COLEMAKDH) -> Lautstaerke
  _SYM / _NAV / _NUM                -> Tab / Shift+Tab (Fenster-/Tabwechsel)
  _GAMING                           -> Pfeil hoch/runter
  _ADJUST                           -> RGB-Helligkeit

Bewusst `encoder_update_user()` statt `ENCODER_MAP_ENABLE`: die Helligkeit
muss ueber die *_noeeprom-Varianten laufen, sonst schreibt jede einzelne
Rastung ins EEPROM. Ein encoder_map koennte nur RGB_VAI/RGB_VAD eintragen --
also genau die EEPROM-schreibende Variante. Dieselbe Ueberlegung wie bei C7
auf der Sofle Choc.
*/
static void wb_brightness(bool up) {
#if defined(RGB_MATRIX_ENABLE)
    if (up) {
        rgb_matrix_increase_val_noeeprom();
    } else {
        rgb_matrix_decrease_val_noeeprom();
    }
#elif defined(RGBLIGHT_ENABLE)
    if (up) {
        rgblight_increase_val_noeeprom();
    } else {
        rgblight_decrease_val_noeeprom();
    }
#endif
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index != 0) {
        return true;
    }

    /*
    ⚠️ _GAMING wird per DF() betreten und liegt damit in `default_layer_state`,
    nicht in `layer_state` -- die beiden sind in QMK getrennte Variablen
    (quantum/action_layer.c:12 und :101), und `get_highest_layer(layer_state)`
    liefert bei nur gesetztem Default-Layer schlicht 0. Ein blosses
    `switch (get_highest_layer(layer_state))` haette den _GAMING-Zweig also nie
    erreicht und dort die Lautstaerke geregelt.

    Deshalb: ein gehaltener/getoggelter Layer gewinnt, sonst zaehlt das
    Default-Layer -- das ist zugleich fuer die Basis-Layouts richtig, weil
    deren Index dort ebenso steht.
    */
    const uint8_t layer = layer_state ? get_highest_layer(layer_state)
                                      : get_highest_layer(default_layer_state);

    switch (layer) {
        case _ADJUST:
            wb_brightness(clockwise);
            break;
        /*
        Im Uhrzeigersinn = nach unten. Festgelegt 2026-08-06 nach dem
        Hardware-Test auf der Sofle Choc, dieselbe Konvention wie dort:
        vertikale Bewegung folgt der Drehung, Lautstaerke und Helligkeit
        bleiben bei "im Uhrzeigersinn = mehr".
        */
        case _GAMING:
            tap_code(clockwise ? KC_DOWN : KC_UP);
            break;
        case _SYM:
        case _NAV:
        case _NUM:
            if (clockwise) {
                tap_code(KC_TAB);
            } else {
                tap_code16(NX_UTAB);
            }
            break;
        default:
            tap_code(clockwise ? KC_VOLU : KC_VOLD);
            break;
    }

    return false;
}

// const custom_shift_key_t custom_shift_keys[] = {
//   {DE_CIRC, N2_HCEK},  // Shift 2 is §
//   {DE_1 , N2__DEG},    // Shift 1 is °
//   {DE_2 , DE_SECT},    // Shift 2 is §
// //   {DE_3 , N2_SS_L},    // Shift 3 is ℓ
// //   {DE_4 , N2_RAQO},    // Shift 4 is »
// //   {DE_5 , N2_LAQO},    // Shift 5 is «
//   {DE_6 , N3__DLR},    // Shift 6 is $
//   {DE_7 , N2__EUR},    // Shift 7 is €
// //   {DE_8 , N2_LODQ},    // Shift 8 is „
// //   {DE_9 , N2_L_DQ},    // Shift 9 is “
// //   {DE_0 , N2_R_DQ},    // Shift 0 is ”
// //   {DE_SS, N2_C_SS},    // Shift ß is ẞ

//   {DE_ACUT , DE_TILD}, // Shift ´ is ~

//   {DE_COMM, N2_NDSH},  // Shift , is –
//   {DE_DOT , N2_BULT},  // Shift . is •
//   {KC_SLSH, N3_MDSH},  // Shift - is —

//   {KC_BSPC, KC_DEL},  // Shift Backspace is Delete
// };

// uint8_t NUM_CUSTOM_SHIFT_KEYS =
//     sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);


#ifdef WB_SIDEBAR_STATUS
/* ---- Der seitliche Lichtbalken ----------------------------------------- */
/*
Die GMMK Pro hat 99 LEDs: 83 unter Tasten und 16 im Lichtbalken ringsum, die
keiner Matrixposition zugeordnet sind (`flags: 2`, LED_FLAG_UNDERGLOW). Die
Farbsprache laeuft ueber die Matrix und erreicht sie deshalb nie -- sie zeigen
also, was der laufende Effekt malt.

Zwei Betriebsarten, umschaltbar mit der SIDEBAR-Taste auf _ADJUST (physisch
Print, direkt neben dem Encoder -- die ganze obere rechte Ecke ist auf _ADJUST
"Beleuchtung"):

  Animation (Vorgabe) -- wir fassen den Balken nicht an, der Effekt laeuft.
                         RGB_MOD/RGB_RMOD wechseln ihn sichtbar.
  Status              -- der Balken zeigt dieselben Zustaende wie die Status-LED
                         der Liatris-Sofle, aus derselben Palette
                         (users/wechselbalg/status_state.h):
                         Caps Word weiss > Layer Lock cyan > Jiggler gelbgruen
                         > Mac-Modus azur (gedaempft) > sonst aus.

Michaels Vorgabe war "grundsaetzlich Animation, im Zweifel aber Funktion vor
Optik" -- deshalb ist die Umschaltung persistent (EEPROM) statt nur fuer die
Sitzung, und der Balken faellt im Status-Modus bei Ruhe auf *aus* zurueck. Damit
ist sein Leuchten selbst schon die Information, genau wie auf der Sofle.

⚠️ Auf diesem Board ist der Status-Modus die einzige Anzeige des Mac-Modus --
die GMMK Pro hat weder OLED noch Win/Mac-Schalter.
*/

// Ruhezustaende (nur der Mac-Hinweis) zusaetzlich daempfen, wie auf der Sofle.
#    ifndef WB_SIDEBAR_IDLE_DIV
#        define WB_SIDEBAR_IDLE_DIV 2
#    endif

static bool wb_sidebar_status = false;  // false = Animation

// Frisches EEPROM: Vorgabe ist die Animation.
void eeconfig_init_user(void) {
    eeconfig_update_user(0);
}

void keyboard_post_init_user(void) {
    wb_sidebar_status = (eeconfig_read_user() & 1u) != 0u;
}

void wb_rgb_extra_leds(uint8_t led_min, uint8_t led_max, uint8_t val) {
    if (!wb_sidebar_status) {
        return;  // Animation stehen lassen
    }

    bool                    idle  = false;
    const wb_status_color_t color = wb_status_color(wb_status_flags_local(), &idle);

    uint16_t scale = val;
    if (idle) {
        scale /= WB_SIDEBAR_IDLE_DIV;
    }

    const uint8_t r = (uint8_t)(((uint16_t)color.r * scale) / 255);
    const uint8_t g = (uint8_t)(((uint16_t)color.g * scale) / 255);
    const uint8_t b = (uint8_t)(((uint16_t)color.b * scale) / 255);

    for (uint8_t i = led_min; i < led_max; i++) {
        if (!HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            continue;  // Tasten-LEDs gehoeren der Farbsprache
        }
        rgb_matrix_set_color(i, r, g, b);
    }
}
#endif  // WB_SIDEBAR_STATUS

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {

    #ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    #endif

    #ifdef CONSOLE_ENABLE
    switch (keycode) {
        case RN_STEM:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(100) "steam" SS_DELAY(500) SS_TAP(X_ENT));
                print("steam\n");
            } else {
                // when keycode is released
            }
            break;
        case RN_CODE:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(100) "Visual Studio Code" SS_DELAY(500) SS_TAP(X_ENT));
                print("code\n");
            } else {
                // when keycode is released
            }
            break;
    }
    #endif

    // if (!process_custom_shift_keys(keycode, record)) { return false; }

#ifdef WB_SIDEBAR_STATUS
    if (keycode == SIDEBAR) {
        if (record->event.pressed) {
            wb_sidebar_status = !wb_sidebar_status;
            eeconfig_update_user(wb_sidebar_status ? 1u : 0u);
        }
        return false;
    }
#endif

    return true;
}

// const key_override_t hcek_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_CIRC, N2_HCEK);
// const key_override_t deg_key_override  = ko_make_basic(MOD_MASK_SHIFT, DE_1, N2__DEG);
// const key_override_t sect_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_2, DE_SECT);
// const key_override_t ss_l_override     = ko_make_basic(MOD_MASK_SHIFT, DE_3, N2_SS_L);
// const key_override_t raqo_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_4, N2_RAQO);
// const key_override_t laqo_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_5, N2_LAQO);
// const key_override_t dlr_key_override  = ko_make_basic(MOD_MASK_SHIFT, DE_6, N3__DLR);
// const key_override_t eur_key_override  = ko_make_basic(MOD_MASK_SHIFT, DE_7, N2__EUR);
// const key_override_t lodq_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_8, N2_LODQ);
// const key_override_t l_dq_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_9, N2_L_DQ);
// const key_override_t r_dq_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_0, N2_R_DQ);
// const key_override_t ss_key_override   = ko_make_basic(MOD_MASK_SHIFT, DE_SS,N2_C_SS);

// const key_override_t tild_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_ACUT, DE_TILD);

// const key_override_t ndsh_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_COMM, N2_NDSH);
// const key_override_t bult_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_DOT,  N2_BULT);
// const key_override_t mdsh_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, N3_MDSH);

// const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// // This globally defines all key overrides to be used
// const key_override_t **key_overrides = (const key_override_t *[]){
//     &hcek_key_override,
//     &deg_key_override,
//     &hcek_key_override,
//     &deg_key_override,
//     &sect_key_override,
//     &ss_l_override,
//     &raqo_key_override,
//     &laqo_key_override,
//     &dlr_key_override,
//     &eur_key_override,
//     &lodq_key_override,
//     &l_dq_key_override,
//     &r_dq_key_override,
//     &ss_key_override,
//     &tild_key_override,
//     &ndsh_key_override,
//     &bult_key_override,
//     &mdsh_key_override,
//     &delete_key_override,
//     NULL // Null terminate the array of overrides!
// };

/*
2026-08-06 entfallen: ein keyboard_post_init_user(), das debug_enable /
debug_matrix / debug_keyboard auf true setzte. Der Build hat CONSOLE_ENABLE aus
und definiert -DNO_DEBUG -- die Ausgaben sind also gar nicht einkompiliert, die
Zuweisungen liefen ins Leere. Zum Debuggen: CONSOLE_ENABLE = yes in der
rules.mk, dann tut es der uprintf-Block in process_record_keymap() oben.
*/


