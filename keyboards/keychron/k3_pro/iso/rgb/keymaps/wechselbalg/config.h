#pragma once

#ifdef RGB_MATRIX_ENABLE
/*
Beleuchtung fuer die abgeleitete Farbsprache (C8, users/wechselbalg/rgb_language.c),
angeschaltet per WB_RGB_LANGUAGE in der rules.mk daneben.

Diese Datei wird NACH keyboards/keychron/k3_pro/iso/rgb/config.h eingelesen --
deshalb kann sie deren Animationsliste per #undef wieder abraeumen. Die
Board-config.h selbst bleibt unangetastet, weil sich die Keymaps `default` und
`via` sie mit uns teilen und ihre Beleuchtung behalten sollen.

Warum die Animationen weg muessen (dieselbe Begruendung wie bei der Sofle Choc):
sie waeren ohnehin unsichtbar, weil die Farbsprache jeden Frame ueber alle 85
Tasten-LEDs schreibt -- uebrig blieben nur RGB_MOD/RGB_RMOD auf _ADJUST, die
dann zwischen Modi umschalten, die man nicht sieht. Uebrig bleibt
RGB_MATRIX_SOLID_COLOR, das QMK immer mitbaut; dessen Hue/Sat faerben bei uns
das **Grundleuchten** und Val dimmt das ganze Board inklusive aller Indikatoren.

⚠️ Ohne die DEFAULT_-Zeilen unten startet das Board mit QMKs Vorgaben, und die
sind hier gleich doppelt falsch: HUE 0 + SAT 255 ergibt ein knallrotes
Grundleuchten -- ausgerechnet die Farbe, die in der Palette `_ADJUST` bedeutet --
und DEFAULT_VAL faellt auf RGB_MATRIX_MAXIMUM_BRIGHTNESS, also volle Helligkeit.
Genau so sah das Board nach dem ersten Flash aus (2026-08-05).
*/

// Grundleuchten: warmes Cremeweiss, identisch zur Sofle Choc.
// VAL 40 ist derselbe absolute Pegel wie dort -- wb_scale() in rgb_language.c
// rechnet mit val/255, unabhaengig von RGB_MATRIX_MAXIMUM_BRIGHTNESS.
// Zu dunkel/zu hell? RGB_VAI/RGB_VAD liegen auf _ADJUST, das braucht kein
// Neuflashen; nur der Startwert steht hier.
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#    define RGB_MATRIX_DEFAULT_HUE 25
#    define RGB_MATRIX_DEFAULT_SAT 135
#    define RGB_MATRIX_DEFAULT_VAL 40

#    define RGB_MATRIX_HUE_STEP 8
#    define RGB_MATRIX_SAT_STEP 8
#    define RGB_MATRIX_VAL_STEP 8

// Reaktive und Framebuffer-Effekte existieren nur fuer die Animationen unten.
#    undef RGB_MATRIX_KEYPRESSES
#    undef RGB_MATRIX_FRAMEBUFFER_EFFECTS

// Alle Animationen aus keyboards/keychron/k3_pro/iso/rgb/config.h abschalten.
#    undef ENABLE_RGB_MATRIX_BREATHING
#    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#    undef ENABLE_RGB_MATRIX_CYCLE_ALL
#    undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#    undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#    undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#    undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#    undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#    undef ENABLE_RGB_MATRIX_DUAL_BEACON
#    undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#    undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#    undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#    undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#    undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#    undef ENABLE_RGB_MATRIX_SPLASH
#    undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#endif
