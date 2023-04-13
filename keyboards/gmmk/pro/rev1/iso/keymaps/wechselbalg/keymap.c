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

#include QMK_KEYBOARD_H
#include "wechselbalg.h"
#include "print.h"
#include "features/custom_shift_keys.h"
// clang-format off

#define SYM_HSH  LT(_NEO_3,DE_HASH)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     /        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right

    // [_BASE] = LAYOUT(
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
    [_NEO_QWERTZ] = LAYOUT(
        KC_ESC,       KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,     KC_F11,  KC_F12,     RN_CODE,          KC_MUTE,
        KC_GRV,       KC_1,       KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,       KC_MINS, KC_EQL,     KC_BSPC,          KC_DEL,
        KC_TAB,       KC_Q,       KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       DE_UE,   KC_RBRC,                      KC_PGUP,
        MO(_NEO_3),   KC_A,       KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    DE_OE,      DE_AE,   SYM_HSH,    KC_ENT,           KC_PGDN,
        KC_LSFT,      MO(_NEO_4), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,     KC_SLSH, KC_RSFT,             KC_UP,   KC_END,
        KC_LCTL,      KC_LGUI,    KC_LALT,                            KC_SPC,                             MO(_NEO_4), MO(_FN), KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_NEO_3] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, N3_SUP1, N3_SUP2, N3_SUP3, N3_RSAQ, N3_LSAQ, N3_CENT, N3__YEN, N3_SBSQ, N3_L_SQ, N3_R_SQ, ___NO__, ___NO__, _______,          _______,
        _______, N3_ELPS, N3_UNDS, N3_LBRC, N3_RBRC, N3_CIRC, N3_EXLM, N3_LABK, N3_RABK, N3_EQUL, N3_AMPR, N3_SL_S, N3_SLSH,                   _______,
        _______, N3_BSLS, N3_SLSH, N3_CLBR, N3_CRBR, N3_ASTR, N3_QUES, N3_LPRN, N3_RPRN, N3_MINS, N3_COLN, N3___AT, _______, _______,          _______,
        _______, _______, N3_HASH, N3__DLR, N3_PIPE, N3_TILD, N3__GRV, N3_PLUS, N3_PERC, N3_DQUO, N3_QUOT, N3_SCLN, _______,          _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
    ),

    [_NEO_4] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, N3_F_OR, N3_M_OR, N3_NUMS, ___NO__, N3_MDOT, N3_BPND, N3_CURS,  KC_TAB, N3_SLSH, N3_ASTR, N3_NMNS, ___NO__, _______,          _______,
        _______, N3_PGUP, N3_BSPC, N3___UP, N3__DEL, N3_PGDN, N3_IEXL, N3_NUM7, N3_NUM8, N3_NUM9, N3_NPLS, N3_DMNS, ___NO__,                   _______,
        _______, N3_HOME, N3_LEFT, N3_DOWN, N3_RGHT, N3__END, N3_IQES, N3_NUM4, N3_NUM5, N3_NUM6, N3_COMM, N3__DOT, _______, _______,          _______,
        _______, _______, N3__ESC, N3__TAB, N3__INS, N3__ENT, N3_UNDO, N3_COLN, N3_NUM1, N3_NUM2, N3_NUM3, N3_SCLN, _______,          KC_MS_U, _______,
        _______, _______, _______,                            N3_NUM0,                            _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_R
    ),

    [_FN] = LAYOUT(
        QK_BOOT, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______,  KC_F13,          _______,
        _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,          _______,
        _______, RGB_VAI,  UC_WIN, UC_WINC, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   UC_NEXT,
        _______, DE_BSLS, DE_SLSH, _______, _______, _______, _______, _______, _______, UC_LINX, _______, _______, _______, _______,          UC_PREV,
        _______, _______, _______, RGB_HUI, _______, _______, _______, NK_TOGG,  UC_MAC, _______, _______, _______, _______,          RGB_MOD,  KC_F14,
        _______, _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),

    [_CONFIG] = LAYOUT(
        QK_BOOT, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,          _______,
        _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,          _______,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
        _______, DE_BSLS, DE_SLSH, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______, _______,          RGB_MOD, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),


};

// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_NEO_QWERTZ] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NEO_3] =      { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_NEO_4] =      { ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_FN] =         { ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [_CONFIG] =     { ENCODER_CCW_CW(KC_MSEL, KC_MPRV) }
};
#endif

const custom_shift_key_t custom_shift_keys[] = {
  {DE_CIRC, N2_HCEK},  // Shift 2 is §
  {DE_1 , N2__DEG},    // Shift 1 is °
  {DE_2 , DE_SECT},    // Shift 2 is §
//   {DE_3 , N2_SS_L},    // Shift 3 is ℓ
//   {DE_4 , N2_RAQO},    // Shift 4 is »
//   {DE_5 , N2_LAQO},    // Shift 5 is «
  {DE_6 , N3__DLR},    // Shift 6 is $
  {DE_7 , N2__EUR},    // Shift 7 is €
//   {DE_8 , N2_LODQ},    // Shift 8 is „
//   {DE_9 , N2_L_DQ},    // Shift 9 is “
//   {DE_0 , N2_R_DQ},    // Shift 0 is ”
//   {DE_SS, N2_C_SS},    // Shift ß is ẞ

  {DE_ACUT , DE_TILD}, // Shift ´ is ~

  {DE_COMM, N2_NDSH},  // Shift , is –
  {DE_DOT , N2_BULT},  // Shift . is •
  {KC_SLSH, N3_MDSH},  // Shift - is —

  {KC_BSPC, KC_DEL},  // Shift Backspace is Delete
};

uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);


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
