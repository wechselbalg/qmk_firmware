#ifdef TAPPING_TERM
    #undef TAPPING_TERM
#endif
/*
150 statt 600: der Wert kommt aus dem KMK-Port derselben Tastatur, dort per
Gefuehl an der Hardware eingestellt (KMK: layouts.py, TAP_TIME). Er ist nur
zusammen mit CHORDAL_HOLD benutzbar -- ohne das werden Roll-Overs auf
derselben Hand zu versehentlichen Modifiern.

Genau deshalb haengen TAPPING_TERM, CHORDAL_HOLD und RETRO_TAPPING_PER_KEY an
*einer* Bedingung: entweder ein Board hat den Flash fuer alle drei, oder es
behaelt das alte, lange Tapping-Term. Kein halbes Setup, sondern zwei ganze.

Abschalten fuer ein einzelnes Board: WB_NO_ADVANCED_TAP_HOLD in dessen
keymaps/<name>/config.h definieren (die wird vor dieser Datei eingelesen).
Aktuell betrifft das nur die Lotus58 -- sie ist noch nicht auf das
Wrapper-System umgestellt, bringt ihre eigenen sechs Basis-Layouts mit und
profitiert deshalb nicht von der WB_LAYOUT_*-Reduktion in wechselbalg.h.
*/
#ifdef WB_NO_ADVANCED_TAP_HOLD
#    define TAPPING_TERM 600
#else
#    define TAPPING_TERM 150
#endif

/*
Entscheidet den Gegenhand-Fall: Tap-Hold gilt als gehalten, sobald die andere
Taste innerhalb des Tap-Holds gedrueckt UND losgelassen wurde.
*/
#define PERMISSIVE_HOLD

/*
Gleiche Hand innerhalb des Tapping-Terms -> sofort als Tap abschliessen.
Erst das macht TAPPING_TERM 150 benutzbar.

Die Handedness-Tabelle gehoert ins jeweilige keymap.c (chordal_hold_layout,
geschrieben mit dem LAYOUT-Makro des Boards). Ohne Tabelle raet QMK anhand
der Geometrie -- das trennt zwar links/rechts, nimmt aber die Daumenreihe
NICHT aus, und genau das ist hier noetig: Daumen-Chords auf derselben Hand
(NUM-Daumen halten + linke NUM-Taste) sind der Normalfall, nicht die
Ausnahme. In KMK war exakt das ein Hardware-Bug.

Aus, wenn WB_NO_ADVANCED_TAP_HOLD gesetzt ist -- siehe TAPPING_TERM oben.
*/
#ifndef WB_NO_ADVANCED_TAP_HOLD
#    define CHORDAL_HOLD
#endif

/*
Retro Tapping nur ueber die Callback-Variante: get_retro_tapping() in
wechselbalg.c setzt getrennte Zeitfenster fuer Layer-Taps und Mod-Taps und
baut damit KMKs retro_tap_timeout nach, das QMK nicht kennt.

DUMMY_MOD_NEUTRALIZER_KEYCODE wird hier NICHT gebraucht: das schuetzt davor,
dass ein Retro-Tap auf einem GUI/Alt-Mod-Tap am Host ein Menue oeffnet -- die
Mod-Taps dieses Layouts sind aber ausschliesslich Shift (SFT_SPC, RFT_SPC,
SFT_PIP, RFT_MIN) und AltGr (RALT_PR). Sobald ein GUI- oder Alt-Mod-Tap
dazukommt, wieder aufnehmen.

Aus, wenn WB_NO_ADVANCED_TAP_HOLD gesetzt ist -- siehe TAPPING_TERM oben.
*/
#ifndef WB_NO_ADVANCED_TAP_HOLD
#    define RETRO_TAPPING_PER_KEY
#endif

/*
Doppelt tippen und halten wiederholt die Tap-Taste, statt den Hold zu
erreichen -- das QMK-Gegenstueck zu KMKs repeat=HoldTapRepeat.TAP.
*/
#define QUICK_TAP_TERM 80
#define TAPPING_TOGGLE 3
#define TAP_CODE_DELAY 10
#define USB_POLLING_INTERVAL_MS 10

#ifndef NO_DEBUG
#define NO_DEBUG
#endif // !NO_DEBUG
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
#define NO_PRINT
#endif // !NO_PRINT
