#pragma once
#include "quantum.h"

// Tap Dance declarations
enum {
    ESC_CIRC,
    F6___F12,
    SS___F11,
    PSCR_APP,
};

// Convenience Macros für die Verwendung in Layouts
#define TD_ESCC TD(ESC_CIRC)
#define TD_F612 TD(F6___F12)
#define TD_SF11 TD(SS___F11)
#define TD_PSAP TD(PSCR_APP)

// Hinweis: Das tap_dance_actions Array muss in der keymap.c definiert werden!
