#include "wechselbalg.h"
#include "layer_lock.h"

// Deutsche Caps-Word-Regeln (Core-Feature CAPS_WORD_ENABLE), gemeinsam fuer alle Boards
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case DE___AE:
        case DE___OE:
        case DE___UE:
        case DE_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case N3_NUM1 ... N3_NUM0:
        case DE_SS:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

/* ------------------------------------------------------------------------
   Retro Tapping mit Zeitfenster (RETRO_TAPPING_PER_KEY)

   QMK kennt kein retro_tap_timeout, KMK schon -- und der Grund dafuer gilt
   hier genauso: HoldTap zaehlt nur Tasten*druecke* als "der Hold wurde
   benutzt", und der Klick einer echten Maus erreicht die Firmware nie. Ohne
   Fenster schiebt Shift+Klick beim Loslassen ein Leerzeichen nach und
   Alt+Klick ein PrintScreen aus RALT_PR.

   Layer-Taps bekommen ein grosszuegiges Fenster: eine Layer-Taste haelt
   niemand, um damit zu klicken. Mod-Taps ein kurzes -- lange Maus-Chords
   (Alt+Ziehen, Ctrl+Klick durch eine Liste) fallen dann heraus. Ein sehr
   kurzer Ctrl+Klick liegt weiterhin im Fenster; die beiden Faelle sind
   zeitlich nicht sauber trennbar (in QMK so wenig wie in KMK). Der Griff,
   der es sicher macht: den Modifier nach dem Klick einen Moment laenger
   halten.

   get_retro_tapping() wird beim *Release* ausgewertet (quantum/action.c),
   der Zeitstempel im record ist dann der des Loslassens -- die Press-Zeit
   muessen wir selbst mitschreiben. Ein einzelner static reicht: QMKs
   Retro-Tap feuert ohnehin nur, wenn zwischen Druck und Loslassen der Taste
   *keine* andere Taste gedrueckt wurde (retro_tap_primed), der gemerkte
   Zeitpunkt gehoert also immer zur richtigen Taste.
   ------------------------------------------------------------------------ */
#ifdef RETRO_TAPPING_PER_KEY
#define WB_RETRO_TAP_TERM_LT 1000
#define WB_RETRO_TAP_TERM_MT 500

static uint16_t wb_tap_hold_press_time = 0;

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_LAYER_TAP(keycode)) {
        return timer_elapsed(wb_tap_hold_press_time) < WB_RETRO_TAP_TERM_LT;
    }
    if (IS_QK_MOD_TAP(keycode)) {
        return timer_elapsed(wb_tap_hold_press_time) < WB_RETRO_TAP_TERM_MT;
    }
    return false;
}
#endif  // RETRO_TAPPING_PER_KEY

/* ------------------------------------------------------------------------
   Wortweise Navigation, host-abhaengig

   FF_WORD/RV_WORD sind SEND_STRING-Makros und laufen damit am Keymap vorbei
   -- CG_TOGG (mod_config() beim Aufloesen der Taste) erreicht sie nicht.
   Sie sind aber ohnehin kein Fall fuer einen Modifier-Tausch: wortweise
   Bewegung ist unter Windows/Linux Ctrl+Pfeil, unter macOS Opt+Pfeil. Es ist
   also ein anderer Modifier, nicht ein vertauschter -- deshalb hier eine
   echte Fallunterscheidung statt eines eigenen Mac-_NAV-Layers.
   ------------------------------------------------------------------------ */
static void wb_word_jump(bool forward) {
    if (WB_HOST_IS_MAC()) {
        if (forward) {
            SEND_STRING(SS_LALT(SS_TAP(X_RIGHT) SS_TAP(X_RIGHT) SS_TAP(X_LEFT)));
        } else {
            SEND_STRING(SS_LALT(SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_RIGHT)));
        }
    } else {
        if (forward) {
            SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT) SS_TAP(X_RIGHT) SS_TAP(X_LEFT)));
        } else {
            SEND_STRING(SS_LCTL(SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_RIGHT)));
        }
    }
}

/* ------------------------------------------------------------------------
   DF_PREV -- zurueck auf das vorherige Basis-Layout (C6)

   Der Ausstieg aus _GAMING, ohne hart auf QWERT zu springen. Nachbau von
   KMKs Layers.previous_default_layer.

   Der Trick steckt in QMKs Reihenfolge: default_layer_state_set() (siehe
   quantum/action_layer.c) ruft erst die _kb/_user-Kette und weist
   `default_layer_state` **danach** zu -- im Callback steht also noch der alte
   Wert. Damit reicht ein Vergleich, kein zweiter Zustand.

   Faengt DF() und set_single_persistent_default_layer() gleichermassen ab,
   weil beide durch dieselbe Funktion laufen. Und weil der Ruecksprung selbst
   wieder durch den Hook geht, bringt zweimal Druecken einen dorthin zurueck,
   wo man war -- genau wie in KMK.

   Bewusst nicht persistent (kein EEPROM): _GAMING wird per DF() betreten,
   also nur fuer die Sitzung, und der Ausstieg muss dazu passen.

   Direkt nach dem Boot zeigt es auf _QWERT. Ist ein anderes Layout
   persistent hinterlegt, landet der allererste Druck also dort -- gelesen
   als "vorher war Layer 0" ist das richtig, KMK wuerde in dem Fall nichts tun.
   ------------------------------------------------------------------------ */
