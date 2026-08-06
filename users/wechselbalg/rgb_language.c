/* ==========================================================================
   Die Farbsprache der Per-Key-LEDs (C8)

   Jede LED-Farbe wird aus dem Keymap **abgeleitet**, nicht gepflegt. Was eine
   Taste ist (Modifier, Layer-Zugang, Nutzblock), bestimmt ihre Farbe -- eine
   Taste, die im Keymap umzieht, nimmt ihre Farbe mit. Uebernommen aus dem
   KMK-Port derselben Tastatur (design/palette.py + design/rgb_rules.py).

   KMK musste die Ableitung als Host-Build-Schritt machen, weil CircuitPython
   die dabei entstehende Heap-Fragmentierung nicht ueberlebte. In QMK ist sie
   zur Laufzeit trivial: Keycodes sind uint16_t mit Bitfeldern, und QMK liefert
   genau die zwei Funktionen, auf die es ankommt --

     layer_switch_get_layer(pos)   der hoechste aktive Layer, der an dieser
                                   Position NICHT KC_TRNS ist (action_layer.c).
                                   Das ist KMKs "TRNS faellt auf das
                                   Basis-Layout durch", nur korrekt statt als
                                   statische Naeherung. Und es macht die Regel
                                   "der aktive Layer faerbt, was er belegt" zu
                                   einem Vergleich statt zu einer Tabelle.
     keymap_key_to_keycode(l, pos) der Keycode dort.

   ---------------------------------------------------------------------------
   Die Regel hinter der Palette: **Farbton = Bedeutung, Saettigung = Art.**

     gesaettigter Farbton  Rolle -- welcher Layer, welcher Modifier
     weiss                 Inhalt -- der Nutzblock des aktiven Layers
                           (Nummernblock auf _NUM, Pfeilkreuz auf _NAV)
     grau                  neutraler Inhalt -- der Maus-Block; "Zeiger" ist
                           keine Farbe so wie ein Modifier eine ist
     gedimmtes Warm        nichts Besonderes -- das Grundleuchten, damit die
                           Tasten im Dunkeln findbar sind

   Der Farbkreis ist mit fuenf Layern + vier Modifiern + Verwaltung praktisch
   voll. Deshalb liegen die beiden Nutzbloecke bewusst auf der *Saettigungs*-
   achse statt auf noch einem Farbton -- so kollidieren sie mit nichts, und
   der Kontrast zur Layer-Farbe ist maximal, egal welche das ist.

   Abweichungen von KMKs Werten, beide wegen einer echten Kollision:
     _NUM   (255, 80,0) -> (255,140,0)  KMKs Orange lag neben Alts Orangerot,
                                        und KC_RALT sitzt auf _NUM.
     Ctrl   (255,0, 90) -> (255,0,140)  lag zu nah an ADJUSTs Rot, und die
                                        Ctrl-Daumen liegen auf _ADJUST.

   Die Farben stehen alle im Block unten -- Nachjustieren am Board ist eine
   Zeile. Das ist der eigentliche Test; alles hier ist am Schreibtisch
   entstanden.

   Zustaende (Caps Word, Layer Lock, Jiggler, Mac-Modus) sind bewusst NICHT
   hier: die traegt die Status-LED (Michael, 2026-08-04). Deshalb braucht diese
   Datei auch keinen eigenen Split-Sync -- layer_state synchronisiert QMK
   selbst (SPLIT_LAYER_STATE_ENABLE), und mehr liest sie nicht. Die *Tasten*
   dieser Zustaende behalten hier ihre Identitaetsfarbe, nur die Rueckmeldung
   "der Zustand ist gerade an" sitzt auf der Status-LED.
   ========================================================================== */
#include "wechselbalg.h"

#ifdef WB_RGB_LANGUAGE

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} wb_color_t;

/* ---- Palette ----------------------------------------------------------- */

#define WB_C(r_, g_, b_) ((wb_color_t){(r_), (g_), (b_)})

#define WB_C_OFF        WB_C(  0,   0,   0)

