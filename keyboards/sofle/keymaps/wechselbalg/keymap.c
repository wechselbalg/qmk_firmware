
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
#include "wechselbalg.h"

#include QMK_KEYBOARD_H

#define INDICATOR_BRIGHTNESS 20

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

// #define SFT_NUB MT(MOD_LSFT, KC_NUBS)
// #define ALT_GUI MT(MOD_LALT, KC_LGUI)
// #define SFT_SPC MT(MOD_LSFT, KC_SPC)
// #define CK_LCTNU MT(MOD_LCTL, KC_NUHS)
// #define CK_RCTNU MT(MOD_RCTL, KC_QUOT)

// // SYMBOL-LAYER (Neo Layer 3)
// #define CK_YSYM LT(_SYM, KC_Y)
// #define CK_ZSYM LT(_SYM, KC_Z)
// #define CK_FSYM LT(_SYM, KC_F)
// #define CK_SSYM LT(_SYM, KC_MINS)
// #define SYM_DOT LT(_SYM, KC_DOT)
// #define SYM_AT LT(_SYM, ALGR(KC_Q))
// #define SYM_AE LT(_SYM, KC_AE)

// // NUMLOCK/NAV-LAYER (Neo Layer 4)
// #define NUM_UE LT(_NUM, KC_UE)
// #define NUM_Z LT(_NUM, KC_Z)
// #define NUM_Y LT(_NUM, KC_Y)
// #define NUM_X LT(_NUM, KC_X)
// #define NUM_ESC LT(_NUM, KC_ESC)
// #define SFT_ENT MT(MOD_RSFT, KC_ENT)
// #define CK_RALT MT(MOD_RALT, KC_APP)
// #define SFT_MIN MT(MOD_RSFT, KC_MINS)
// #define CK_ALF4 LALT(KC_F4)
// #define KC_AE KC_QUOT
// #define KC_OE KC_SCLN
// #define KC_UE KC_LBRC
// #define CK_AT ALGR(KC_Q)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

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
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_QWERT] = LAYOUT_wrapper(
  //,------------------------------------------------------.      ,-----------------------------------------------------.
      TD_ESCC, ____________5_F_L_________________,                          _________________5_F_R____________, TD_SF11,
      _________________QWERTY_L1_________________,                          _________________QWERTY_R1_________________,
      _________________QWERTY_L2_________________,                          _________________QWERTY_R2_________________,
      _________________QWERTY_L3_________________, KC_MUTE,        KC_MPLY, _________________QWERTY_R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
),

