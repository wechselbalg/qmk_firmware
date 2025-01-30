#pragma once
#include "wechselbalg.h"

/*
Since our quirky block definitions are basically a list of comma separated
arguments, we need a wrapper in order for these definitions to be
expanded before being used as arguments to the LAYOUT_xxx macro.
*/
#if (!defined(LAYOUT) && defined(KEYMAP))
#    define LAYOUT KEYMAP
#endif

// clang-format off

#define KEYMAP_wrapper(...)                  LAYOUT(__VA_ARGS__)
#define LAYOUT_wrapper(...)                  LAYOUT(__VA_ARGS__)
#define LAYOUT_ortho_5x14_wrapper(...)       LAYOUT_ortho_5x14(__VA_ARGS__)

/*
Blocks for each of the four major keyboard layouts
Organized so we can quickly adapt and modify all of them
at once, rather than for each keyboard, one at a time.
And this allows for much cleaner blocks in the keymaps.
For instance Tap/Hold for Control on all of the layouts

NOTE: These are all the same length.  If you do a search/replace
  then you need to add/remove underscores to keep the
  lengths consistent.
*/

// layout parts for easy reuse between keyboard keymaps
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  1  |  2  |  3  |  4  |  5  |   |  6  |  7  |  8  |  9  |  0  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________5_NUMBERS_L______________  KC_1, KC_2, KC_3, KC_4, KC_5         // TD_ESCC,
#define _______________5_NUMBERS_R________  KC_6, KC_7, KC_8, KC_9, KC_0         // ,DE_SS
#define _______________________________NUMBERS________________________________  _________5_NUMBERS_L______________, _______________5_NUMBERS_R________

// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// | F1  | F2  | F3  | F4  | F5  |   | F6  | F7  | F8  | F9  | F10 |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define ____________5_F_L_________________  KC_F1,   KC_F2, KC_F3, KC_F4, KC_F5     // TD_ESCC,
#define _________________5_F_R____________  TD_F612, KC_F7, KC_F8, KC_F9, KC_F10  // , TD_SF11

// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// | F1  | F2  | F3  | F4  | F5  |   | F6  | F7  | F8  | F9  | F10 |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define ___________________6_F_L___________________  KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6     // TD_ESCC,
#define ____________________6_F_R__________________  KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12  // , TD_SF11
#define _________________________________________F_KEYS_________________________________________ ___________________6_F_L___________________, ____________________6_F_R__________________
#define ________________________________________FN_KEYS_________________________________________ KC_BRID, KC_BRIU, KC_MCTL, KC_CPNL, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU

// THUMBKEYS
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// | F1  | F2  | F3  | F4  | F5  |   | F6  | F7  | F8  | F9  | F10 |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define ____________5_THUMBS_L______________  KC_LGUI, KC_LALT, CTL_ENT, SFT_SPC, NUM_DEL
#define ____________5_THUMBS_R______________  NAV_BSC, RFT_ENT, KC_RCTL, RALT_PR, KC_RGUI
#define ________________________________________7_THUMBS________________________________________  KC_LCTL, KC_LGUI, KC_LALT, NAV_SPC, MO__NAV, MO__NUM, KC_RCTL

// QWERT
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________QWERTY_L1_________________  NAV_TAB , DE_Q   , DE_W   , DE_E   , DE_R   , DE_T
#define _________________QWERTY_L2_________________  MO__SYM , DE_A   , DE_S   , DE_D   , DE_F   , DE_G
#define _________________QWERTY_L3_________________  SFT_PIP , NUM___Y, DE_X   , DE_C   , DE_V   , DE_B

#define _________________QWERTY_R1_________________  DE_Z    , DE_U   , DE_I   , DE_O   , DE_P   , NUM__UE
#define _________________QWERTY_R2_________________  DE_H    , DE_J   , DE_K   , DE_L   , DE___OE, SYM__AE
#define _________________QWERTY_R3_________________  DE_N    , DE_M   , DE_COMM, DE_DOT , DE_MINS, KC_RSFT

#define ________________________________________QWERTY_1________________________________________  _________________QWERTY_L1_________________, _________________QWERTY_R1_________________
#define ________________________________________QWERTY_2________________________________________  _________________QWERTY_L2_________________, _________________QWERTY_R2_________________
#define ________________________________________QWERTY_3________________________________________  KC_LSFT, MO__NUM, DE_Y, DE_X, DE_C, DE_V, DE_B, _________________QWERTY_R3_________________

