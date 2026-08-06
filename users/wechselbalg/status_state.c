#include "status_state.h"

#include "wechselbalg.h"
#include "caps_word.h"
#include "layer_lock.h"

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

uint8_t wb_status_flags_local(void) {
    uint8_t flags = 0;
    if (is_caps_word_on()) flags |= WB_STATUS_CAPS_WORD;
    if (wb_layer_locked) flags |= WB_STATUS_LAYER_LOCK;
#ifdef WB_JIGGLER
    if (wb_is_jiggling()) flags |= WB_STATUS_JIGGLER;
#endif
    if (WB_HOST_IS_MAC()) flags |= WB_STATUS_MAC;
    return flags;
}

/*
Zeichengleich mit dem, was bis 2026-08-06 in status_led.c stand -- bewusst
inklusive der fruehen returns statt einer if/else-Kette. Damit ist die
Liatris-Sofle nach dem Herausziehen bytegleich geblieben, und das ist bei einem
Board, das schon auf Hardware bestaetigt ist, mehr wert als schoenerer Code.

Auch der Aus-Fall zaehlt als "idle": das Weiterdimmen einer schwarzen Farbe ist
folgenlos, und so stand es im Original.
*/
wb_status_color_t wb_status_color(uint8_t flags, bool *is_idle) {
    *is_idle = false;
    if (flags & WB_STATUS_CAPS_WORD) return WB_SC_CAPS_WORD;
    if (flags & WB_STATUS_LAYER_LOCK) return WB_SC_LAYER_LOCK;
    if (flags & WB_STATUS_JIGGLER) return WB_SC_JIGGLER;

    *is_idle = true;
    return (flags & WB_STATUS_MAC) ? WB_SC_MAC : WB_SC_OFF;
}