// Layer. Die Basis-Layouts haben bewusst keine Farbe: ADJUSTs zwei
// Umschaltreihen sind zehn Tasten fuer fuenf Aktionen, und sie einzufaerben
// hat in KMK auf Hardware nur die Tasten begraben, nach denen man dort
// wirklich sucht.
#define WB_C_SYM        WB_C(  0,   0, 255)
#define WB_C_NAV        WB_C(  0, 255,   0)
#define WB_C_NUM        WB_C(255, 140,   0)
#define WB_C_GAMING     WB_C(160,   0, 255)
#define WB_C_ADJUST     WB_C(255,   0,   0)

// Modifier. Links und rechts teilen sich eine Farbe -- zwischen LShift und
// RShift gibt es visuell nichts zu unterscheiden, und AltGr liest sich auf
// einem deutschen Layout als "die Alt-Taste".
#define WB_C_CTRL       WB_C(255,   0, 140)
#define WB_C_SHIFT      WB_C(255, 255,   0)
#define WB_C_ALT        WB_C(255,  45,   0)
#define WB_C_GUI        WB_C(  0, 120, 255)

// Inhalt
#define WB_C_PAYLOAD    WB_C(255, 255, 255)
#define WB_C_MOUSE      WB_C(120, 120, 120)

// Einzeltasten. Cyan heisst Layer-*Navigation* und nur das: raus aus dem
// Layer, auf dem du bist (LR_EXIT, obere linke Ecke jedes Overlays; D__PREV,
// GAMINGs Rueckweg), oder ihn festhalten (F_LLOCK).
#define WB_C_LAYER_MGMT WB_C(  0, 255, 255)
#define WB_C_BOOT       WB_C(255,   0, 255)  // magenta, nicht rot: liegt auf ADJUST
#define WB_C_CAPS_WORD  WB_C(255, 255, 255)
#define WB_C_JIGGLER    WB_C(170, 255,   0)
#define WB_C_HOST       WB_C(  0, 120, 255)  // die Taste, die Ctrl/GUI tauscht, in GUIs Farbe

/* ---- Klassifikation ---------------------------------------------------- */

enum wb_modifier_role {
    WB_ROLE_CTRL  = 1 << 0,
    WB_ROLE_SHIFT = 1 << 1,
    WB_ROLE_ALT   = 1 << 2,
    WB_ROLE_GUI   = 1 << 3,
};

/*
Die Modifier-Rollen, fuer die eine Taste steht -- Shift fuer ein blankes
KC_LSFT wie fuer LSFT_T(x), Shift+Ctrl fuer SFT_CTL, nichts fuer LCTL(KC_C).

Der letzte Fall ist der interessante: NX_COPY & Co. sind Zeichen mit einem
Modifier davor, keine Modifier. QMK legt beide als QK_MODS ab und
unterscheidet sie am Basis-Keycode -- ist der selbst ein Modifier, ist die
Taste eine Modifier-Kombination. Das ist genau KMKs `key.key is None`.

MOD_BIT() legt die rechten Modifier ins obere Nibble (KC_RCTL -> 0x10), das
Mod-Tap-Feld benutzt dafuer Bit 4. Beides wird hier auf die vier Rollen
heruntergefaltet.
*/
static uint8_t wb_modifier_roles(uint16_t keycode) {
    if (IS_MODIFIER_KEYCODE(keycode)) {
        const uint8_t bit = MOD_BIT(keycode);
        return (bit | (bit >> 4)) & 0x0F;
    }
    if (IS_QK_MOD_TAP(keycode)) {
        return QK_MOD_TAP_GET_MODS(keycode) & 0x0F;
    }
    if (IS_QK_MODS(keycode)) {
        const uint16_t basic = QK_MODS_GET_BASIC_KEYCODE(keycode);
        if (!IS_MODIFIER_KEYCODE(basic)) {
            return 0;  // ein Zeichen mit Modifier, kein Modifier
        }
        const uint8_t bit = MOD_BIT(basic);
        return (QK_MODS_GET_MODS(keycode) | bit | (bit >> 4)) & 0x0F;
    }
    return 0;
}