#ifdef WB_DF_PREV
static uint8_t wb_prev_default_layer = _QWERT;

layer_state_t default_layer_state_set_user(layer_state_t state) {
    const uint8_t next    = get_highest_layer(state);
    const uint8_t current = get_highest_layer(default_layer_state);  // noch der alte Wert
    if (next != current) {
        wb_prev_default_layer = current;
    }
    return state;
}
#endif  // WB_DF_PREV

/* ------------------------------------------------------------------------
   Mouse Jiggler auf A_MSJIG (C4)

   QMK hat dafuer kein Core-Feature. Takt per housekeeping_task_user() statt
   DEFERRED_EXEC_ENABLE: fuer genau einen Timer sind ein uint32 und ein
   timer_elapsed32() billiger als quantum/deferred_exec.c, und der Callback
   liefe ohnehin im selben Schleifendurchlauf.

   Bewegung als roher HID-Report und nicht per tap_code(MS_LEFT): Mouse Keys
   bewegen um MOUSEKEY_MOVE_DELTA *mit Beschleunigung*, hier soll es genau
   ein Pixel sein.

   Die Richtung alterniert (aus KMK uebernommen): upstream wuerfelte sie pro
   Takt, was ein 2D-Random-Walk ist -- der Zeiger driftet ueber einen
   Arbeitstag zig Pixel weg. Mit wechselndem Vorzeichen ist er beim naechsten
   Takt wieder da, wo er war.

   Der Report traegt auch die Maustasten, ein {0}-Report wuerde also eine
   gerade gehaltene Taste loslassen. Deshalb der buttons-Check: waehrend
   wirklich mit der Maus gearbeitet wird, setzt der Jiggler einen Takt aus.

   Laeuft auf beiden Haelften nur auf dem Master -- QMK verarbeitet alle
   Key-Events dort, und host_mouse_send() gibt es nur da sinnvoll.
   Ein Tastendruck beendet ihn *nicht* (Michael, 2026-08-04), nur A_MSJIG.
   ------------------------------------------------------------------------ */
#ifdef WB_JIGGLER
#define WB_JIGGLE_PERIOD_MS 5000
#define WB_JIGGLE_STEP      1

static bool     wb_jiggling     = false;
static int8_t   wb_jiggle_dir   = WB_JIGGLE_STEP;
static uint32_t wb_jiggle_timer = 0;

void housekeeping_task_user(void) {
    if (!wb_jiggling || timer_elapsed32(wb_jiggle_timer) < WB_JIGGLE_PERIOD_MS) {
        return;
    }
    wb_jiggle_timer = timer_read32();

    if (mousekey_get_report().buttons != 0) {
        return;  // Maustaste gehalten -- diesen Takt auslassen
    }

    report_mouse_t report = {0};
    report.x              = wb_jiggle_dir;
    report.y              = wb_jiggle_dir;
    host_mouse_send(&report);
    report.x = 0;
    report.y = 0;
    host_mouse_send(&report);

    wb_jiggle_dir = -wb_jiggle_dir;
}
#endif  // WB_JIGGLER

/* ------------------------------------------------------------------------
   Gemeinsames process_record_user()

   Lag frueher kopiert in jeder Board-Keymap. Board-spezifisches kommt jetzt
   in process_record_keymap(), das am Ende aufgerufen wird.
   ------------------------------------------------------------------------ */
__attribute__((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RETRO_TAPPING_PER_KEY
    if (record->event.pressed && (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode))) {
        wb_tap_hold_press_time = record->event.time;
    }
#endif

    const uint8_t mods         = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();

    switch (keycode) {
        case LR_EXIT:
            if (record->event.pressed) {
                layer_lock_all_off();
                layer_clear();
            }
            return false;
#ifdef WB_DF_PREV
        case DF_PREV:
            if (record->event.pressed) {
                default_layer_set((layer_state_t)1 << wb_prev_default_layer);
            }
            return false;
#endif
#ifdef WB_JIGGLER
        case A_MSJIG:
            if (record->event.pressed) {
                wb_jiggling     = !wb_jiggling;
                wb_jiggle_timer = timer_read32();
            }
            return false;
#endif
        case QWERT:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERT);
            }
            return false;
#ifdef WB_LAYOUT_DVORAK
        case DVORAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DVORAK);
            }
            return false;
#endif
#ifdef WB_LAYOUT_COLEMAKDH
        case COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAKDH);
            }
            return false;
#endif
#ifdef WB_LAYOUT_MINE
        case MINE:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_MINE);
            }
            return false;
#endif
#ifdef WB_LAYOUT_VOU
        case VOU:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_VOU);
            }
            return false;
#endif
        case FF_WORD:
            if (record->event.pressed) {
                wb_word_jump(true);
            }
            return false;
        case RV_WORD:
            if (record->event.pressed) {
                wb_word_jump(false);
            }
            return false;
        case DBRACES:  // Types [], {}, or <> and puts cursor between braces.
            if (record->event.pressed) {
                clear_oneshot_mods();  // Temporarily disable mods.
                unregister_mods(MOD_MASK_CSAG);
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    SEND_STRING("{}");
                } else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                    SEND_STRING("<>");
                } else {
                    SEND_STRING("[]");
                }
                tap_code(KC_LEFT);    // Move cursor between braces.
                register_mods(mods);  // Restore mods.
            }
            return false;
    }

    return process_record_keymap(keycode, record);
}