// DVORAK
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________DVORAK_L1_________________  NAV_TAB ,KC_QUOTE, DE_COMM, DE__DOT, DE_P   , DE_Y
#define _________________DVORAK_L2_________________  MO__SYM , DE_A   , DE_O   , DE_E   , DE_U   , DE_I
#define _________________DVORAK_L3_________________  SFT_PIP , KC_SCLN, DE_Q   , DE_J   , DE_K   , DE_X

#define _________________DVORAK_R1_________________  DE_F    , DE_G   , DE_C   , DE_R   , DE_L   , MO__NUM
#define _________________DVORAK_R2_________________  DE_D    , DE_H   , DE_T   , DE_N   , DE_S   , SYM_HSH
#define _________________DVORAK_R3_________________  DE_B    , DE_M   , DE_W   , DE_V   , DE_Z   , RFT_MIN

// COLEMAK DH
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define ______________COLEMAKDH_L1_________________  NAV_TAB , DE_Q   , DE_W   , DE_F   , DE_P   , DE_B
#define ______________COLEMAKDH_L2_________________  MO__SYM , DE_A   , DE_R   , DE_S   , DE_T   , DE_G
#define ______________COLEMAKDH_L3_________________  SFT_PIP , DE_Z   , DE_X   , DE_C   , DE_D   , DE_V

#define ______________COLEMAKDH_R1_________________  DE_J    , DE_L   , DE_U   , DE_Y   , DE_SCLN  , MO__NUM
#define ______________COLEMAKDH_R2_________________  DE_H    , DE_N   , DE_E   , DE_I   , DE_O     , SYM_HSH
#define ______________COLEMAKDH_R3_________________  DE_K    , DE_H   , DE_COMM, DE_DOT , DE_MINS  , KC_RSFT

// MINE
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define __________________MINE__L1_________________  NAV_TAB , DE_J   , DE_L   , DE_U   , DE_A   , DE_Q
#define __________________MINE__L2_________________  MO__SYM , DE_C   , DE_R   , DE_I   , DE_E   , DE_O
#define __________________MINE__L3_________________  SFT_PIP , DE_V   , DE_X   , DE___UE, DE___AE, DE___OE

#define __________________MINE__R1_________________  DE_W    , DE_B   , DE_D   , DE_G   , DE_Y   , NUM__SS
#define __________________MINE__R2_________________  DE_M    , DE_N   , DE_T   , DE_S   , DE_H   , SYM___Z
#define __________________MINE__R3_________________  DE_P    , DE_F   , DE_COMM, DE_DOT , DE_K   , RFT_MIN

// VOUX
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define __________________VOUX__L1_________________  NAV_TAB , DE_V   , DE__DOT, DE_O   , DE_U   , DE___AE
#define __________________VOUX__L2_________________  MO__SYM , DE_C   , DE_A   , DE_E   , DE_I   , DE_Y
#define __________________VOUX__L3_________________  KC_LSFT , DE_Z   , DE_X   ,DE_COMM , DE___UE, DE___OE

#define __________________VOUX__R1_________________  DE_P    , DE_G   , DE_L   , DE_H   , DE_F   , NUM___J
#define __________________VOUX__R2_________________  DE_B    , DE_T   , DE_R   , DE_N   , DE_S   , SYM__SS
#define __________________VOUX__R3_________________  DE_Q    , DE_D   , DE_W   , DE_M   , DE_K   , RFT_MIN

// GAMING
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________GAMING_L1_________________  KC_TAB  , DE_Q   , DE_T   , DE_W   , DE_E   , DE_R
#define _________________GAMING_L2_________________  MO__SYM , KC_LSFT, DE_A   , DE_S   , DE_D   , DE_F
#define _________________GAMING_L3_________________  DE_Y,     DE_G   , DE_X   , DE_B   , DE_C   , DE_V

#define _________________GAMING_R1_________________  DE_Z    , DE_U   , DE_I   , DE_O   , DE_P   , NUM__UE
#define _________________GAMING_R2_________________  DE_H    , DE_J   , DE_K   , DE_L   , DE___OE, SYM__AE
#define _________________GAMING_R3_________________  DE_N    , DE_M   , DE_COMM, DE_DOT , DE_MINS, KC_RSFT

