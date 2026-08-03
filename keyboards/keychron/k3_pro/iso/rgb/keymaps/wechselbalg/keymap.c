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

#include QMK_KEYBOARD_H
#include "tap_dance_actions.h"  // Zentrale Definition

// Einfacher Include statt direkter Definition
tap_dance_action_t tap_dance_actions[] = {
    WECHSELBALG_TAP_DANCE_ACTIONS
};

// clang-format off

/*
Der frueherer _TEST-Layer (RGB-/Bluetooth-Testebene) ist am 2026-08-04
entfallen: er war von keiner Taste erreichbar (`_TEST` kam im ganzen Repo nur
in seiner eigenen Definition vor) und in diesem Build vollstaendig redundant --
BT_HST1..3 waren `_______`, weil Bluetooth aus ist
(`# OPT_DEFS += -DKC_BLUETOOTH_ENABLE` in keyboards/keychron/k3_pro/rules.mk),
und RGB-Steuerung plus NK_TOGG liegen bereits auf _ADJUST.

Falls der Bluetooth-Build je aktiviert wird: BT_HST1..3 und BAT_LVL gehoeren
dann auf _ADJUST, nicht in einen eigenen Layer (siehe git-Historie fuer die
alte Belegung).
*/

#undef LAYOUT_wrapper
#define LAYOUT_wrapper(...) LAYOUT_iso_85(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#ifdef WB_LAYOUT_MINE
    [_MINE] = LAYOUT_wrapper(
        KC_ESC,   _________________________________________F_KEYS_________________________________________,  KC_CALC,  KC_MAIL,  RGB_TOG,
        // 2026-08-04, RUECKGAENGIG MACHEN = `DE_MINS,   DE_GRV,`:
        // die beiden ISO-Extratasten rechts der 0 senden jetzt das, was auf
        // ihnen steht (ss und Akut), wie auf allen anderen Basis-Layouts.
        // Vorher: DE_MINS (Dublette -- `-` liegt auf MINE schon auf RFT_MIN,
        // rechter Pinky) und DE_GRV (= S(DE_ACUT), ein Dead Key: erst nach
        // einem zusaetzlichen Space erscheint das Zeichen).
        KC_GRV, _______________________________NUMBERS________________________________, DE_SS,     DE_ACUT,  KC_BSPC,            KC_DEL,
        ________________________________________MINE___1________________________________________,  DE_PLUS,  KC_ENT,   KC_HOME,
        // 2026-08-04, RUECKGAENGIG MACHEN = `SYM_ACU,`:
        // die physische ISO-#-Taste sendet jetzt auch auf MINE `#` statt `\u00b4`,
        // wie auf allen anderen Basis-Layouts. Der Akut ist seit der Aenderung
        // eine Zeile darueber auf seiner eigenen Taste erreichbar.
        ________________________________________MINE___2________________________________________,  SYM_HSH,                      KC_END,
        ________________________________________MINE___3________________________________________,            KC_UP,    MO__ADJ,
        ________________________________________7_THUMBS________________________________________,  KC_LEFT,  KC_DOWN,  KC_RGHT),
#endif

    [_QWERT] = LAYOUT_wrapper(
        // 2026-08-04: RN_CODE -> KC_MAIL. RN_CODE hatte auf diesem Board nie
        // einen Handler (der einzige steht in der GMMK-Keymap, dort auch nur
        // unter #ifdef CONSOLE_ENABLE) -- die Taste tat schlicht nichts.
        KC_ESC,  _________________________________________F_KEYS_________________________________________, KC_MAIL,  KC_CALC,  RGB_MOD,
        KC_GRV, _______________________________NUMBERS________________________________, DE_SS,    DE_ACUT,           KC_BSPC,  KC_DEL,
        ________________________________________QWERTY_1________________________________________, KC_RBRC, KC_ENT,             KC_HOME,
        ________________________________________QWERTY_2________________________________________, SYM_HSH,                     KC_END,
        ________________________________________QWERTY_3________________________________________,          KC_UP,    MO__ADJ,
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
        ________________________________________NUMBER__3_______________________________________,           _______,  _______,
        ________________________________7_NUMBER__THUMBS________________________________________, _______, _______,  _______
    ),

    [_NAV] = LAYOUT_wrapper(
        KC_ESC,  ________________________________________FN_KEYS_________________________________________,   KC_F13,  _______,  _______,
        ______________________________________NAVIGATION__0_____________________________________, ___NO__,            _______,  _______,
        ______________________________________NAVIGATION__1_____________________________________, N3_SLSH,  _______,            UC_NEXT,
        ______________________________________NAVIGATION__2_____________________________________, _______,                      UC_PREV,
        ______________________________________NAVIGATION__3_____________________________________,           MS_UP  ,  _______,
        __________________________________7_NAVIGATION__THUMBS__________________________________, MS_LEFT,  MS_DOWN,  MS_RGHT
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
    ),

};

/*
Handedness fuer CHORDAL_HOLD -- siehe den ausfuehrlichen Kommentar in der
GMMK-Pro-Keymap. Kurz: '*' in der letzten Reihe ist die Leertaste und damit
NAV_SPC, der einzige Tap-Hold der Daumenreihe.
Nachpruefbar mit `qmk generate-keyboard-c -kb keychron/k3_pro/iso/rgb`.
*/
#ifdef CHORDAL_HOLD
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_wrapper(

  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
  'L', 'L', 'L', '*', 'R', 'R', 'R', 'R', 'R', 'R'
);
#endif


bool process_record_keymap(uint16_t keycode, keyrecord_t* record) {

  return true;
}
