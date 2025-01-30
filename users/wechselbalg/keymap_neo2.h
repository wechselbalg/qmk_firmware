#pragma once
#include "keymap.h"

// State bitmap to track which key(s) enabled NEO_3 layer
// static uint8_t neo3_state = 0;
// State bitmap to track key combo for CAPSLOCK
// static uint8_t capslock_state = 0;

// bitmasks for modifier keys
#define MODS_NONE   0
#define MODS_SHIFT  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
#define MODS_CTRL   (MOD_BIT(KC_LCTL)|MOD_BIT(KC_RCTRL))
#define MODS_ALT    (MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
#define MODS_GUI    (MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI))

// // Used to trigger macros / sequences of keypresses
// enum custom_keycodes {
//   PLACEHOLDER = SAFE_RANGE,     // can always be here
//   NEO2_LMOD3,
//   NEO2_RMOD3,
//   YELDIR_AC,
//   YELDIR_CTLTAB,
//   YELDIR_CTLSTAB,
//   NEO2_1,
//   NEO2_2,
//   NEO2_3,
//   NEO2_4,
//   NEO2_5,
//   NEO2_6,
//   NEO2_7,
//   NEO2_8,
//   NEO2_9,
//   NEO2_0,
//   NEO2_MINUS,
//   NEO2_COMMA,
//   NEO2_DOT,
//   NEO2_SHARP_S
// };

#define ___NO__ KC_NO

#define NEO2_LMOD4                   MO(NEO_4)
#define NEO2_RMOD4                   NEO2_LMOD4

// Use _______ to indicate a key that is transparent / falling through to a lower level
#define _______ KC_TRNS



// NEO2 Layer 2 - Line 1
#define N2_HCEK        UC(L'ˇ')        // ˇ
#define N2__DEG        DE_DEG          // °
#define N2_SECT        DE_SECT         // §
#define N2_SS_L        UC(L'ℓ')        // ℓ
#define N2_RAQO        RALT(DE_Y)      // »
#define N2_LAQO        RALT(DE_X)      // «
#define N2__DLR        DE_DLR          // $
#define N2__EUR        ALGR(DE_E)      // €
#define N2_LODQ        RALT(DE_V)      // „
#define N2_L_DQ        RALT(DE_B)      // “
#define N2_R_DQ        RALT(DE_N)      // ”
#define N2_C_SS        RSA(DE_S)       // ẞ

// NEO2 Layer 2 - Line 2
#define N2_TILD        DE_TILD           // ~

// NEO2 Layer 2 - Line 4
#define N2_NDSH        RALT(DE_MINS)   // –   RALT(DE_MINS)
#define N2_BULT        UC(L'•')        // •
#define N2_MDSH        RSA(DE_MINS)    // —   RSA(DE_MINS)

// NEO2 Layer 3 - Line 1
#define N3_SUP1        RALT(DE_1)      // ¹   RALT(DE_1)
#define N3_SUP2        DE_SUP2         // ²
#define N3_SUP3        DE_SUP3         // ³
#define N3_RSAQ        RSA(DE_Y)       // ›   RSA(DE_Y)
#define N3_LSAQ        RSA(DE_X)       // ‹   RSA(DE_X)
#define N3_CENT        RALT(DE_C)      // ¢   RALT(DE_C)
#define N3__YEN        RSA(DE_Z)       // ¥   RSA(DE_Z)
#define N3_SBSQ        RSA(DE_V)       // ‚   RSA(DE_V)
#define N3_L_SQ        RSA(DE_B)       // ‘   RSA(DE_B)
#define N3_R_SQ        RSA(DE_N)       // ’   RSA(DE_N)
//      ___NO__
//      ___NO__

// NEO2 Layer 3 - Line 2
#define N3_ELPS        RALT(DE_DOT)    // …    RALT(DE_DOT)
#define N3_UNDS        DE_UNDS         // _
#define N3_LBRC        DE_LBRC         // [
#define N3_RBRC        DE_RBRC         // ]
#define N3_CIRC        DE_CIRC         // ^
#define N3_EXLM        DE_EXLM         // !
#define N3_LABK        DE_LABK         // <
#define N3_RABK        DE_RABK         // >
#define N3_EQUL        DE_EQL          // =
#define N3_AMPR        DE_AMPR         // &
#define N3_SL_S        RALT(DE_S)      // ſ   RALT(DE_S)