/*
 * DVORAK
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_DVORAK] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      TD_ESCC, ____________5_F_L_________________,                          _________________5_F_R____________, TD_SF11,
      _________________DVORAK_L1_________________,                          _________________DVORAK_R1_________________,
      _________________DVORAK_L2_________________,                          _________________DVORAK_R2_________________,
      _________________DVORAK_L3_________________, KC_MUTE,        KC_MPLY, _________________DVORAK_R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
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
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_COLEMAKDH] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      TD_ESCC, ____________5_F_L_________________,                          _________________5_F_R____________, TD_SF11,
      ______________COLEMAKDH_L1_________________,                          ______________COLEMAKDH_R1_________________,
      ______________COLEMAKDH_L2_________________,                          ______________COLEMAKDH_R2_________________,
      ______________COLEMAKDH_L3_________________, KC_MUTE,        KC_MPLY, ______________COLEMAKDH_R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
),

/*
 * MINE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |PAUSE|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_MINE] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      TD_ESCC, ____________5_F_L_________________,                          _________________5_F_R____________, TD_SF11,
      __________________MINE__L1_________________,                          __________________MINE__R1_________________,
      __________________MINE__L2_________________,                          __________________MINE__R2_________________,
      __________________MINE__L3_________________, KC_MUTE,        KC_MPLY, __________________MINE__R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
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
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_VOUX] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      TD_ESCC, ____________5_F_L_________________,                          _________________5_F_R____________, TD_SF11,
      __________________VOUX__L1_________________,                          __________________VOUX__R1_________________,
      __________________VOUX__L2_________________,                          __________________VOUX__R2_________________,
      __________________VOUX__L3_________________, KC_MUTE,        KC_MPLY, __________________VOUX__R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
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
 *            | Bspc | WIN  |SYMBOL | Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_SYM] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      _________________SYMBOL_L0_________________,                          _________________SYMBOL_R0_________________,
      _________________SYMBOL_L1_________________,                          _________________SYMBOL_R1_________________,
      _________________SYMBOL_L2_________________,                          _________________SYMBOL_R2_________________,
      _________________SYMBOL_L3_________________, KC_MUTE,        KC_MPLY, _________________SYMBOL_R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
),
/* NUM (Numpad layer - NEO layer 4)
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |SYMBOL | /Space  /       \Enter \  |NUM | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NUM] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      _________________NUMBER_L0_________________,                          _________________NUMBER_R0_________________,
      _________________NUMBER_L1_________________,                          _________________NUMBER_R1_________________,
      _________________NUMBER_L2_________________,                          _________________NUMBER_R2_________________,
      _________________NUMBER_L3_________________, KC_MUTE,        F_LLOCK, _________________NUMBER_R3_________________,
                      ____________5_THUMBS_L______________,        _________5_NUMBER__THUMBS_R_________
  //                 \-------------------------------------|      |------------------------------------/
),

/* NAV (Navigation layer - NEO layer 4)
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |SYMBOL | /Space  /       \Enter \  |NUM | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NAV] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      _____________NAVIGATION_L0_________________,                          _____________NAVIGATION_R0_________________,
      _____________NAVIGATION_L1_________________,                          _____________NAVIGATION_R1_________________,
      _____________NAVIGATION_L2_________________,                          _____________NAVIGATION_R2_________________,
      _____________NAVIGATION_L3_________________, KC_MUTE,        KC_MPLY, _____________NAVIGATION_R3_________________,
                      ____________5_THUMBS_L______________,        ______5_NAVIGATION_THUMBS_R_________
  //                 \-------------------------------------|      |------------------------------------/
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
[_GAMING] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      TD_ESCC, _________5_NUMBERS_L______________,                          _______________5_NUMBERS_R________, FN_EXIT,
      _________________GAMING_L1_________________,                          _________________GAMING_R1_________________,
      _________________GAMING_L2_________________,                          _________________GAMING_R2_________________,
      _________________GAMING_L3_________________, KC_MUTE,        KC_MPLY, _________________GAMING_R3_________________,
                      _________5_GAMING_THUMBS_L__________,        _________5_GAMING_THUMBS_R__________
  //                 \-------------------------------------|      |------------------------------------/
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
 *            | LGUI | LAlt | LCTR |SYMBOL | /Space  /       \Enter \  |NUM | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT_wrapper(
    //,-----------------------------------------------------.     ,-----------------------------------------------------.
      _________________ADJUST_L0_________________,                          _________________ADJUST_R0_________________,
      _________________ADJUST_L1_________________,                          _________________ADJUST_R1_________________,
      _________________ADJUST_L2_________________,                          _________________ADJUST_R2_________________,
      _________________ADJUST_L3_________________, KC_MUTE,        KC_MPLY, _________________ADJUST_R3_________________,
                      ____________5_THUMBS_L______________,        ____________5_THUMBS_R______________
  //                 \-------------------------------------|      |------------------------------------/
  ),
};

#ifdef RGBLIGHT_ENABLE
char layer_state_str[70];
// Now define the array of layers. Later layers take precedence

// QWERT,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_qwert_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_ORANGE)

);
const rgblight_segment_t PROGMEM layer_colemakdh_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_PINK)
);

// _NUM,
// Light on outer column and underglow
const rgblight_segment_t PROGMEM layer_NUM_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_TEAL)

);
// _SYM,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_SYM_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_BLUE)

    );
// _NAV,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_NAV_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_PURPLE)
);

//_GAMING
const rgblight_segment_t PROGMEM layer_gaming_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_INDICATORS(HSV_RED),
    SET_UNDERGLOW(HSV_RED),
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
	layer_NUM_lights,// overrides layer 1
	layer_SYM_lights,
    layer_NAV_lights,
	layer_gaming_lights,
	layer_switcher_lights,  // Overrides other layers
	layer_colemakdh_lights
);

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _SYM, _NUM, _ADJUST);
	rgblight_set_layer_state(0, layer_state_cmp(state, _DEFAULTS) && layer_state_cmp(default_layer_state,_QWERT));
	rgblight_set_layer_state(3, layer_state_cmp(state, _DEFAULTS) && layer_state_cmp(default_layer_state,_COLEMAKDH));


	rgblight_set_layer_state(1, layer_state_cmp(state, _SYM));
	rgblight_set_layer_state(2, layer_state_cmp(state, _NUM));

	rgblight_set_layer_state(4, layer_state_cmp(state, _GAMING));
	rgblight_set_layer_state(5, layer_state_cmp(state, _NAV));
    rgblight_set_layer_state(6, layer_state_cmp(state, _ADJUST));

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
    oled_write_ln_P(PSTR("Sofle\nCHOC"), false);

    oled_write_ln_P(PSTR(""), false);

	//snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state)

    oled_write_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state  | default_layer_state)) {
        case _QWERT:
             oled_write_P(PSTR("QWERT\n"), false);
            break;
        case _DVORAK:
            oled_write_P(PSTR("DVORK\n"), false);
            break;
        case _COLEMAKDH:
            oled_write_P(PSTR("COLE\n"), false);
            break;
        case _GAMING:
            oled_write_P(PSTR("GAME\n"), false);
            break;
        case _MINE:
            oled_write_P(PSTR("MINE\n"), false);
            break;
        case _VOUX:
            oled_write_P(PSTR("VOUX\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("NUM \n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("NAV \n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("SYMB\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adj \n"), false);
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
        case QWERT:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERT);
            }
            return false;
        case DVORAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DVORAK);
            }
            return false;
        case COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAKDH);
            }
            return false;
        case MINE:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_MINE);
            }
            return false;
        case VOUX:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_VOUX);
            }
            return false;
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
			case _NUM:
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

void keyboard_post_init(void) {
    set_tri_layer_layers(_NUM, _NAV, _ADJUST);
}
