#pragma once
#include "keymap.h"

// State bitmap to track which key(s) enabled NEO_3 layer
static uint8_t neo3_state = 0;
// State bitmap to track key combo for CAPSLOCK
static uint8_t capslock_state = 0;

// bitmasks for modifier keys
#define MODS_NONE   0
#define MODS_SHIFT  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
#define MODS_CTRL   (MOD_BIT(KC_LCTL)|MOD_BIT(KC_RCTRL))
#define MODS_ALT    (MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
#define MODS_GUI    (MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI))

// // Used to trigger macros / sequences of keypresses
// enum custom_keycodes {
//   PLACEHOLDER = SAFE_RANGE,     // can always be here
//   NEO2_LMOD3,
//   NEO2_RMOD3,
//   YELDIR_AC,
//   YELDIR_CTLTAB,
//   YELDIR_CTLSTAB,
//   NEO2_1,
//   NEO2_2,
//   NEO2_3,
//   NEO2_4,
//   NEO2_5,
//   NEO2_6,
//   NEO2_7,
//   NEO2_8,
//   NEO2_9,
//   NEO2_0,
//   NEO2_MINUS,
//   NEO2_COMMA,
//   NEO2_DOT,
//   NEO2_SHARP_S
// };

#define NEO2_LMOD4                   MO(NEO_4)
#define NEO2_RMOD4                   NEO2_LMOD4

// Use _______ to indicate a key that is transparent / falling through to a lower level
#define _______ KC_TRNS

// NEO_2 special characters
#define NEO2_L2_CAPITAL_SS           UC(L'ℓ')                    // ℓ
#define NEO2_L2_RAQUO                UC(L'»')                    // »   RALT(DE_Y)
#define NEO2_L2_LAQUO                UC(L'«')                    // «   RALT(DE_X)

// NEO_3 special characters
#define NEO2_L3_CAPITAL_SS           UC(L'ẞ')                    // ẞ    RSA(DE_S)
#define NEO2_L3_CAPITAL_UE           S(DE_UE)                    // Ü
#define NEO2_L3_CAPITAL_OE           S(DE_OE)                    // Ö
#define NEO2_L3_CAPITAL_AE           S(DE_AE)                    // Ä
#define NEO2_L3_SUPERSCRIPT_1        UC(L'¹')                    // ¹   RALT(DE_1)
#define NEO2_L3_SUPERSCRIPT_2        DE_SUP2                     // ²
#define NEO2_L3_SUPERSCRIPT_3        DE_SUP3                     // ³
#define NEO2_L3_RSAQUO               UC(L'›')                    // ›   RSA(DE_Y)
#define NEO2_L3_LSAQUO               UC(L'‹')                    // ‹   RSA(DE_X)
#define NEO2_L3_CENT                 UC(L'¢')                    // ¢   RALT(DE_C)
#define NEO2_L3_YEN                  UC(L'¥')                    // ¥   RSA(DE_Z)
#define NEO2_L3_SBQUO                UC(L'‚')                    // ‚   RSA(DE_V)
#define NEO2_L3_LEFT_SINGLE_QUOTE    UC(L'‘')                    // ‘   RSA(DE_B)
#define NEO2_L3_RIGHT_SINGLE_QUOTE   UC(L'’')                    // ’   RSA(DE_N)
#define NEO2_L3_LOW9_DBQUOTE         UC(L'„')                    // „   RALT(DE_V)
#define NEO2_L3_LEFT_DBQUOTE         UC(L'„')                    // “   RALT(DE_B)
#define NEO2_L3_RIGHT_DBQUOTE        UC(L'„')                    // ”   RALT(DE_N)
#define NEO2_L3_ELLIPSIS             UC(L'…')                    // …    RALT(DE_DOT)
#define NEO2_L3_UNDERSCORE           DE_UNDS                     // _
#define NEO2_L3_LBRACKET             DE_LBRC                     // [
#define NEO2_L3_RBRACKET             DE_RBRC                     // ]
#define NEO2_L3_CIRCUMFLEX           DE_CIRC                     // ^
#define NEO2_L3_EXCLAMATION          DE_EXLM                     // !
#define NEO2_L3_LESSTHAN             DE_LABK                     // <
#define NEO2_L3_GREATERTHAN          DE_RABK                     // >
#define NEO2_L3_EQUAL                DE_EQL                      // =
#define NEO2_L3_AMPERSAND            DE_AMPR                     // &
#define NEO2_L3_SMALL_LONG_S         UC(L'ſ')                    // ſ   RALT(DE_S)
#define NEO2_L3_BSLASH               DE_BSLS                     // (backslash)
#define NEO2_L3_SLASH                DE_SLSH                     // /
#define NEO2_L3_CLBRACKET            DE_LCBR                     // {
#define NEO2_L3_CRBRACKET            DE_RCBR                     // }
#define NEO2_L3_ASTERISK             DE_ASTR                     // *
#define NEO2_L3_QUESTIONMARK         DE_QUES                     // ?
#define NEO2_L3_LPARENTHESES         DE_LPRN                     // (
#define NEO2_L3_RPARENTHESES         DE_RPRN                     // )
#define NEO2_L3_HYPHEN_MINUS         DE_MINS                     // -
#define NEO2_L3_COLON                DE_COLN                     // :
#define NEO2_L3_AT                   DE_AT                       // @
#define NEO2_L3_HASH                 DE_HASH                     // #
#define NEO2_L3_PIPE                 DE_PIPE                     // |
#define NEO2_L3_TILDE                DE_TILD                     // ~
#define NEO2_L3_BACKTICK             DE_GRV                      // `
#define NEO2_L3_PLUS                 DE_PLUS                     // +
#define NEO2_L3_PERCENT              DE_PERC                     // %
#define NEO2_L3_DOUBLE_QUOTE         DE_DQUO                     // "
#define NEO2_L3_SINGLE_QUOTE         DE_QUOT                     // '
#define NEO2_L3_SEMICOLON            DE_SCLN                     // ;

// NEO_4 special characters
#define NEO2_L3_FEMININE_ORDINAL     RSA(DE_F)                   // ª
#define NEO2_L3_MASCULINE_ORDINAL    RSA(DE_M)                   // º
#define NEO2_L3_NUMERO_SIGN          KC_NO                       // №
#define NEO2_L3_MIDDLE_DOT           RALT(DE_COMM)               // ·
#define NEO2_L3_BRITISH_POUND        RSA(DE_3)                   // £
#define NEO2_L3_CURRENCY_SIGN        RSA(DE_4)                   // ¤
#define NEO2_L3_INV_EXCLAMATION      RSA(DE_1)                   // ¡
#define NEO2_L3_INV_QUESTIONMARK     UC(L'¿')                    // ¿   RSA(DE_SS)
#define NEO2_L3_DOLLAR               DE_DLR                      // $
#define NEO2_L3_EN_DASH              UC(L'–')                    // –   RALT(DE_MINS)
#define NEO2_L3_EM_DASH              UC(L'—')                    // —   RSA(DE_MINS)
