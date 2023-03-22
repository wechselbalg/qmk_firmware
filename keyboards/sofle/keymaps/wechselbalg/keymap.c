
 /* Copyright 2021 Dane Evans
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
  // SOFLE RGB
#include <stdio.h>

#include QMK_KEYBOARD_H

#define INDICATOR_BRIGHTNESS 30

#define HSV_OVERRIDE_HELP(h, s, v, Override) h, s , Override
#define HSV_OVERRIDE(hsv, Override) HSV_OVERRIDE_HELP(hsv,Override)

// Light combinations
#define SET_INDICATORS(hsv) \
	{0, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}, \
    {35+0, 1, hsv}
#define SET_UNDERGLOW(hsv) \
	{1, 6, hsv}, \
    {35+1, 6,hsv}
#define SET_GAMING(hsv)     \
	{35+15, 5, hsv},\
	  {35+22, 3, hsv},\
	  {35+27, 3, hsv}
#define SET_NUMROW(hsv) \
	{10, 2, hsv}, \
		{20, 2, hsv}, \
		{30, 2, hsv}, \
	  {35+ 10, 2, hsv}, \
	  {35+ 20, 2, hsv}, \
	  {35+ 30, 2, hsv}
#define SET_INNER_COL(hsv)	\
	{33, 4, hsv}, \
	  {35+ 33, 4, hsv}

#define SET_OUTER_COL(hsv) \
	{7, 4, hsv}, \
	  {35+ 7, 4, hsv}
#define SET_THUMB_CLUSTER(hsv) 	\
	{25, 2, hsv}, \
	  {35+ 25, 2, hsv}
#define SET_LAYER_ID(hsv) 	\
	{0, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}, \
    {35+0, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}, \
		{1, 6, hsv}, \
    {35+1, 6, hsv}, \
		{7, 4, hsv}, \
	  {35+ 7, 4, hsv}, \
		{25, 2, hsv}, \
	  {35+ 25, 2, hsv}


enum sofle_layers {
    _DEFAULTS = 0,
    _QWERT = 0,
    _NEO,
    _KOY,
    _VOUX,
    _COLEMAK,
	_COLEMAKDH,
    _SYM,
    _NUMP,
    _GAMING,
    _FUNC,
    _ADJUST,
};

enum custom_keycodes {
    KC_QWERT = SAFE_RANGE,
    KC_COLEMAK,
	KC_COLEMAKDH,
    KC_NEO,
    KC_KOY,
    KC_VOUX,
    KC_NUMP,
    KC_SYM,
    KC_GAMING,
    KC_FUNC,
    KC_ADJUST,
    KC_D_MUTE
};

// Tap Dance declarations
enum {
    MED_SCR,
    PG_UP,
    PG_DN,
    DEL_INS,
    ESC_PIPE,
    APP_END,
    GUI_HOM
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [MED_SCR] = ACTION_TAP_DANCE_DOUBLE(KC_MPLY, KC_MUTE),
    [PG_DN] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, KC_PGDN),
    [PG_UP] = ACTION_TAP_DANCE_DOUBLE(KC_UP, KC_PGUP),
    [DEL_INS] = ACTION_TAP_DANCE_DOUBLE(KC_DEL, KC_INS),
    //[ESC_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_GRV),
    [ESC_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, EE_CLR),
    [APP_END] = ACTION_TAP_DANCE_DOUBLE(KC_APP, KC_END),
    [GUI_HOM] = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_HOME)
};

#define CK_ESC TD(ESC_PIPE)
#define SFT_NUB MT(MOD_LSFT, KC_NUBS)
#define ALT_GUI MT(MOD_LALT, KC_LGUI)
#define SFT_SPC MT(MOD_LSFT, KC_SPC)
#define CK_LCTNU MT(MOD_LCTL, KC_NUHS)
#define CK_RCTNU MT(MOD_RCTL, KC_QUOT)

// SYMBOL-LAYER (Neo Layer 3)
#define CK_YSYM LT(_SYM, KC_Y)
#define CK_ZSYM LT(_SYM, KC_Z)
#define CK_FSYM LT(_SYM, KC_F)
#define CK_SSYM LT(_SYM, KC_MINS)
#define SYM_DOT LT(_SYM, KC_DOT)
#define SYM_AT LT(_SYM, ALGR(KC_Q))
#define SYM_AE LT(_SYM, KC_AE)
#define FUN_TAB LT(_FUNC,KC_TAB)
#define FUN_LBRC LT(_FUNC,KC_LBRC)
#define FUN_MINS LT(_FUNC,KC_MINS)
#define FUN_Z LT(_FUNC,KC_Z)
#define FUN_J LT(_FUNC,KC_J)

// NUMPLOCK/NAV-LAYER (Neo Layer 4)
#define NUM_UE LT(_NUMP, KC_UE)
#define NUM_Z LT(_NUMP, KC_Z)
#define NUM_Y LT(_NUMP, KC_Y)
#define NUM_X LT(_NUMP, KC_X)
#define NUM_ESC LT(_NUMP, KC_ESC)
#define CK_DELINS TD(DEL_INS)
#define SFT_ENT MT(MOD_RSFT, KC_ENT)
#define CK_RALT MT(MOD_RALT, KC_APP)
#define SFT_MIN MT(MOD_RSFT, KC_MINS)
#define CK_ALF4 LALT(KC_F4)
#define KC_AE KC_QUOT
#define KC_OE KC_SCLN
#define KC_UE KC_LBRC
#define CK_AT ALGR(KC_Q)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERT
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------| MUTE  |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_QWERT] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
      CK_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,                            KC_F6,     KC_F7,    KC_F8,    KC_F9,    KC_F10,  KC_MINS,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
      FUN_TAB,    KC_Q,    KC_W,   KC_E,    KC_R,    KC_T,                                 KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,   FUN_LBRC,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
      TT(_SYM),  KC_A,   KC_S,    KC_D,    KC_F,   KC_G,                                   KC_H,    KC_J,   KC_K,    KC_L,    KC_OE,   SYM_AE,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
      SFT_NUB,   NUM_Z,  KC_X,    KC_C,    KC_V,   KC_B,  KC_MUTE,                 KC_MPLY, KC_N,    KC_M,  KC_COMM, KC_DOT,  KC_SLSH,  KC_MINS,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                        KC_LGUI,   KC_LALT,   KC_DEL,  SFT_SPC,  KC_LCTL,       KC_RCTL,  SFT_ENT, KC_BSPC,  TT(_NUMP),KC_RALT
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/*
 * COLEMAK
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   G  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   D  |-------.    ,-------|   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   K  |   M  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_COLEMAK] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
  _______,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,     KC_7,   KC_8,   KC_9,    KC_0,    _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
  _______,  KC_Q,   KC_W,    KC_F,    KC_P,    KC_G,                      KC_J,    KC_L,   KC_U,    KC_Y,    KC_SCLN, _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
  _______,  KC_A,   KC_R,    KC_S,    KC_T,    KC_D,                      KC_H,    KC_N,   KC_E,    KC_I,    KC_O,    _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
  _______,  NUM_Z,   KC_X,    KC_C,    KC_V,    KC_B,  _______,    _______,KC_K,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH, _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                        _______,   _______,  _______,  _______,  _______,      _______,  _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/*
 * COLEMAK-DH
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_COLEMAKDH] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
  _______,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
  _______,  KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,                      KC_J,    KC_L,   KC_U,    KC_Y,    KC_SCLN, _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
  _______,  KC_A,   KC_R,    KC_S,    KC_T,    KC_G,                      KC_M,    KC_N,   KC_E,    KC_I,    KC_O,    _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
  _______,  NUM_Z,   KC_X,    KC_C,    KC_D,    KC_V,  _______,    _______,KC_K,    KC_H,   KC_COMM, KC_DOT, KC_SLSH, _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                        _______,   _______,  _______,  _______,  _______,      _______,  _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/*
 * NEO
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NEO] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    _______,   KC_1,     KC_2,     KC_3,     KC_4,      KC_5,                            KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______,   KC_X,     KC_V,     KC_L,     KC_C,      KC_W,                            KC_K, KC_H,  KC_G,    KC_F,   KC_Q, FUN_MINS,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______,   KC_U,     KC_I,     KC_A,     KC_E,      KC_O,                            KC_S, KC_N,  KC_R,    KC_T,   KC_D, CK_ZSYM,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    _______,   NUM_UE,   KC_OE,    KC_AE,    KC_P,      KC_Z,    _______,      _______,  KC_B, KC_M,  KC_COMM, KC_DOT, KC_J, _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                        _______,   _______,  _______,  _______,  _______,      _______,  _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/*
 * KOY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_KOY] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
  _______,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
  _______, KC_K,    KC_DOT, KC_O,  KC_COMM, KC_Y,                         KC_V,    KC_G,    KC_C,    KC_L,  KC_MINS, FUN_Z,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
  _______, KC_H,    KC_A,   KC_E,  KC_I,    KC_U,                         KC_D,    KC_T,    KC_R,    KC_N,  KC_S,    CK_FSYM,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
  _______, NUM_X,   KC_Q,   KC_AE, KC_UE,   KC_OE, _______,      _______, KC_B,    KC_P,    KC_W,    KC_M,  KC_J,    _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                        _______,   _______,  _______,  _______,  _______,      _______,  _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/*
 * VOUX
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_VOUX] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    _______,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,   KC_8,    KC_9,    KC_0,   KC_SLSH,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______, KC_V,    KC_DOT, KC_O,    KC_U,   KC_AE,                       KC_P,    KC_G,    KC_L,    KC_H,   KC_F,   FUN_J,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______, KC_C,    KC_A,   KC_E,    KC_I,   KC_Z,                        KC_B,    KC_T,    KC_R,    KC_N,   KC_S,   CK_SSYM,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    _______, NUM_Y, KC_X,   KC_COMM, KC_UE,  KC_OE, _______,      _______,  KC_Q,    KC_D,    KC_W,    KC_M,  KC_K,   _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                        _______,   _______,  _______,  _______,  _______,      _______,  _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/* SYMBOL (Symbo layer - NEO layer 3)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | trans|  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | trans|   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | trans|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUMP | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_SYM] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    _______,KC_1, ALGR(KC_2),ALGR(KC_2),LSFT(KC_NUBS),KC_NUBS,                          ALGR(KC_E), KC_F7, KC_COMM,LSFT(KC_NUHS),LSFT(KC_NUHS),  _______,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______, L%%
    _______,LSFT(KC_SLSH),ALGR(KC_8),ALGR(KC_9),KC_GRV,                     LSFT(KC_1),    KC_NUBS, LSFT(KC_NUBS), LSFT(KC_0), LSFT(KC_6), KC_F,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______, ALGR(KC_MINS),LSFT(KC_7),ALGR(KC_7),ALGR(KC_0),LSFT(KC_RBRC),              LSFT(KC_MINS), LSFT(KC_8), LSFT(KC_9), KC_SLSH, LSFT(KC_DOT), CK_AT,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    _______, KC_NUHS,LSFT(KC_4),ALGR(KC_NUBS),ALGR(KC_RBRC),ALGR(KC_E),_______, _______,KC_RBRC, LSFT(KC_5),LSFT(KC_2), LSFT(KC_NUHS), LSFT(KC_COMM), _______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                         _______,   _______,  _______,  _______,  _______,      _______,  _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),
/* NUMP (Numpad & Navigation layer - NEO layer 4)
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |SYMBOL | /Space  /       \Enter \  |NUMP | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NUMP] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    TG(_NUMP), _______,  LSFT(KC_GRV),_______, _______,_______,                          KC_CIRC, KC_TAB,  KC_PSLS, KC_PAST, KC_PMNS, KC_PSLS,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
     _______,  KC_PGUP,  KC_BSPC,  KC_UP,    KC_DEL,   KC_PGDN,                          KC_HASH, KC_P7,   KC_P8,   KC_P9, KC_PPLS, KC_PAST,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______,   KC_HOME,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_END,                           KC_AMPR,  KC_P4,   KC_P5,   KC_P6, KC_COMM, SYM_DOT,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    _______,   NUM_ESC,  KC_TAB,   KC_INS,   KC_ENT,   LCTL(KC_Z),_______,     _______,  LSFT(KC_DOT), KC_P1,KC_P2, KC_P3,LSFT(KC_COMM),_______,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                         _______,   _______,  _______,  _______,  _______,     _______,  _______,  KC_P0,    KC_PDOT,  KC_DOT
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/* FUNC (Function layer)
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |SYMBOL | /Space  /       \Enter \  |NUMP | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_FUNC] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    TG(_FUNC), KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,                            KC_F6,     KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______,   XXXXXXX,  XXXXXXX,  KC_EXEC,  XXXXXXX,  XXXXXXX,                          XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PSCR,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                          LCTL(KC_PSCR),LSFT(LCTL(KC_F)),ALGR(KC_PSCR),LSFT(KC_PSCR), XXXXXXX, XXXXXXX,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    _______,   XXXXXXX,   XXXXXXX, KC_CALC,  XXXXXXX,  XXXXXXX    ,_______,     _______,_______,   KC_MAIL,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                          _______, _______,  _______,  _______,  _______,       _______, _______,  _______,  _______,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/* GAMING
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | trans|      |      |      |      |      |                    |      |NumLck|      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |      |      |      |      |      |                    |   ^  |   7  |   8  |   9  |   *  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |-------.    ,-------|   -  |   4  |   5  |   6  |      |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |-------|    |-------|   +  |   1  |   2  |   3  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE | 0    |  .   | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_GAMING] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    KC_ESC,     KC_1,    KC_2,     KC_3,     KC_4,     KC_5,                             KC_6,      KC_7,    KC_8,     KC_9,     KC_0,     KC_MINS,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    _______,    KC_T,    KC_Q,     KC_W,     KC_E,     KC_R,                             KC_Y,      KC_U,    KC_I,     KC_O,     KC_P,     KC_LBRC,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    KC_G,       KC_LSFT, KC_A,     KC_S,     KC_D,     KC_F,                             KC_H,      KC_J,    KC_K,     KC_L,     KC_SCLN,  SYM_AE,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    KC_LSFT,    KC_Z,    KC_X,     KC_B,     KC_V,     KC_C,     _______,       _______, KC_N,      KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  SFT_MIN,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                          _______, _______,  KC_LCTL,  KC_SPC,   KC_SPC,        _______, TT(_NUMP),TT(_SYM),  KC_RALT,  _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),

/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | RESET|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |RGB_TOG|hue^ |sat ^ | bri ^|      |COLEMAK|-------.   ,-------|desk <|      |      |desk >|      |      |j
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | mode | hue dn|sat d|bri dn|      |QWERT|-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |SYMBOL | /Space  /       \Enter \  |NUMP | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  //,-----------------------------------------------------------.                       ,-----------------------------------------------------------.
    KC_QWERT,KC_COLEMAK,KC_COLEMAKDH,KC_NEO, KC_KOY,   KC_VOUX,                          XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  NK_TOGG,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    DF(_QWERT),DF(_COLEMAK),DF(_COLEMAKDH),DF(_NEO),DF(_KOY),DF(_VOUX),                  XXXXXXX,  LCTL(KC_V),XXXXXXX, RGUI(KC_L),XXXXXXX, XXXXXXX,
  //|---------+---------+---------+---------+---------+---------|                       |---------+---------+---------+---------+---------+---------|
    RGB_TOG,   RGB_HUI,  RGB_SAI,  RGB_VAI,   XXXXXXX,DF(_GAMING),                      C(G(KC_LEFT)),KC_NO, KC_NO,   C(G(KC_RGHT)),XXXXXXX, SYM_AE,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
    RGB_MOD,   RGB_HUD,  RGB_SAD,  RGB_VAD,   XXXXXXX, XXXXXXX,   EEP_RST,      RESET,   XXXXXXX,  KC_MPRV,  KC_MPLY,  KC_MNXT,  XXXXXXX,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------|  =====  |   |  =====  |---------+---------+---------+---------+---------+---------|
                         _______,  ALT_GUI,  CK_DELINS, TT(_SYM), SFT_SPC,     SFT_ENT,  TT(_NUMP), TT(_SYM), KC_RALT, _______
  //                    \---------+---------+---------+---------+---------|   |---------+---------+---------+---------+---------/
),
};

#ifdef RGBLIGHT_ENABLE
char layer_state_str[70];
// Now define the array of layers. Later layers take precedence

// QWERT,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_qwert_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_RED)

);
const rgblight_segment_t PROGMEM layer_colemakdh_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_PINK)
);

// _NUMP,
// Light on outer column and underglow
const rgblight_segment_t PROGMEM layer_NUMP_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_TEAL)

);
// _SYM,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_SYM_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_BLUE)

    );
// _COMMAND,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_command_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_PURPLE)
);

//_GAMING
const rgblight_segment_t PROGMEM layer_gaming_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_INDICATORS(HSV_ORANGE),
    SET_UNDERGLOW(HSV_ORANGE),
	SET_GAMING(HSV_BLUE),
    {7, 4, HSV_ORANGE},
    {25, 2, HSV_ORANGE},
    {35+6, 4, HSV_ORANGE},
    {35+25, 2, HSV_ORANGE}
    );
// _SWITCHER   // light up top row
const rgblight_segment_t PROGMEM layer_switcher_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_GREEN),
	SET_NUMROW(HSV_GREEN)
);

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(

    layer_qwert_lights,
	layer_NUMP_lights,// overrides layer 1
	layer_SYM_lights,
    layer_command_lights,
	layer_gaming_lights,
	layer_switcher_lights,  // Overrides other layers
	layer_colemakdh_lights
);

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _SYM, _NUMP, _ADJUST);
	rgblight_set_layer_state(0, layer_state_cmp(state, _DEFAULTS) && layer_state_cmp(default_layer_state,_QWERT));
	rgblight_set_layer_state(7, layer_state_cmp(state, _DEFAULTS) && layer_state_cmp(default_layer_state,_COLEMAKDH));


	rgblight_set_layer_state(1, layer_state_cmp(state, _SYM));
	rgblight_set_layer_state(2, layer_state_cmp(state, _NUMP));
	rgblight_set_layer_state(3, layer_state_cmp(state, _ADJUST));
	rgblight_set_layer_state(4, layer_state_cmp(state, _GAMING));
	rgblight_set_layer_state(5, layer_state_cmp(state, _FUNC));

    return state;
}
void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;

	rgblight_mode(10);// haven't found a way to set this in a more useful way

}
#endif

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("Dane\nEvans"), false);

    oled_write_ln_P(PSTR(""), false);

	//snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state)


    switch (get_highest_layer(default_layer_state)) {
        case _QWERT:
            oled_write_ln_P(PSTR("Qwrt"), false);
            break;
        case _COLEMAK:
            oled_write_ln_P(PSTR("Clmk"), false);
            break;
        case _COLEMAKDH:
            oled_write_ln_P(PSTR("CmkDH"), false);
            break;
        case _NEO:
            oled_write_P(PSTR("NEO "), false);
            break;
        case _KOY:
            oled_write_P(PSTR("KOY "), false);
            break;
        case _VOUX:
            oled_write_P(PSTR("VOUX"), false);
            break;
        case _GAMING:
            oled_write_P(PSTR("GAME"), false);
            break;


        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state  | default_layer_state)) {
        case _COLEMAK:
        case _QWERT:
        case _COLEMAKDH:
        case _GAMING:
        case _NEO:
        case _KOY:
        case _VOUX:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _NUMP:
            oled_write_P(PSTR("NUMP\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("SYMB\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adj\n"), false);
            break;
        case _FUNC:
            oled_write_P(PSTR("Func\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    return false;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_QWERT:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERT);
            }
            return false;
        case KC_COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK);
            }
            return false;
        case KC_COLEMAKDH:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAKDH);
            }
            return false;
        case KC_NEO:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_NEO);
            }
            return false;
        case KC_KOY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_KOY);
            }
            return false;
        case KC_VOUX:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_VOUX);
            }
            return false;
        case KC_SYM:
            if (record->event.pressed) {
                layer_on(_SYM);
                update_tri_layer(_SYM, _NUMP, _ADJUST);
            } else {
                layer_off(_SYM);
                update_tri_layer(_SYM, _NUMP, _ADJUST);
            }
            return false;
        case KC_NUMP:
            if (record->event.pressed) {
                layer_on(_NUMP);
                update_tri_layer(_SYM, _NUMP, _ADJUST);
            } else {
                layer_off(_NUMP);
                update_tri_layer(_SYM, _NUMP, _ADJUST);
            }
            return false;
        case KC_ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
        case KC_D_MUTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_MEH));
                register_code(KC_UP);
            } else {
                unregister_mods(mod_config(MOD_MEH));
                unregister_code(KC_UP);
            }
    }
    return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
		} else if (index == 1) {
			switch (get_highest_layer(layer_state)) {
				case _COLEMAK:
				case _QWERT:
				case _COLEMAKDH:
					if (clockwise) {
						tap_code(KC_PGDOWN);
					} else {
						tap_code(KC_PGUP);
					}
				break;
			case _NUMP:
			case _SYM:
					if (clockwise) {
						tap_code(KC_DOWN);
					} else {
						tap_code(KC_UP);
					}
				break;
			default:
					if (clockwise) {
						tap_code(KC_WH_D);
					} else {
						tap_code(KC_WH_U);
					}
				break;
		}
    }
    return true;
}

#endif