/*
Eine kombinierte Modifier-Taste (NAVs Shift+Ctrl) bekommt die Mischung ihrer
Teile, damit sie sich von dem blanken Shift und dem blanken Ctrl direkt
daneben in derselben Reihe unterscheidet.
*/
static wb_color_t wb_modifier_color(uint8_t roles) {
    uint16_t r = 0, g = 0, b = 0, n = 0;

#define WB_ADD_ROLE(role, color)                              \
    if (roles & (role)) {                                     \
        const wb_color_t c = (color);                         \
        r += c.r;                                             \
        g += c.g;                                             \
        b += c.b;                                             \
        n++;                                                  \
    }

    WB_ADD_ROLE(WB_ROLE_CTRL, WB_C_CTRL)
    WB_ADD_ROLE(WB_ROLE_SHIFT, WB_C_SHIFT)
    WB_ADD_ROLE(WB_ROLE_ALT, WB_C_ALT)
    WB_ADD_ROLE(WB_ROLE_GUI, WB_C_GUI)
#undef WB_ADD_ROLE

    return WB_C(r / n, g / n, b / n);
}

/*
Der Layer, den eine Taste erreicht -- MO/TO/DF/TG/TT und die Hold-Seite eines
Layer-Taps tragen ihn alle. F_LLOCK hat kein Ziel (es haelt fest, was ohnehin
schon an ist) und ist deshalb bewusst keiner davon.
*/
static int16_t wb_layer_target(uint16_t keycode) {
    if (IS_QK_LAYER_TAP(keycode)) return QK_LAYER_TAP_GET_LAYER(keycode);
    if (IS_QK_MOMENTARY(keycode)) return QK_MOMENTARY_GET_LAYER(keycode);
    if (IS_QK_TOGGLE_LAYER(keycode)) return QK_TOGGLE_LAYER_GET_LAYER(keycode);
    if (IS_QK_LAYER_TAP_TOGGLE(keycode)) return QK_LAYER_TAP_TOGGLE_GET_LAYER(keycode);
    if (IS_QK_TO(keycode)) return QK_TO_GET_LAYER(keycode);
    if (IS_QK_DEF_LAYER(keycode)) return QK_DEF_LAYER_GET_LAYER(keycode);
    return -1;
}

static bool wb_layer_color(uint8_t layer, wb_color_t *out) {
    switch (layer) {
        case _SYM:
            *out = WB_C_SYM;
            return true;
        case _NAV:
            *out = WB_C_NAV;
            return true;
        case _NUM:
            *out = WB_C_NUM;
            return true;
        case _GAMING:
            *out = WB_C_GAMING;
            return true;
        case _ADJUST:
            *out = WB_C_ADJUST;
            return true;
        default:
            return false;  // Basis-Layouts tragen keine Farbe
    }
}

/*
Der Tri-Layer-Hinweis: solange _NAV gehalten wird, ist die _NUM-Taste in
Wahrheit die ADJUST-Taste -- also traegt sie ADJUSTs Rot. Und andersherum.
Faellt weg, sobald _ADJUST wirklich an ist, weil der Hinweis dann erfuellt ist.

Trifft genau die beiden inneren Daumen (NUM_ENT links, NAV_BSC rechts), weil
nur die auf dem jeweils anderen Layer nicht ueberschrieben werden -- ohne dass
das hier irgendwo steht.
*/
static uint8_t wb_tri_layer_hint(uint8_t target) {
#ifdef WB_NO_TRI_LAYER_HINT
    /*
    Nicht jedes Board hat Tri-Layer: GMMK Pro und K3 Pro kommen ueber eine
    eigene MO__ADJ-Taste nach _ADJUST, halten von _NAV+_NUM tut dort nichts.
    Der Hinweis waere dann keine Hilfe, sondern eine Falschaussage -- eine
    Taste rot faerben heisst "die fuehrt nach ADJUST".
    Gesetzt per -DWB_NO_TRI_LAYER_HINT in der Keymap-rules.mk des Boards.
    */
    return target;
#else
    if (layer_state_is(_ADJUST)) {
        return target;
    }
    if (target == _NUM && layer_state_is(_NAV)) return _ADJUST;
    if (target == _NAV && layer_state_is(_NUM)) return _ADJUST;
    return target;
#endif
}