#define ________________________________________GAMING_2________________________________________  KC_ENT, DE_A, DE_S, DE_D, DE_F, DE_G,  _________________QWERTY_R2_________________
#define ________________________________________GAMING_3________________________________________  KC_LSFT, DE_LABK, DE_Y, DE_X, DE_C, DE_V, DE_B,  _________________QWERTY_R3_________________

#define _________5_GAMING_THUMBS_L__________  KC_LCTL , KC_LALT, KC_LCTL, KC_SPC , KC_ENT
#define _________5_GAMING_THUMBS_R__________  _______ , _______, MO__NAV, MO__NUM, MO__SYM
#define ____________________________________7_GAMING__THUMBS____________________________________  KC_LCTL, DE_M, KC_LALT, KC_SPC, MO__NAV, MO__NUM, KC_RCTL

// SYMBOL
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________SYMBOL_L0_________________  FN_EXIT , N3_SUP1, N3_SUP2, N3_SUP3, N3_RSAQ, N3_LSAQ
#define _________________SYMBOL_L1_________________  DE_GRV  , DE_DEG , N3_UNDS, N3_LBRC, N3_RBRC, N3_CIRC
#define _________________SYMBOL_L2_________________  DE_TILD , N3_BSLS, N3_SLSH, N3_CLBR, N3_CRBR, N3_ASTR
#define _________________SYMBOL_L3_________________  F_LLOCK , N3_HASH, N3__DLR, N3_PIPE, N3_TILD, N3__GRV

#define _________________SYMBOL_R0_________________  N3_CENT, N2_SECT, DBRACES, N3_L_SQ, N3_R_SQ, ___NO__
#define _________________SYMBOL_R1_________________  N3_EXLM, N3_LABK, N3_RABK, N3_EQUL, N3_AMPR, DE_PLUS
#define _________________SYMBOL_R2_________________  N3_QUES, N3_LPRN, N3_RPRN, N3_MINS, N3_COLN, N3___AT
#define _________________SYMBOL_R3_________________  N3_PLUS, N3_PERC, N3_DQUO, N3_QUOT, N3_SCLN, F_LLOCK

#define ________________________________________SYMBOL__0_______________________________________  _________________SYMBOL_L0_________________, _________________SYMBOL_R0_________________
#define ________________________________________SYMBOL__1_______________________________________  _________________SYMBOL_L1_________________, _________________SYMBOL_R1_________________
#define ________________________________________SYMBOL__2_______________________________________  _________________SYMBOL_L2_________________, _________________SYMBOL_R2_________________
#define ________________________________________SYMBOL__3_______________________________________  F_LLOCK, ___NO__, N3_HASH, N3__DLR, N3_PIPE, N3_TILD, N3__GRV, _________________SYMBOL_R3_________________

// NUMBER
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________NUMBER_L0_________________  FN_EXIT,  DE_DEG, DE_SUP2, DE_SUP3, ___NO__, N3_MDOT
#define _________________NUMBER_L1_________________  KC_NUM ,  KC_F13,  KC_F7 ,  KC_F8 ,  KC_F9 , NX__CUT
#define _________________NUMBER_L2_________________  KC_RALT,  KC_F12,  KC_F4 ,  KC_F5 ,  KC_F6 , NX_COPY
#define _________________NUMBER_L3_________________  F_LLOCK,  KC_F11,  KC_F1 ,  KC_F2 ,  KC_F3 , NX_PAST

#define _________________NUMBER_R0_________________  N4__TAB, N2_SECT, N3_SLSH, N3_ASTR, DE_HASH, N3_NMNS
#define _________________NUMBER_R1_________________  N2__EUR, N3_NUM7, N3_NUM8, N3_NUM9, N3_ASTR, N3_SLSH
#define _________________NUMBER_R2_________________  N3__DLR, N3_NUM4, N3_NUM5, N3_NUM6, N3_NPLS, N3_NMNS
#define _________________NUMBER_R3_________________  N3_COLN, N3_NUM1, N3_NUM2, N3_NUM3, N3_SCLN, N3_EQUL

#define _________5_NUMBER__THUMBS_R_________         _______, _______, NAV_NM0, N3_COMM, N3__DOT
#define ________________________________7_NUMBER__THUMBS________________________________________  KC_LCTL, KC_LGUI, KC_LALT, NAV_NM0, N3_COMM, N3__DOT, KC_RCTL

