#pragma once

#ifdef RGB_MATRIX_ENABLE
/*
Beleuchtung fuer die abgeleitete Farbsprache (C8, users/wechselbalg/rgb_language.c),
angeschaltet per WB_RGB_LANGUAGE in der rules.mk daneben.

⚠️ Ohne die DEFAULT_-Zeilen unten startet das Board mit QMKs Vorgaben, und die
sind hier gleich doppelt falsch: HUE 0 + SAT 255 ergibt ein knallrotes
Grundleuchten -- ausgerechnet die Farbe, die in der Palette `_ADJUST` bedeutet --
und DEFAULT_VAL faellt auf RGB_MATRIX_MAXIMUM_BRIGHTNESS, also volle Helligkeit.
Genau so sah der K3 Pro nach seinem ersten Flash aus (2026-08-05); die Lehre
steht dort im Kapitel und gilt fuer jedes weitere Board.

## Warum hier die Animationen BLEIBEN (anders als beim K3 Pro)

Die GMMK Pro hat 99 LEDs: 83 unter Tasten und **16 im seitlichen Lichtbalken**,
die keiner Matrixposition zugeordnet sind. rgb_language.c laeuft ueber die
Matrix und schreibt darum ausschliesslich die 83 Tasten-LEDs -- der Balken
bleibt so, wie ihn der laufende Effekt gemalt hat.

Damit ergibt sich hier etwas, das K3 Pro und Sofle nicht koennen: die Tasten
sprechen die Farbsprache, waehrend der Balken weiterlaeuft. RGB_MOD/RGB_RMOD auf
_ADJUST wechseln dann sichtbar die Balken-Animation, statt zwischen unsichtbaren
Modi umzuschalten (Entscheidung Michael, 2026-08-06). Deshalb steht hier kein
einziges `#undef ENABLE_RGB_MATRIX_*` -- der Aufschlag von rund 6 KB ist bei
256 KB Flash belanglos.

Das **Grundleuchten der Tasten** haengt trotzdem nicht am Effekt: rgb_language.c
baut es aus `rgb_matrix_get_hue()/get_sat()`, also aus der *Konfiguration*. Eine
laufende Animation faerbt darum den Balken, nicht die Tasten.
*/

// Grundleuchten: warmes Cremeweiss, identisch zu Sofle Choc und K3 Pro.
// Zu dunkel/zu hell? RGB_VAI/RGB_VAD liegen auf _ADJUST und der Encoder regelt
// es dort ebenfalls -- das braucht kein Neuflashen, hier steht nur der Startwert.
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#    define RGB_MATRIX_DEFAULT_HUE 25
#    define RGB_MATRIX_DEFAULT_SAT 135
#    define RGB_MATRIX_DEFAULT_VAL 40

#    define RGB_MATRIX_HUE_STEP 8
#    define RGB_MATRIX_SAT_STEP 8
#    define RGB_MATRIX_VAL_STEP 8
#endif
