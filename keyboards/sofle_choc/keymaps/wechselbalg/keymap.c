
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
#include "wechselbalg.h"
#include "sendstring_german.h"

#include QMK_KEYBOARD_H

#define INDICATOR_BRIGHTNESS 120

#define HSV_OVERRIDE_HELP(h, s, v, Override) h, s , Override
#define HSV_OVERRIDE(hsv, Override) HSV_OVERRIDE_HELP(hsv,Override)

// Light combinations
#define SET_INDICATORS(hsv) \
	{28, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}, \
    {29+0, 1, hsv}
#define SET_UNDERGLOW(hsv) \
	{1, 6, hsv}, \
    {29+1, 6,hsv}
#define SET_GAMING(hsv) \
	{8, 1, hsv},\
    {12, 2, hsv},\
    {18, 1, hsv}
#define SET_ADJUST(hsv) \
	{2, 1, hsv},\
    {8, 1, hsv},\
    {13, 1, hsv},\
    {18, 1, hsv},\
    {23, 1, hsv},\
    {26, 1, hsv}
#define SET_ADJUST_PERSIST(hsv) \
    {9, 1, hsv},\
    {12, 1, hsv},\
    {19, 1, hsv},\
    {22, 1, hsv},\
    {27, 1, hsv}
#define SET_QWERT(hsv) \
	{1, 1, hsv},\
    {9, 1, hsv},\
    {12, 1, hsv},\
    {19, 1, hsv},\
    {22, 1, hsv},\
    {29+ 1, 1, hsv}
#define SET_NUMPAD(hsv) \
	{29+7, 3, hsv}, \
	{29+12, 3, hsv}, \
	{29+17, 3, hsv}
#define SET_NUMROW(hsv) \
	{0, 1, hsv}, \
    {10, 2, hsv}, \
    {20, 2, hsv}, \
    {28, 1, hsv}, \
      {29+ 0, 1, hsv}, \
	  {29+ 10, 2, hsv}, \
	  {29+ 20, 2, hsv}, \
	  {29+ 28, 1, hsv}
#define SET_INNER_COL(hsv)	\
	{33, 4, hsv}, \
	  {29+ 33, 4, hsv}
#define SET_OUTER_COL(hsv) \
	{7, 4, hsv}, \
	  {29+ 7, 4, hsv}
#define SET_THUMB_CLUSTER(hsv) 	\
	{5, 3, hsv}, \
    {16, 2, hsv}, \
    {29+5, 3, hsv}, \
	{29+ 16, 2, hsv}
#define SET_LAYER_ID(hsv) 	\
	{0, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}, \
    {29+0, 1, HSV_OVERRIDE_HELP(hsv, INDICATOR_BRIGHTNESS)}, \
		{1, 3, hsv}, \
    {29+1, 3, hsv}, \
		{25, 4, hsv}, \
	  {29+ 25, 4, hsv}



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
 * |------+------+------+------+------+------| MUTE  |    |PAUSE  |------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL| Enter| /Space  /       \Enter \  |SPACE | NUM  | RCTR | RAlt |
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
 * |------+------+------+------+------+------|  MUTE |    |PAUSE  |------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL| Enter| /Space  /       \Enter \  |SPACE | NUM  | RCTR | RAlt |
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
 * |------+------+------+------+------+------|  MUTE |    |PAUSE  |------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL| Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
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
 * |------+------+------+------+------+------|  MUTE |    |PAUSE  |------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |LShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |SYMBOL| Enter| /Space  /       \Enter \  |SPACE |NUM | RCTR | RAlt |
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
 * VOU
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
[_VOU] = LAYOUT_wrapper(
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
      KC_ESC , _________5_NUMBERS_L______________,                          _______________5_NUMBERS_R________, FN_EXIT,
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
const rgblight_segment_t PROGMEM layer_default_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_QWERT(HSV_CHARTREUSE)
);
// _NUM,
// Light on outer column and underglow
const rgblight_segment_t PROGMEM layer_NUM_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_TEAL),
    SET_NUMPAD(HSV_MAGENTA)
);
// _SYM,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_SYM_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_BLUE)
);
// _NAV,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_NAV_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(HSV_CYAN),
  SET_GAMING(HSV_RED)
);
//_GAMING
const rgblight_segment_t PROGMEM layer_gaming_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_INDICATORS(HSV_PURPLE),
	SET_GAMING(HSV_RED)
);
//_ADJUST
const rgblight_segment_t PROGMEM layer_ADJUST_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_ADJUST(HSV_ORANGE),
	SET_ADJUST_PERSIST(HSV_RED)
);