#define ________________________________________NUMBER__0_______________________________________  _________________NUMBER_L0_________________, _________________NUMBER_R0_________________
#define ________________________________________NUMBER__1_______________________________________  _________________NUMBER_L1_________________, _________________NUMBER_R1_________________
#define ________________________________________NUMBER__2_______________________________________  _________________NUMBER_L2_________________, _________________NUMBER_R2_________________
#define ________________________________________NUMBER__3_______________________________________  F_LLOCK, ___NO__, KC_F11, KC_F1, KC_F2, KC_F3, N3__ENT, _________________NUMBER_R3_________________

// NAVIGATION
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _____________NAVIGATION_L0_________________  FN_EXIT, NX_UTAB, RV_WORD, N3__TAB, FF_WORD, KC_INS
#define _____________NAVIGATION_L1_________________  N3__TAB, N3_PGUP, N3_BSPC, N3___UP, N3__DEL, N3_PGDN
#define _____________NAVIGATION_L2_________________  NX_UTAB, N3_HOME, N3_LEFT, N3_DOWN, N3_RGHT, N3__END
#define _____________NAVIGATION_L3_________________  F_LLOCK, N3__ESC, N3_UNDO, N3__TAB, N3_REDO, N3__ENT


#define _____________NAVIGATION_R0_________________  KC_TAB , ___NO__, ___NO__, ___NO__, ___NO__, KC_INS
#define _____________NAVIGATION_R1_________________  NX__CUT, NX_COPY, NX_PAST, NX_FIND, KC_PSCR, KC_APP
#define _____________NAVIGATION_R2_________________  SFT_CTL, KC_LCTL, KC_LSFT, KC_LALT, KC_RALT, KC_RGUI
#define _____________NAVIGATION_R3_________________  NX_CENT, KC_WH_D, KC_MS_U, KC_WH_U, ___NO__, F_LLOCK

#define ______5_NAVIGATION_THUMBS_R_________         KC_BTN2, KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R

#define ______________________________________NAVIGATION__0_____________________________________  _____________NAVIGATION_L0_________________, _____________NAVIGATION_R0_________________
#define ______________________________________NAVIGATION__1_____________________________________  _____________NAVIGATION_L1_________________, _____________NAVIGATION_R1_________________
#define ______________________________________NAVIGATION__2_____________________________________  _____________NAVIGATION_L2_________________, _____________NAVIGATION_R2_________________
#define ______________________________________NAVIGATION__3_____________________________________  F_LLOCK, KC_INS, N3__ESC, N3_UNDO, N3__TAB, N3_REDO, N3__ENT, _____________NAVIGATION_R3_________________
#define __________________________________7_NAVIGATION__THUMBS__________________________________  KC_LCTL, KC_LGUI, KC_LALT, NAV_NM0, N3_COMM, N3__DOT, KC_RCTL

// ADJUST
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________ADJUST_L0_________________  FN_EXIT, KC_BRID, KC_BRIU, ___NO__, ___NO__, RGB_VAD
#define _________________ADJUST_L1_________________  QWERT  , DVORAK , COLEMAK, VOU    , MINE   , ___NO__
#define _________________ADJUST_L2_________________  D_QWERT, D_DVORK, D_COLMK, D___VOU, D__MINE, D__GAME
#define _________________ADJUST_L3_________________  ___NO__, ___NO__, ___NO__, ___NO__, ___NO__, QK_BOOT

#define _________________ADJUST_R0_________________  RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, RGB_TOG, RGB_MOD
#define _________________ADJUST_R1_________________  ___NO__, ___NO__, ___NO__, ___NO__, ___NO__, A_MSJIG
#define _________________ADJUST_R2_________________  RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD, ___NO__
#define _________________ADJUST_R3_________________  NK_TOGG, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,CW_TOGG

#define ________________________________________ADJUST__0_______________________________________  _________________ADJUST_L0_________________, _________________ADJUST_R0_________________
#define ________________________________________ADJUST__1_______________________________________  _________________ADJUST_L1_________________, _________________ADJUST_R1_________________
#define ________________________________________ADJUST__2_______________________________________  _________________ADJUST_L2_________________, _________________ADJUST_R2_________________
#define ________________________________________ADJUST__3_______________________________________  ___NO__, ___NO__, ___NO__, ___NO__, ___NO__, ___NO__, QK_BOOT, _________________ADJUST_R3_________________

// clang-format on
