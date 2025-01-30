/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "wechselbalg.h"
#include "sendstring_german.h"

#include QMK_KEYBOARD_H

// #include "print.h"
// clang-format off

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     /        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right

    // [_BASE] = LAYOUT_wrapper(
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    //     _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
    // ),

    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backspace. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.

    [_QWERT] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________,  RN_CODE,  KC_MUTE,
        KC_GRV,  _______________________________NUMBERS________________________________, KC_MINS, KC_EQL ,  KC_BSPC,  KC_DEL,
        ________________________________________QWERTY_1________________________________________, KC_RBRC,            KC_HOME,
        ________________________________________QWERTY_2________________________________________, SYM_HSH,  KC_ENT,   KC_END,
        ________________________________________QWERTY_3________________________________________,           KC_UP,    MO__ADJ,
        ________________________________________7_THUMBS________________________________________, KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    [_SYM] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________, _______,  _______,
        ________________________________________SYMBOL__0_______________________________________, ___NO__, _______,  _______,
        ________________________________________SYMBOL__1_______________________________________, N3_SLSH,           _______,
        ________________________________________SYMBOL__2_______________________________________, _______, _______,  _______,
        ________________________________________SYMBOL__3_______________________________________,          _______,  _______,
        ________________________________________7_THUMBS________________________________________, _______, _______,  _______
    ),

    [_NUM] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,  KC_F13,   _______,
        ________________________________________NUMBER__0_______________________________________, ___NO__,  _______,  _______,
        ________________________________________NUMBER__1_______________________________________, N3_SLSH,            _______,
        ________________________________________NUMBER__2_______________________________________, _______,  _______,  _______,
        ________________________________________NUMBER__3_______________________________________            _______,  _______,
        ________________________________7_NUMBER__THUMBS________________________________________, _______,  _______,  _______
    ),

    [_NAV] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,   KC_F13,  _______,
        ______________________________________NAVIGATION__0_____________________________________, ___NO__,  _______,  _______,
        ______________________________________NAVIGATION__1_____________________________________, N3_SLSH,            UC_NEXT,
        ______________________________________NAVIGATION__2_____________________________________, _______,  _______,  UC_PREV,
        ______________________________________NAVIGATION__3_____________________________________,           KC_MS_U,  _______,
        __________________________________7_NAVIGATION__THUMBS__________________________________, KC_MS_L,  KC_MS_D,  KC_MS_R
    ),

    [_GAMING] = LAYOUT_wrapper(
        KC_ESC,  _________________________________________F_KEYS_________________________________________,  _______,  _______,
        KC_GRV,  _______________________________NUMBERS________________________________, KC_MINS, _______,  _______,  _______,
        ________________________________________QWERTY_1________________________________________, _______,            _______,
        ________________________________________GAMING_2________________________________________, _______,  _______,  _______,
        ________________________________________GAMING_3________________________________________,           _______,  _______,
        ____________________________________7_GAMING__THUMBS____________________________________, _______,  _______,  _______
    ),

    [_ADJUST] = LAYOUT_wrapper(
        QK_BOOT, ________________________________________FN_KEYS_________________________________________,  _______,  _______,
        ________________________________________ADJUST__0_______________________________________, _______,  QK_BOOT,  _______,
        ________________________________________ADJUST__1_______________________________________, _______,            _______,
        ________________________________________ADJUST__2_______________________________________, FN_EXIT,  _______,  _______,
        ________________________________________ADJUST__3_______________________________________,           RGB_MOD,  _______,
        ________________________________________7_THUMBS________________________________________, RGB_SPD, RGB_RMOD,  RGB_SPI
    ),
};

// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERT] =    { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_SYM] =      { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_NUM] =      { ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_NAV] =      { ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [_GAMING] =   { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_ADJUST] =   { ENCODER_CCW_CW(KC_MSEL, KC_MPRV) }
};
#endif

