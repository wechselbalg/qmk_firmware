/* Copyright 2021 TweetyDaBird
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

enum layers {
    _QWERT,
    _NEO,
    _KOY,
    _VOU,
    _COLEMAK,
    _SYMBOL,
    _NUMB,
    _GAMING,
    _ADJUST,
};

enum custom_keycodes {
    KC_QWERT = SAFE_RANGE,
    KC_NEO,
    KC_KOY,
    KC_VOU,
    KC_COLEMAK,
    KC_SYMBOL,
    KC_NUMB,
    KC_GAMING,
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
    [ESC_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_GRV),
    [APP_END] = ACTION_TAP_DANCE_DOUBLE(KC_APP, KC_END),
    [GUI_HOM] = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_HOME)
};

#define CK_ESC TD(ESC_PIPE)
#define CK_LSFT MT(MOD_LSFT, KC_NUBS)
#define CK_LALT MT(MOD_LALT, KC_LGUI)
#define CK_LSPC MT(MOD_LSFT, KC_SPC)
#define CK_LCTNU MT(MOD_LCTL, KC_NUHS)
#define CK_RCTNU MT(MOD_RCTL, KC_QUOT)
#define CK_YSYM LT(_SYMBOL, KC_Y)
#define CK_ZSYM LT(_SYMBOL, KC_Z)
#define CK_FSYM LT(_SYMBOL, KC_F)
#define CK_SSYM LT(_SYMBOL, KC_MINS)
#define CK_AESYM LT(_SYMBOL, KC_AE)
#define CK_UENUM LT(_NUMB, KC_UE)
#define CK_ZNUM LT(_NUMB, KC_Z)
#define CK_YNUM LT(_NUMB, KC_Y)
#define CK_XNUM LT(_NUMB, KC_X)
#define CK_DELINS TD(DEL_INS)
#define CK_RENT MT(MOD_RSFT, KC_ENT)
#define CK_RALT MT(MOD_RALT, KC_APP)
#define CK_RSFT MT(MOD_RSFT, KC_NUHS)
#define CK_ALF4 LALT(KC_F4)
#define KC_AE KC_QUOT
#define KC_OE KC_SCLN
#define KC_UE KC_LBRC

/* Keymap for reference
 * ,-----------------------------------------. ______               ,------------------------------------------.
 * | Esc  |   1  |   2  |   3  |   4  |   5  ||      |        ___   |   6  |   7  |   8  |   9  |   0  |   ß   |
 * |------+------+------+------+------+------|| Disp |       /   \  |------+------+------+------+------+-------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  ||      |      ( ENC ) |   Y  |   U  |   I  |   O  |   P  |   Ü   |
 * |------+------+------+------+------+------||______|       \___/  |------+------+------+------+------+-------|
 * | SYM  |   A  |   S  |   D  |   F  |   G  |                      |   H  |   J  |   K  |   L  |   Ö  | CTRLÄ |
 * |------+------+------+------+------+------|--------.    ,--------|------+------+------+------+------+-------|
 * | Shift| Z/NUM|   X  |   C  |   V  |   B  |GUI/HOME|    |Menu/END|   N  |   M  |   ,  |   .  |   -  | Shift |
 * `-----------------------------------------|--------/    \--------|------------------------------------------'
 *                    | Alt  | Del | Ctrl | / Space  /      \ Enter  \ |SYMBOL|BSPACE| AltGr|
 *                    |      | Ins |      |/ Shift  /        \ Shift  \|      |      |      |
 *                    `-------------------'--------'          '--------'--------------------'
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERT] = LAYOUT(
    CK_ESC,     KC_1,    KC_2,   KC_3,     KC_4,     KC_5,                                KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    KC_MINS,
    KC_TAB,     KC_Q,    KC_W,   KC_E,     KC_R,     KC_T,                                KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC,
    TT(_SYMBOL),KC_A,    KC_S,   KC_D,     KC_F,     KC_G,                   TD(MED_SCR), KC_H,    KC_J,    KC_K,    KC_L,   KC_OE,  CK_AESYM,
    CK_LSFT,    CK_ZNUM, KC_X,   KC_C,     KC_V,     KC_B,  TD(GUI_HOM),     TD(APP_END), KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, CK_RSFT,
                                CK_LALT, CK_DELINS, KC_LCTL,   CK_LSPC,        CK_RENT,   TT(_NUMB), KC_BSPC, KC_RALT
),

[_COLEMAK] = LAYOUT(
   _______, KC_1,   KC_2,   KC_3,     KC_4,    KC_5,                         KC_6, KC_7,    KC_8,    KC_9,   KC_0,    _______,
   _______, KC_Q,   KC_W,   KC_F,     KC_P,    KC_B,                         KC_J, KC_L,    KC_U,    KC_Y,   KC_COMM, _______,
   _______, KC_A,   KC_R,   KC_S,     KC_T,    KC_G,               _______,  KC_M, KC_N,    KC_E,    KC_I,   KC_O,    _______,
   _______, CK_ZNUM,KC_X,   KC_C,     KC_D,    KC_V, _______,      _______,  KC_K, KC_H,    KC_COMM, KC_DOT, KC_SLSH, _______,
                         _______,_______, _______, _______,        _______, _______, _______, _______
),

[_NEO] = LAYOUT(
    _______, KC_1,    KC_2,    KC_3,     KC_4, KC_5,                        KC_6, KC_7,  KC_8,    KC_9,   KC_0, _______,
    _______, KC_X,    KC_V,    KC_L,     KC_C, KC_W,                        KC_K, KC_H,  KC_G,    KC_F,   KC_Q, KC_MINS,
    _______, KC_U,    KC_I,    KC_A,     KC_E, KC_O,              _______,  KC_S, KC_N,  KC_R,    KC_T,   KC_D, CK_ZSYM,
    _______, CK_UENUM,KC_OE,   KC_AE,    KC_P, KC_Z, _______,     _______,  KC_B, KC_M,  KC_COMM, KC_DOT, KC_J, CK_RSFT,
                            _______,_______, _______, _______,    _______, _______, _______, _______
),

[_KOY] = LAYOUT(
    _______, KC_1,    KC_2,   KC_3,  KC_4,    KC_5,                         KC_6, KC_7,    KC_8,    KC_9,  KC_0,    KC_MINS,
    _______, KC_K,    KC_DOT, KC_O,  KC_COMM, KC_Y,                         KC_V, KC_G,    KC_C,    KC_L,  KC_MINS, KC_Z,
    _______, KC_H,    KC_A,   KC_E,  KC_I,    KC_U,                _______, KC_D, KC_T,    KC_R,    KC_N,  KC_S,    CK_FSYM,
    _______, CK_XNUM, KC_Q,   KC_AE, KC_UE,   KC_OE, _______,      _______, KC_B, KC_P,    KC_W,    KC_M,  KC_J,    CK_RSFT,
                           _______,_______, _______, _______,      _______, _______, _______, _______
),

[_VOU] = LAYOUT(
    _______, KC_1,    KC_2,   KC_3,    KC_4,   KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,  KC_0,   KC_SLSH,
    _______, KC_V,    KC_DOT, KC_O,    KC_U,   KC_AE,                        KC_Q,    KC_G,    KC_L,    KC_H,  KC_F,   KC_J,
    _______, KC_C,    KC_A,   KC_E,    KC_I,   KC_Z,                _______, KC_D,    KC_T,    KC_R,    KC_N,  KC_S,   CK_SSYM ,
    _______, CK_YNUM, KC_X,   KC_COMM, KC_UE,  KC_OE,   _______,    _______, KC_P,    KC_D,    KC_W,    KC_M,  KC_K,   CK_RSFT,
                              _______,_______, _______, _______,    _______, _______, _______, _______
),

[_SYMBOL] = LAYOUT(
  _______,   KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,                                    KC_F6,         KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_F11,
  _______, LSFT(KC_3),   LSFT(KC_SLSH),ALGR(KC_8),ALGR(KC_9),KC_GRV,                          LSFT(KC_1),    KC_NUBS, LSFT(KC_NUBS), LSFT(KC_0), LSFT(KC_6), KC_F12,
  _______, ALGR(KC_MINS),LSFT(KC_7),ALGR(KC_7),ALGR(KC_0), LSFT(KC_RBRC),             _______,LSFT(KC_MINS), LSFT(KC_8), LSFT(KC_9), KC_SLSH, LSFT(KC_DOT), ALGR(KC_Q),
  _______, KC_NUHS,LSFT(KC_4),ALGR(KC_NUBS), ALGR(KC_RBRC), ALGR(KC_E),_______,       _______,KC_RBRC,       LSFT(KC_5),LSFT(KC_2), LSFT(KC_NUHS), LSFT(KC_COMM), _______,
                                           _______, _______, _______, _______,        _______, _______, _______, _______
),

[_NUMB] = LAYOUT(
  CK_ALF4, KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,                              KC_CIRC,      KC_TAB,  KC_PSLS, KC_PAST, KC_PMNS, KC_PSLS,
  _______, KC_PGUP, KC_BSPC, KC_UP,   KC_DEL,   KC_PGDN,                            KC_HASH,      KC_P7,   KC_P8,   KC_P9, KC_PPLS, KC_PAST,
  _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT, KC_END,                    _______, KC_AMPR,      KC_P4,   KC_P5,   KC_P6, KC_COMM, KC_DOT,
  KC_LALT, KC_ESC,  KC_TAB,  KC_INS,  KC_ENT,   LCTL(KC_Z), _______,       _______, LSFT(KC_DOT), KC_P1,  KC_P2,   KC_P3, LSFT(KC_COMM), _______,
                               _______, _______, _______,  _______,         _______, KC_P0,  KC_PDOT,  KC_BSPC
),

[_GAMING] = LAYOUT(
    _______, KC_1,  KC_2,   KC_3,     KC_4,    KC_5,                           KC_6, KC_7,    KC_8,    KC_9,   KC_0,    KC_MINS,
    _______, KC_T,  KC_Q,   KC_W,     KC_E,    KC_R,                           KC_Y, KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC,
    KC_G, KC_LSFT,  KC_A,   KC_S,     KC_D,    KC_F,                 _______,  KC_H, KC_J,    KC_K,    KC_L,   KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,  KC_X,   KC_B,     KC_V,    KC_C,  _______,       _______,  KC_N, KC_M,    KC_COMM, KC_DOT, KC_SLSH, CK_RSFT,
                            _______,  KC_LCTL, KC_SPC, KC_SPC,       _______,  _______, TT(_SYMBOL), _______
),

[_ADJUST] = LAYOUT(
  CK_ALF4 , KC_QWERT, XXXXXXX ,XXXXXXX ,CK_ALF4, DF(_KOY),                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT ,  DF(_QWERT),DF(_COLEMAK),DF(_NEO),DF(_VOU),DF(_GAMING),                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX,XXXXXXX,LCTL(KC_A),LCTL(KC_C),XXXXXXX,              XXXXXXX, XXXXXXX, LCTL(KC_V), XXXXXXX, RGUI(KC_L), XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                              _______, _______, _______, _______,       _______, _______, _______, _______ \
  )
};

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        // 'logo', 128x32px
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };
//     static const char PROGMEM lily58_logo[] = {
//     // 'logo', 128x32px
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x40, 0x40, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00,
//     0x80, 0xe0, 0x70, 0x3c, 0x0e, 0x06, 0x0e, 0x3c, 0x70, 0xe0, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0x00,
//     0xc0, 0xc0, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x80,
//     0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0xc0, 0x80, 0x80, 0x80, 0x81, 0x83, 0x83,
//     0x07, 0x07, 0x0c, 0x18, 0x70, 0xe0, 0x80, 0x00, 0x00, 0x01, 0xff, 0xfc, 0x80, 0xb6, 0xb6, 0x80,
//     0xb0, 0xb0, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf1, 0x00, 0x00, 0x00, 0x00, 0xff,
//     0xff, 0x00, 0x00, 0x00, 0x30, 0xf0, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xf0,
//     0x30, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xe1, 0x71, 0x71, 0xf1, 0xf1, 0xe1, 0xc1, 0x81, 0x00, 0x00,
//     0x00, 0x00, 0x0c, 0x3f, 0xff, 0xf3, 0xe1, 0xc1, 0xc1, 0x81, 0x81, 0xc3, 0xff, 0x7f, 0x1c, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x20, 0x70, 0x78, 0xdc, 0xcc, 0x86, 0x06, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//     0x01, 0x03, 0x02, 0x06, 0x84, 0xe1, 0xfb, 0x38, 0x1c, 0x0c, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
//     0x01, 0x01, 0x03, 0x03, 0x06, 0x86, 0xcc, 0xdc, 0x78, 0x70, 0x20, 0x00, 0xff, 0xff, 0x80, 0x80,
//     0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
//     0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1f, 0x7e, 0xf8, 0xe0, 0xf0, 0x7e, 0x1f, 0x03, 0x00,
//     0x00, 0x00, 0x00, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0xc0, 0xe1, 0xff, 0x7f, 0x3f, 0x00,
//     0x00, 0x00, 0x3e, 0xff, 0xff, 0xc1, 0xc0, 0x80, 0x81, 0x81, 0xc3, 0xc3, 0xff, 0xfe, 0x3c, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x06, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06,
//     0x06, 0x02, 0x03, 0x01, 0x01, 0x00, 0x01, 0x01, 0x03, 0x02, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04,
//     0x06, 0x06, 0x06, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
//     0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x01, 0x01, 0x01, 0x00, 0x00, 0x60, 0x60, 0x70, 0x38, 0x1f, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00
// };
    oled_write_raw_P(qmk_logo, sizeof(qmk_logo));
}


#    define KEYLOG_LEN 6
char     keylog_str[KEYLOG_LEN] = {};
uint8_t  keylogs_str_idx        = 0;
uint16_t log_timer              = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'z', 'y',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', '<', 'T', '_', '-', '=', 'U', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '-', ' ', ' ', ' '};

void add_keylog(uint16_t keycode) {
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        keycode = keycode & 0xFF;
    }

    for (uint8_t i = KEYLOG_LEN - 1; i > 0; i--) {
        keylog_str[i] = keylog_str[i - 1];
    }
    if (keycode < 60) {
        keylog_str[0] = code_to_name[keycode];
    }
    keylog_str[KEYLOG_LEN - 1] = 0;

    log_timer = timer_read();
}

void update_log(void) {
    if (timer_elapsed(log_timer) > 750) {
        add_keylog(0);
    }
}

void render_keylogger_status(void) {
    oled_write_P(PSTR("KLogr"), false);
    oled_write(keylog_str, false);
}

void render_layer_state(void) {
    oled_write_P(PSTR("Layer"), false);
    oled_write_P(PSTR(" "), false);
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case _QWERT:
            oled_write_P(PSTR("QWER"), false);
            break;
        case _NEO:
            oled_write_P(PSTR("NEO "), false);
            break;
        case _KOY:
            oled_write_P(PSTR("KOY "), false);
            break;
        case _VOU:
            oled_write_P(PSTR("VOU "), false);
            break;
        case _COLEMAK:
            oled_write_P(PSTR("COLE"), false);
            break;
        case _GAMING:
            oled_write_P(PSTR("GAME"), false);
            break;
        case _SYMBOL:
            oled_write_ln_P(PSTR("SYM "), false);
            break;
        case _NUMB:
            oled_write_P(PSTR("NUM "), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("ADJ "), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}

void render_keylock_status(led_t led_state) {
    oled_write_ln_P(PSTR("Lock"), false);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("N"), led_state.num_lock);
    oled_write_P(PSTR("C"), led_state.caps_lock);
    oled_write_ln_P(PSTR("S"), led_state.scroll_lock);
}

void render_mod_status(uint8_t modifiers) {
    oled_write_ln_P(PSTR("Mods"), false);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("S"), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR("C"), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR("A"), (modifiers & MOD_MASK_ALT));
    oled_write_P(PSTR("G"), (modifiers & MOD_MASK_GUI));
}

void render_status_main(void) {
    // Show keyboard layout
    render_layer_state();
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    // Show host keyboard led status
    render_keylock_status(host_keyboard_led_state());
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    // Show modifier status
    render_mod_status(get_mods());
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    render_keylogger_status();
}

bool oled_task_user(void) {
  update_log();
    render_status_main();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    return false;
}

#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        add_keylog(keycode);
    }

    switch (keycode) {
        case KC_QWERT:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERT);
            }
            return false;
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _SYMBOL, _NUMB, _ADJUST);
}

// Rotary encoder related code
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
 // if (index == 0) { // Encoder on master side
    if(IS_LAYER_ON(_NUMB)) { // on NUMB layer
      // Cursor control
      if (!clockwise) {
          tap_code(KC_MNXT);
      } else {
          tap_code(KC_MPRV);
      }
    }
    else {
      if (!clockwise) {
          tap_code(KC_VOLU);
      } else {
          tap_code(KC_VOLD);
      }
    }
    return true;
}
#endif
