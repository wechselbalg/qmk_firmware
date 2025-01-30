/* Copyright 2023 @ Keychron (https://www.keychron.com)
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
#include "wechselbalg.h"
#include "sendstring_german.h"
#include "layer_lock.h"

#include QMK_KEYBOARD_H

// clang-format off

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MINE] = LAYOUT_iso_85(
        KC_ESC,   ___________________6_F_L___________________,     ____________________6_F_R__________________,   KC_CALC,  KC_MAIL,  RGB_TOG,
        DE_CIRC,  _________5_NUMBERS_L______________,     _______________5_NUMBERS_R________,     DE_MINS,  DE_GRV,   KC_BSPC,            KC_DEL,
        KC_TAB,   DE_J,     DE_L,     DE_U,     DE_A,     DE_W,     DE_B,     DE_B,     DE_D,     DE_G,     DE_Y,     DE_Z,     DE_SS,    KC_ENT,             KC_HOME,
        MO(SYM),  DE_C,     DE_R,     DE_I,     DE_E,     DE_O,     DE_M,     DE_N,     DE_T,     DE_S,     DE_H,     MO(SYM),  N_SY_AC,                      KC_END,
        KC_LSFT,  MO(NUM),  DE_V,     DE_X,     DE_UE,    DE_AE,    DE_OE,    DE_P,     DE_F,     DE_COMM,  DE_DOT,   DE_K,     KC_RSFT,            KC_UP,    MO(FN),
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(NAV),  KC_RALT,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [TEST] = LAYOUT_iso_85(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,  RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

    [QWERT] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________, RN_CODE,  KC_CALC,  RGB_MOD,
        KC_GRV, _______________________________NUMBERS________________________________, DE_SS,    DE_ACUT,           KC_BSPC,  KC_DEL,
        ________________________________________QWERTY_1________________________________________, KC_RBRC, KC_ENT,             KC_HOME,
        ________________________________________QWERTY_2________________________________________, N_SY_HS,                     KC_END,
        ________________________________________QWERTY_3________________________________________,          KC_UP,    MO(FN),
        ________________________________________7_THUMBS________________________________________, KC_LEFT, KC_DOWN,  KC_RGHT),

    [_SYM] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________, _______,  _______,  _______,
        ________________________________________SYMBOL__0_______________________________________, ___NO__,           _______,  _______,
        ________________________________________SYMBOL__1_______________________________________, N3_SLSH, _______,            _______,
        ________________________________________SYMBOL__2_______________________________________, _______,                     _______,
        ________________________________________SYMBOL__3_______________________________________,          _______,  _______,
        ________________________________________7_THUMBS________________________________________, _______, _______,  _______
    ),

    [_NUM] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,  KC_F13,  _______,  _______,
        ________________________________________NUMBER__0_______________________________________, ___NO__,           _______,  _______,
        ________________________________________NUMBER__1_______________________________________, N3_SLSH, _______,            _______,
        ________________________________________NUMBER__2_______________________________________, _______,                     _______,
        ________________________________________NUMBER__3_______________________________________           _______,  _______,
        ________________________________7_NUMBER__THUMBS________________________________________, _______, _______,  _______
    ),

    [_NAV] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,   KC_F13,  _______,  _______,
        ______________________________________NAVIGATION__0_____________________________________, ___NO__,            _______,  _______,
        ______________________________________NAVIGATION__1_____________________________________, N3_SLSH,  _______,            UC_NEXT,
        ______________________________________NAVIGATION__2_____________________________________, _______,                      UC_PREV,
        ______________________________________NAVIGATION__3_____________________________________,           KC_MS_U,  _______,
        __________________________________7_NAVIGATION__THUMBS__________________________________, KC_MS_L,  KC_MS_D,  KC_MS_R
    ),

    [_GAMING] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________,  _______,  _______,  _______,
        KC_GRV,  _______________________________NUMBERS________________________________, KC_MINS, _______,            _______,  _______,
        ________________________________________QWERTY_1________________________________________, _______,  _______,            _______,
        ________________________________________GAMING_2________________________________________, _______,                      _______,
        ________________________________________GAMING_3________________________________________,           _______,  _______,
        ____________________________________7_GAMING__THUMBS____________________________________, _______,  _______,  _______
    ),

    [_ADJUST] = LAYOUT_wrapper(
        QK_BOOT, ________________________________________FN_KEYS_________________________________________,  _______,  _______,  _______,
        ________________________________________ADJUST__0_______________________________________, _______,            QK_BOOT,  _______,
        ________________________________________ADJUST__1_______________________________________, _______,  _______,            _______,
        ________________________________________ADJUST__2_______________________________________, FN_EXIT,                      _______,
        ________________________________________ADJUST__3_______________________________________,           RGB_MOD,  _______,
        ________________________________________7_THUMBS________________________________________, RGB_SPD, RGB_RMOD,  RGB_SPI
    )
};

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case DE_AE:
        case DE_OE:
        case DE_UE:
        case DE_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case N3_NUM1 ... N3_NUM0:
        case DE_SS:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_layer_lock(keycode, record, F_LLOCK)) { return false; }

  return true;
}