/*
Semantische Tastenklassen -- der Teil, der aus QMKs alten
Beleuchtungsschemata stammt (der beleuchtete Nummernblock auf _NUM, das
Pfeilkreuz auf _NAV) und frueher als handgezaehlte LED-Indizes gepflegt wurde
(SET_NUMPAD/SET_GAMING in der Keymap).

Er ist hier eine reine Keycode-Regel, weil keymap_neo2.h auf Standard-Keycodes
abbildet: N3_NUM0..9 *sind* KC_P0..P9 und liegen als KC_KP_1..KC_KP_0
zusammenhaengend, N3___UP/LEFT/DOWN/RGHT *sind* KC_UP/LEFT/DOWN/RIGHT und
liegen als KC_RIGHT..KC_UP zusammenhaengend. Damit wandern die Bloecke mit,
wenn die Tasten im Keymap umziehen.
*/
static bool wb_payload_color(uint16_t keycode, uint8_t layer, wb_color_t *out) {
    if (IS_MOUSE_KEYCODE(keycode)) {
        *out = WB_C_MOUSE;
        return true;
    }
    if ((keycode >= KC_KP_1 && keycode <= KC_KP_0) ||  // Nummernblock
        (keycode >= KC_RIGHT && keycode <= KC_UP)) {   // Pfeilkreuz
        *out = WB_C_PAYLOAD;
        return true;
    }
    /*
    Das Bewegungskreuz auf _GAMING. Anders als Nummernblock und Pfeilkreuz ist
    das KEINE layerfreie Keycode-Regel: W/A/S/D sind gewoehnliche Buchstaben
    und stehen auf jedem Basis-Layout, wo sie nichts hervorzuheben haben.
    Deshalb an _GAMING gebunden -- innerhalb dieses Layers wandern sie
    trotzdem mit, wenn sie im Keymap umziehen.
    */
    if (layer == _GAMING) {
        switch (keycode) {
            case KC_W:
            case KC_A:
            case KC_S:
            case KC_D:
                *out = WB_C_PAYLOAD;
                return true;
        }
    }
    return false;
}

/* ---- Die Prioritaetskette ---------------------------------------------- */

static wb_color_t wb_color_for(uint16_t keycode, uint8_t layer, uint8_t base_layer, wb_color_t glow) {
    // 1. KC_NO heisst "hier ist nichts auf diesem Layer" -- anders als KC_TRNS,
    //    das durchfaellt. Das ist die Regel, die einen Overlay-Layer lesbar
    //    macht: er zeigt nur noch, was er wirklich belegt.
    if (keycode == KC_NO) {
        return WB_C_OFF;
    }

    // 2. Einzeltasten, die es wert sind, beim Namen genannt zu werden.
    switch (keycode) {
        case QK_BOOT:
            return WB_C_BOOT;
        case QK_CAPS_WORD_TOGGLE:
            return WB_C_CAPS_WORD;
        case QK_LAYER_LOCK:
            return WB_C_LAYER_MGMT;
        case LR_EXIT:
        case DF_PREV:
            return WB_C_LAYER_MGMT;
        case A_MSJIG:
            return WB_C_JIGGLER;
        case CG_TOGG:
            return WB_C_HOST;
    }

    // 3. Modifier behalten ihre Farbe, wo immer sie auftauchen -- die
    //    Daumenreihe, die beiden Shift-Pinkies der Basis-Layouts, NAVs
    //    Modifier-Homerow, NUMs AltGr.
    const uint8_t roles = wb_modifier_roles(keycode);
    if (roles) {
        return wb_modifier_color(roles);
    }

    // 4. Wer einen Layer erreicht, traegt dessen Farbe dauerhaft -- damit die
    //    NAV-Taste schon NAVs Gruen zeigt, bevor man sie haelt, und nicht erst
    //    danach. Eine Taste ist entweder Modifier oder Layer-Zugang, 3 und 4
    //    koennen sich also nicht in die Quere kommen.
    const int16_t target = wb_layer_target(keycode);
    if (target >= 0) {
        wb_color_t color;
        if (wb_layer_color(wb_tri_layer_hint((uint8_t)target), &color)) {
            return color;
        }
    }

    // 5. Der Nutzblock des Layers.
    wb_color_t payload;
    if (wb_payload_color(keycode, layer, &payload)) {
        return payload;
    }

    // 6. Der aktive Overlay-Layer, an den Positionen die er wirklich belegt.
    //    Genau das sagt layer_switch_get_layer() bereits -- an einer
    //    durchgereichten Position haette es das Basis-Layout geliefert.
    //
    //    _GAMING zaehlt hier mit, obwohl es ein *Default*-Layer ist und damit
    //    `layer == base_layer` gilt: es ist ein Modus, in dem man ist, kein
    //    Layout, auf dem man tippt -- und genau das soll das Board zeigen.
    //    Die uebrigen Basis-Layouts bleiben bewusst farblos (siehe oben).
    if (layer != base_layer || layer == _GAMING) {
        wb_color_t color;
        if (wb_layer_color(layer, &color)) {
            return color;
        }
    }

    // 7. Grundleuchten.
    return glow;
}