// NEO2 Layer 3 - Line 3
#define N3_BSLS         DE_BSLS         // (backslash)
#define N3_SLSH         DE_SLSH         // /
#define N3_CLBR         DE_LCBR         // {
#define N3_CRBR         DE_RCBR         // }
#define N3_ASTR         DE_ASTR         // *
#define N3_QUES         DE_QUES         // ?
#define N3_LPRN         DE_LPRN         // (
#define N3_RPRN         DE_RPRN         // )
#define N3_MINS         DE_MINS         // -
#define N3_COLN         DE_COLN         // :
#define N3___AT         DE_AT           // @

// NEO2 Layer 3 - Line 4
#define N3_HASH         DE_HASH         // #
#define N3__DLR         DE_DLR          // $
#define N3_PIPE         DE_PIPE         // |
#define N3_TILD         DE_TILD         // ~
#define N3__GRV         DE_GRV          // `
#define N3_PLUS         DE_PLUS         // +
#define N3_PERC         DE_PERC         // %
#define N3_DQUO         DE_DQUO         // "
#define N3_QUOT         DE_QUOT         // '
#define N3_SCLN         DE_SCLN         // ;


// NEO2 Layer 4 - Line 1
#define N3_F_OR        RSA(DE_F)        // ª
#define N3_M_OR        RSA(DE_M)        // º
#define N3_NUMS        KC_NO            // №
//      ___NO__
#define N3_MDOT        RALT(DE_COMM)    // ·
#define N3_BPND        RSA(DE_3)        // £
#define N3_CURS        RSA(DE_4)        // ¤
#define N4__TAB        KC_TAB           // TAB
//      N3_SLSH                         // /
#define N3_ASTR        DE_ASTR          // *
#define N3_MDSH        RSA(DE_MINS)     // —   RSA(DE_MINS)
#define N3_NMNS        KC_PMNS          // Keypad -
//      ___NO__

// NEO2 Layer 4 - Line 2
#define N3_PGUP        KC_PGUP
#define N3_BSPC        KC_BSPC
#define N3___UP        KC_UP
#define N3__DEL        KC_DEL
#define N3_PGDN        KC_PGDN
#define N3_IEXL        RSA(DE_1)       // ¡   RSA(DE_1)
#define N3_NUM7        KC_P7           // Keypad 7
#define N3_NUM8        KC_P8           // Keypad 8
#define N3_NUM9        KC_P9           // Keypad 9
#define N3_NPLS        KC_PPLS         // Keypad +
#define N3_DMNS        DE_MINS         // - (not the original Neo − on this position)
//      ___NO__

// NEO2 Layer 4 - Line 3
#define N3_HOME        KC_HOME
#define N3_LEFT        KC_LEFT
#define N3_DOWN        KC_DOWN
#define N3_RGHT        KC_RGHT
#define N3__END        KC_END
#define N3_IQES        RSA(DE_SS)      // ¿   RSA(DE_SS)
#define N3_NUM4        KC_P4           // Keypad 4
#define N3_NUM5        KC_P5           // Keypad 5
#define N3_NUM6        KC_P6           // Keypad 6
#define N3_COMM        KC_COMM         // ,
#define N3__DOT        KC_DOT          // .

// NEO2 Layer 4 - Line 4
//      _______
#define N3__ESC        KC_ESC
#define N3__TAB        KC_TAB
#define N3__INS        KC_INS
#define N3__ENT        KC_ENT
#define N3_UNDO        LCTL(DE_Z)
#define N3_COLN        DE_COLN
#define N3_NUM1        KC_P1           // Keypad 1
#define N3_NUM2        KC_P2           // Keypad 2
#define N3_NUM3        KC_P3           // Keypad 3
//      N3_SCLN

// NEO2 Layer 4 - Line 5
#define N3_NUM0        KC_P0           // Keypad 0




// NEO_2 special characters



#define N3_C_UE     S(DE_UE)        // Ü
#define N3_C_OE     S(DE_OE)        // Ö
#define N3_C_AE     S(DE_AE)        // Ä
#define N3_REDO     RCS(DE_Z)       // Shift + CTL + Z
#define NX_COPY     LCTL(DE_C)      // CTL + C
#define NX_PAST     LCTL(DE_V)      // CTL + V
#define NX__CUT     LCTL(DE_X)      // CTL + X
#define NX_UTAB     LSFT(KC_TAB)    // Shift + TAB
#define NX_CENT     LCTL(KC_ENT)    // CTL + ENTER
#define NX_FIND     LCTL(KC_F)      // CTL + F
