#ifdef TAPPING_TERM
    #undef TAPPING_TERM
#endif
/*
150 statt 600: der Wert kommt aus dem KMK-Port derselben Tastatur, dort per
Gefuehl an der Hardware eingestellt (KMK: layouts.py, TAP_TIME). Er ist nur
zusammen mit CHORDAL_HOLD benutzbar -- ohne das werden Roll-Overs auf
derselben Hand zu versehentlichen Modifiern.

Genau deshalb haengen die beiden hier zusammen an einer Bedingung: die vier
ATmega32u4-Boards haben den Flash fuer CHORDAL_HOLD nicht mehr, also behalten
sie auch das alte, lange Tapping-Term. Kein halbes Setup, sondern zwei ganze.
Faellt weg, sobald die AVR-Boards auf groessere Controller umgezogen sind.
*/
#ifdef __AVR__
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

Auf AVR aus, siehe TAPPING_TERM oben.
*/
#ifndef __AVR__
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

Auf AVR bleibt es aus: die vier ATmega32u4-Boards sind am Flash-Limit, und
von den beiden neuen Tap-Hold-Features ist CHORDAL_HOLD das, ohne das
TAPPING_TERM 150 nicht funktioniert -- Retro Tapping ist Komfort. Faellt weg,
sobald die Boards auf groessere Controller umgezogen sind.
*/
#ifndef __AVR__
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