// Light Layer (Order is important)
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(

    layer_default_lights,
    layer_SYM_lights,
	layer_NUM_lights,// overrides layer 1
    layer_NAV_lights,
	layer_gaming_lights,
    layer_ADJUST_lights
);

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _NAV, _NUM, _ADJUST);
	rgblight_set_layer_state(0, layer_state_cmp(state, _DEFAULTS)); //  && layer_state_cmp(default_layer_state,_QWERT))

	rgblight_set_layer_state(1, layer_state_cmp(state, _SYM));
	rgblight_set_layer_state(2, layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(3, layer_state_cmp(state, _NAV));

	rgblight_set_layer_state(4, layer_state_cmp(state, _GAMING));
    rgblight_set_layer_state(5, layer_state_cmp(state, _ADJUST));

    return state;
}
void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;

	rgblight_mode(10);// haven't found a way to set this in a more useful way

}

#else

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _NUM, _NAV, _ADJUST);
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
        case _VOU:
            oled_write_P(PSTR("VOU\n"), false);
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

// const custom_shift_key_t custom_shift_keys[] = {
//   {NAV_BSC, KC_DEL}, // Shift BACKSPACE is DELETE
// };
// uint8_t NUM_CUSTOM_SHIFT_KEYS =
//     sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_layer_lock(keycode, record, F_LLOCK)) { return false; }
    if (!process_caps_word(keycode, record)) { return false; }
    // if (!process_custom_shift_keys(keycode, record)) { return false; }

    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();

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
        case VOU:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_VOU);
            }
            return false;
        case FF_WORD:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT) SS_TAP(X_RIGHT) SS_TAP(X_LEFT)));
            }
            return false;
        case RV_WORD:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_RIGHT)));
            }
            return false;
        case DBRACES:  // Types [], {}, or <> and puts cursor between braces.
            if (record->event.pressed) {
            clear_oneshot_mods();  // Temporarily disable mods.
            unregister_mods(MOD_MASK_CSAG);
            if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                SEND_STRING("{}");
            } else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                SEND_STRING("<>");
            } else {
                SEND_STRING("[]");
            }
            tap_code(KC_LEFT);  // Move cursor between braces.
            register_mods(mods);  // Restore mods.
            }
            return false;
    }
    return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (get_highest_layer(layer_state)) {
            case _GAMING:
                if (clockwise) {
                    tap_code(KC_PGUP);
                } else {
                    tap_code(KC_PGDN);
                }
            break;
            case _NUM:
            case _NAV:
            case _SYM:
                    if (clockwise) {
                        tap_code16(N3_REDO);
                    } else {
                        tap_code16(N3_UNDO);
                    }
                break;
            default:
                    if (clockwise) {
                        tap_code(KC_PGUP);
                    } else {
                        tap_code(KC_PGDN);
                    }
                break;
		}
    } else if (index == 1) {
        switch (get_highest_layer(layer_state)) {
            case _GAMING:
                if (clockwise) {
                    tap_code(KC_UP);
                } else {
                    tap_code(KC_DOWN);
                }
            break;
        case _NUM:
        case _SYM:
        case _NAV:
                if (clockwise) {
                    tap_code(KC_TAB);
                } else {
                    tap_code16(NX_UTAB);
                }
            break;
        default:
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            break;
		}
    }
    return true;
}

#endif
