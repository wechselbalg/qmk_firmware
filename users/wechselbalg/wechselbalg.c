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
   Mac-Modus: die AltGr-Ebene der deutschen Belegung liegt woanders

   Zweiter Fall derselben Sorte wie wb_word_jump() darueber, aber aus einem
   anderen Grund: hier ist nicht der Modifier ein anderer, sondern die
   *Basistaste*. @ ist unter Windows/Linux AltGr+Q, unter macOS Opt+L -- der
   Modifier (RAlt = rechte Option) ist auf beiden Systemen derselbe. Ein
   Modifier-*Tausch* wie CG_TOGG kann das prinzipiell nicht reparieren.

   Deshalb werden die betroffenen Keycodes im Mac-Modus abgefangen und durch
   ihre Mac-Entsprechung ersetzt, statt neue Keycodes ins Keymap zu legen.
   Das wirkt auf allen Layern und allen Boards zugleich, laesst die Keymaps
   unangetastet und -- der eigentliche Grund -- kostet keinen Eintrag in
   enum CustomKeys: dessen Verschiebung war die Ursache des FN_EXIT-Bugs.

   Unveraendert bleibt alles mit S(...): ° (S(DE_CIRC)) und § (S(DE_3))
   liegen auf beiden Systemen auf derselben Shift-Kombination. Ebenso € und
   µ, die auf dem Mac zufaellig auf derselben Taste sitzen.

   ~ ist der Sonderfall: Opt+N ist auf dem Mac ein Dead Key und braucht ein
   Leerzeichen hinterher, passt also nicht in die Tabelle. Siehe unten.

   Ohne MAGIC_ENABLE gibt es gar keinen Mac-Modus (WB_HOST_IS_MAC() ist dann
   ein hartes false), der ganze Block faellt dort also weg -- die AVR-Boards
   sind nicht betroffen. Einzeln abschaltbar per -DWB_NO_MAC_ALTGR.
   ------------------------------------------------------------------------ */
#if defined(MAGIC_ENABLE) && !defined(WB_NO_MAC_ALTGR)
#    define WB_MAC_ALTGR
#endif

#ifdef WB_MAC_ALTGR
// Mac-Entsprechung eines AltGr-Zeichens, oder 0 wenn es keinen Unterschied gibt.
static uint16_t wb_mac_altgr(uint16_t keycode) {
    switch (keycode) {
        case DE_AT:   return ALGR(DE_L);  // @   AltGr+Q  -> Opt+L
        case DE_LBRC: return ALGR(DE_5);  // [   AltGr+8  -> Opt+5
        case DE_RBRC: return ALGR(DE_6);  // ]   AltGr+9  -> Opt+6
        case DE_LCBR: return ALGR(DE_8);  // {   AltGr+7  -> Opt+8
        case DE_RCBR: return ALGR(DE_9);  // }   AltGr+0  -> Opt+9
        case DE_PIPE: return ALGR(DE_7);  // |   AltGr+<  -> Opt+7
        case DE_BSLS: return RSA(DE_7);   // '\' AltGr+ss -> Shift+Opt+7
        default:      return 0;
    }
}
#endif