/*
Skalierung mit der eingestellten Helligkeit.

rgb_matrix_set_color() schreibt rohes RGB, ohne Skalierung. Da wir hier JEDE
LED selbst setzen, wuerde der Helligkeitsregler sonst wirkungslos bleiben --
deshalb geht jede Farbe hier durch. Das ist die Nahtstelle zu C7 (Encoder auf
_ADJUST = Helligkeit): ein Regler dimmt das ganze Board, Indikatoren inklusive.
*/
static inline uint8_t wb_scale(uint8_t channel, uint8_t val) {
    return (uint8_t)(((uint16_t)channel * (uint16_t)val) / 255);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    const uint8_t base = get_highest_layer(default_layer_state);
    const uint8_t val  = rgb_matrix_get_val();

    // Das Grundleuchten kommt aus der laufenden Konfiguration, damit die
    // Hue/Sat-Tasten auf _ADJUST etwas tun: sie faerben es. Bewusst mit voller
    // Helligkeit erzeugt -- die Skalierung passiert unten fuer alle Farben an
    // derselben Stelle, sonst wuerde ausgerechnet das Grundleuchten doppelt
    // gedimmt und liefe der Palette davon.
    // hsv_to_rgb() und nicht rgb_matrix_hsv_to_rgb(): letzteres steht nur in
    // rgb_matrix.c und ist in keinem oeffentlichen Header deklariert -- es ist
    // ohnehin nur ein schwacher Wrapper um genau diese Funktion.
    const hsv_t      base_hsv = {rgb_matrix_get_hue(), rgb_matrix_get_sat(), UINT8_MAX};
    const rgb_t      base_rgb = hsv_to_rgb(base_hsv);
    const wb_color_t glow     = WB_C(base_rgb.r, base_rgb.g, base_rgb.b);

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            const uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED || led < led_min || led >= led_max) {
                continue;  // keine LED, oder die der anderen Haelfte
            }

            const keypos_t pos     = {.row = row, .col = col};
            const uint8_t  layer   = layer_switch_get_layer(pos);
            const uint16_t keycode = keymap_key_to_keycode(layer, pos);
            const wb_color_t color = wb_color_for(keycode, layer, base, glow);

            rgb_matrix_set_color(led, wb_scale(color.r, val), wb_scale(color.g, val), wb_scale(color.b, val));
        }
    }

    wb_rgb_extra_leds(led_min, led_max, val);
    return false;
}

/*
Haken fuer LEDs, die an *keiner* Matrixposition haengen.

Die Schleife oben laeuft ueber die Matrix, erreicht also nur LEDs, die zu einer
Taste gehoeren. Auf Sofle Choc und K3 Pro ist das jede einzelne; die GMMK Pro
hat daneben 16 im seitlichen Lichtbalken, die keiner Taste zugeordnet sind.

Wer nichts ueberschreibt, laesst dort stehen, was der laufende Effekt gemalt
hat -- deshalb tut die schwache Vorgabe hier bewusst nichts: ohne Board-Code
animiert der Balken einfach weiter. Das Board kann die Funktion ueberschreiben,
um ihn stattdessen zu belegen (GMMK Pro: Statusanzeige, umschaltbar).

`val` ist bereits die laufende Helligkeit -- wer hier faerbt, skaliert selbst,
genau wie die Schleife oben.
*/
__attribute__((weak)) void wb_rgb_extra_leds(uint8_t led_min, uint8_t led_max, uint8_t val) {
    (void)led_min;
    (void)led_max;
    (void)val;
}

#endif  // WB_RGB_LANGUAGE
