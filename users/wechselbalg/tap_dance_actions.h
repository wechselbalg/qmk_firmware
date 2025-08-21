// Zentrale Definition aller Tap Dance Actions

#pragma once
#include "tap_dance.h"
#include "keymap_german.h"

// Zentrales Array - wird in keymap.c included
#define WECHSELBALG_TAP_DANCE_ACTIONS \
    [ESC_CIRC] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, DE_CIRC), \
    [F6___F12] = ACTION_TAP_DANCE_DOUBLE(KC_F6, KC_F12), \
    [SS___F11] = ACTION_TAP_DANCE_DOUBLE(DE_SS, KC_F11), \
    [PSCR_APP] = ACTION_TAP_DANCE_DOUBLE(KC_PSCR, KC_APP)

// Verwendung in keymap.c:
// tap_dance_action_t tap_dance_actions[] = {
//     WECHSELBALG_TAP_DANCE_ACTIONS
// };