/*
Den Keycode auf den laufenden Host uebersetzen. Ausserhalb des Mac-Modus --
und auf jedem Board ohne MAGIC_ENABLE -- die Identitaet, die LTO wegfaltet.
Wird auch von DBRACES gebraucht, das seine Klammern selbst tippt.
*/
static uint16_t wb_localize(uint16_t keycode) {
#ifdef WB_MAC_ALTGR
    if (WB_HOST_IS_MAC()) {
        const uint16_t mac_kc = wb_mac_altgr(keycode);
        if (mac_kc) return mac_kc;
    }
#endif
    return keycode;
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
/*
Die beiden Stellschrauben. Hier aendern gilt fuer alle Boards; ein einzelnes
Board abweichend: -DWB_JIGGLE_PERIOD_MS=... / -DWB_JIGGLE_STEP=... per
OPT_DEFS in dessen Keymap-rules.mk.

  WB_JIGGLE_PERIOD_MS  Taktabstand. Hochsetzen, wenn 5 s unnoetig oft ist;
                       runter, wenn ein Host-Timer schon frueher zuschlaegt.
  WB_JIGGLE_STEP       Pixel pro Takt. Hochsetzen, wenn ein Host einen
                       1-px-Stubser ignoriert -- die Richtung wechselt, der
                       Zeiger kommt also immer wieder an den Ausgangspunkt
                       zurueck, egal wie gross der Schritt ist.
*/
#ifndef WB_JIGGLE_PERIOD_MS
#    define WB_JIGGLE_PERIOD_MS 5000
#endif
#ifndef WB_JIGGLE_STEP
#    define WB_JIGGLE_STEP 1
#endif

static bool     wb_jiggling     = false;
static int8_t   wb_jiggle_dir   = WB_JIGGLE_STEP;
static uint32_t wb_jiggle_timer = 0;

bool wb_is_jiggling(void) {
    return wb_jiggling;
}

static void wb_jiggle_task(void) {
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

/*
Der gemeinsame Takt.

Board-Eigenes haengt sich in housekeeping_task_keymap() ein -- dasselbe Muster
wie process_record_keymap(), und aus demselben Grund: housekeeping_task_user()
darf es nur einmal geben, eine Keymap wuerde sonst ein Doppel-Symbol erzeugen.

Die schwache Vorgabe ist leer und verschwindet per LTO, wenn sie niemand
ueberschreibt -- auf den AVR-Boards bleibt also alles beim Alten (nachgemessen:
das weisse Board aendert sich um 0 Byte).
*/
__attribute__((weak)) void housekeeping_task_keymap(void) {}

void housekeeping_task_user(void) {
#ifdef WB_JIGGLER
    wb_jiggle_task();
#endif
#ifdef WB_STATUS_LED
    wb_status_led_task();
#endif
    housekeeping_task_keymap();
}

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

#ifdef WB_MAC_ALTGR
    if (WB_HOST_IS_MAC()) {
        /* ~ passt nicht in die Tabelle: Opt+N ist auf dem Mac ein Dead Key
           und erscheint erst, wenn ein Zeichen folgt. Ein Leerzeichen loest
           ihn zu einer blanken Tilde auf. */
        if (keycode == DE_TILD) {
            if (record->event.pressed) {
                tap_code16(ALGR(DE_N));
                tap_code(KC_SPC);
            }
            return false;
        }
        const uint16_t mac_kc = wb_mac_altgr(keycode);
        if (mac_kc) {
            // register/unregister statt tap_code16, damit Halten und
            // Auto-Repeat erhalten bleiben wie bei der Taste selbst.
            if (record->event.pressed) {
                register_code16(mac_kc);
            } else {
                unregister_code16(mac_kc);
            }
            return false;
        }
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
                /* Nicht mehr per SEND_STRING: dessen Tabelle (sendstring_german.h)
                   bildet die AltGr-Ebene der *Windows*-Belegung ab, unter macOS
                   kaeme also Murks heraus. Getippt werden jetzt die Keycodes
                   selbst, durch wb_localize() auf den Host uebersetzt.
                   < und > brauchen das nicht -- sie liegen auf beiden Systemen
                   auf derselben Taste. */
                uint16_t open, close;
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    open = DE_LCBR, close = DE_RCBR;
                } else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                    open = DE_LABK, close = DE_RABK;
                } else {
                    open = DE_LBRC, close = DE_RBRC;
                }
                clear_oneshot_mods();  // Temporarily disable mods.
                unregister_mods(MOD_MASK_CSAG);
                tap_code16(wb_localize(open));
                tap_code16(wb_localize(close));
                tap_code(KC_LEFT);    // Move cursor between braces.
                register_mods(mods);  // Restore mods.
            }
            return false;
    }

    return process_record_keymap(keycode, record);
}
