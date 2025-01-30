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

#include QMK_KEYBOARD_H
#include "wechselbalg.h"
#include "layer_lock.h"

// clang-format off
#define N_SY_HS        LT(SYM,DE_HASH) // SYM Layer Or Hash
#define N_SY_AC        LT(SYM,DE_ACUT) // SYM Layer Or Hash
#define N_SY_AE        LT(SYM,DE_AE)   // SYM Layer Or Hash
#define FN_EXIT        TO(QWERTY)      // Turns off all layers, except the default

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

[QWERTY] = LAYOUT_iso_85(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_CALC,  KC_MAIL,  RGB_MOD,
     DE_CIRC,  DE_1,     DE_2,     DE_3,     DE_4,     DE_5,     DE_6,     DE_7,     DE_8,     DE_9,     DE_0,     DE_SS,    DE_ACUT,  KC_BSPC,            KC_DEL,
     NAV_TAB,  DE_Q,     DE_W,     DE_E,     DE_R,     DE_T,     DE_Z,     DE_U,     DE_I,     DE_O,     DE_P,     NUM__UE,  DE_PLUS,  KC_ENT,             KC_HOME,
     MO__SYM,  DE_A,     DE_S,     DE_D,     DE_F,     DE_G,     DE_H,     DE_J,     DE_K,     DE_L,     DE_OE,    N_SY_AE,  N_SY_HS,                      KC_END,
     KC_LSFT,  SFT_PIP , NUM___Y,  DE_X,     DE_C,     DE_V,     DE_B,     DE_N,     DE_M,     DE_COMM,  DE_DOT,   DE_MINS,  KC_RSFT,            KC_UP,    MO(FN),
     KC_LCTL,  KC_LGUI,  KC_LALT,                                NAV_SPC,                                MO__NAV,  MO__NUM,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[SYM] = LAYOUT_iso_85(
     FN_EXIT,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  N3_SUP1,  N3_SUP2,  N3_SUP3,  N3_RSAQ,  N3_LSAQ,  N3_CENT,  N3__YEN,  N3_SBSQ,  N3_L_SQ,  N3_R_SQ,  DE_SECT,  DE_CIRC,  _______,            _______,
     _______,  KC_Q,     N3_UNDS,  N3_LBRC,  N3_RBRC,  N3_CIRC,  N3_EXLM,  N3_LABK,  N3_RABK,  N3_EQUL,  N3_AMPR,  DE_SECT,  DE_EURO,  _______,            _______,
     F_LLOCK,  N3_BSLS,  N3_SLSH,  N3_CLBR,  N3_CRBR,  N3_ASTR,  N3_QUES,  N3_LPRN,  N3_RPRN,  N3_MINS,  N3_COLN,  N3___AT,  F_LLOCK,                      _______,
     CW_TOGG,  _______,  N3_HASH,  N3__DLR,  N3_PIPE,  N3_TILD,  N3__GRV,  N3_PLUS,  N3_PERC,  N3_DQUO,  N3_QUOT,  N3_SCLN,  _______,            _______,  _______,
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[NUM] = LAYOUT_iso_85(
     FN_EXIT,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_NUM,  _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   KC_TAB,  N3_SLSH,  N3_ASTR,  N3_NMNS,  DE_COLN,  _______,            _______,
     _______,  N3_PGUP,  N3_BSPC,  N3___UP,  N3__DEL,  N3_PGDN,  N3_REDO,  N3_NUM7,  N3_NUM8,  N3_NUM9,  N3_NPLS,  N3_DMNS,  DE_EURO,  _______,            KC_WH_U,
     _______,  N3_HOME,  N3_LEFT,  N3_DOWN,  N3_RGHT,  N3__END,  DE_PERC,  N3_NUM4,  N3_NUM5,  N3_NUM6,  N3_COMM,  N3__DOT,  _______,                      KC_WH_D,
     _______,  F_LLOCK,  N3__ESC,  N3__TAB,  N3__INS,  N3__ENT,  N3_UNDO,  N3_COLN,  N3_NUM1,  N3_NUM2,  N3_NUM3,  N3_SCLN,  KC_BTN1,            KC_MS_U,  KC_BTN2,
     _______,  _______,  _______,                                N3_NUM0,                                F_LLOCK,  _______,  _______,  KC_MS_L,  KC_MS_D,  KC_MS_R),

[NAV] = LAYOUT_iso_85(
     FN_EXIT,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   KC_TAB,  N3_SLSH,  N3_ASTR,  N3_NMNS,  DE_COLN,  _______,            _______,
     _______,  N3_PGUP,  N3_BSPC,  N3___UP,  N3__DEL,  N3_PGDN,  N3_REDO,  _______,  _______,  _______,  NX_COPY,  NX_PAST,  NX__CUT,  _______,            KC_WH_U,
     _______,  N3_HOME,  N3_LEFT,  N3_DOWN,  N3_RGHT,  N3__END,  ___NO__,  KC_LCTL,  KC_LSFT,  KC_LCTL,  KC_LSFT,  KC_LALT,  KC_RALT,                      KC_WH_D,
     CW_TOGG,  F_LLOCK,  N3__ESC,  N3__TAB,  N3__INS,  N3__ENT,  N3_UNDO,  N3_COLN,  _______,  _______,  _______,  N3_SCLN,  KC_BTN1,            KC_MS_U,  KC_BTN2,
     _______,  _______,  _______,                                _______,                                F_LLOCK,  _______,  KC_BTN3,  KC_MS_L,  KC_MS_D,  KC_MS_R),

[FN] = LAYOUT_iso_85(
     QK_BOOT,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  RGB_MOD,  RGB_TOG,  RGB_MOD,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  KC_BRID,  KC_BRIU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      KC_SLEP,
     _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,  CW_TOGG,            _______,  _______,
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______)
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
