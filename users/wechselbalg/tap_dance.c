#include "tap_dance.h"
#include "keymap_german.h"

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [ESC_CIRC] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, DE_CIRC),
    [F6___F12] = ACTION_TAP_DANCE_DOUBLE(KC_F6, KC_F12),
    [SS___F11] = ACTION_TAP_DANCE_DOUBLE(DE_SS, KC_F11),
    [PSCR_APP] = ACTION_TAP_DANCE_DOUBLE(KC_PSCR, KC_APP),
};