// const custom_shift_key_t custom_shift_keys[] = {
//   {DE_CIRC, N2_HCEK},  // Shift 2 is §
//   {DE_1 , N2__DEG},    // Shift 1 is °
//   {DE_2 , DE_SECT},    // Shift 2 is §
// //   {DE_3 , N2_SS_L},    // Shift 3 is ℓ
// //   {DE_4 , N2_RAQO},    // Shift 4 is »
// //   {DE_5 , N2_LAQO},    // Shift 5 is «
//   {DE_6 , N3__DLR},    // Shift 6 is $
//   {DE_7 , N2__EUR},    // Shift 7 is €
// //   {DE_8 , N2_LODQ},    // Shift 8 is „
// //   {DE_9 , N2_L_DQ},    // Shift 9 is “
// //   {DE_0 , N2_R_DQ},    // Shift 0 is ”
// //   {DE_SS, N2_C_SS},    // Shift ß is ẞ

//   {DE_ACUT , DE_TILD}, // Shift ´ is ~

//   {DE_COMM, N2_NDSH},  // Shift , is –
//   {DE_DOT , N2_BULT},  // Shift . is •
//   {KC_SLSH, N3_MDSH},  // Shift - is —

//   {KC_BSPC, KC_DEL},  // Shift Backspace is Delete
// };

// uint8_t NUM_CUSTOM_SHIFT_KEYS =
//     sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    #ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    #endif

    #ifdef CONSOLE_ENABLE
    switch (keycode) {
        case RN_STEM:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(100) "steam" SS_DELAY(500) SS_TAP(X_ENT));
                print("steam\n");
            } else {
                // when keycode is released
            }
            break;
        case RN_CODE:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI) SS_DELAY(100) "Visual Studio Code" SS_DELAY(500) SS_TAP(X_ENT));
                print("code\n");
            } else {
                // when keycode is released
            }
            break;
    }
    #endif

    if (!process_layer_lock(keycode, record, F_LLOCK)) { return false; }
    if (!process_caps_word(keycode, record)) { return false; }
    // if (!process_custom_shift_keys(keycode, record)) { return false; }

    return true;
}

// const key_override_t hcek_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_CIRC, N2_HCEK);
// const key_override_t deg_key_override  = ko_make_basic(MOD_MASK_SHIFT, DE_1, N2__DEG);
// const key_override_t sect_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_2, DE_SECT);
// const key_override_t ss_l_override     = ko_make_basic(MOD_MASK_SHIFT, DE_3, N2_SS_L);
// const key_override_t raqo_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_4, N2_RAQO);
// const key_override_t laqo_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_5, N2_LAQO);
// const key_override_t dlr_key_override  = ko_make_basic(MOD_MASK_SHIFT, DE_6, N3__DLR);
// const key_override_t eur_key_override  = ko_make_basic(MOD_MASK_SHIFT, DE_7, N2__EUR);
// const key_override_t lodq_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_8, N2_LODQ);
// const key_override_t l_dq_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_9, N2_L_DQ);
// const key_override_t r_dq_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_0, N2_R_DQ);
// const key_override_t ss_key_override   = ko_make_basic(MOD_MASK_SHIFT, DE_SS,N2_C_SS);

// const key_override_t tild_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_ACUT, DE_TILD);

// const key_override_t ndsh_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_COMM, N2_NDSH);
// const key_override_t bult_key_override = ko_make_basic(MOD_MASK_SHIFT, DE_DOT,  N2_BULT);
// const key_override_t mdsh_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, N3_MDSH);

// const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);



// // This globally defines all key overrides to be used
// const key_override_t **key_overrides = (const key_override_t *[]){
//     &hcek_key_override,
//     &deg_key_override,
//     &hcek_key_override,
//     &deg_key_override,
//     &sect_key_override,
//     &ss_l_override,
//     &raqo_key_override,
//     &laqo_key_override,
//     &dlr_key_override,
//     &eur_key_override,
//     &lodq_key_override,
//     &l_dq_key_override,
//     &r_dq_key_override,
//     &ss_key_override,
//     &tild_key_override,
//     &ndsh_key_override,
//     &bult_key_override,
//     &mdsh_key_override,
//     &delete_key_override,
//     NULL // Null terminate the array of overrides!
// };

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
}
