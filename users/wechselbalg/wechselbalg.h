#include "keymap_german_ia.h"
#include "keymap_neo2.h"
#include "quantum.h"

enum layers {
  _NEO_QWERTZ = 0,
  _NEO_3,
  _NEO_4,
  _FN,
  _CONFIG
};

enum splitlayers {
    _DEFAULTS = 0,
    _QWERT = 0,
    _DVORAK,
    _COLEMAK_DH,
    _MINE,
    _VOUX,
    _SYM,
    _NAV,
    _NUM,
    _GAMING,
    _ADJUST,
};

enum CustomKeys {
  RN_STEM = SAFE_RANGE,
  RN_CODE
};

// QWERT
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________QWERTY_L1_________________  KC_TAB  , DE_Q   , DE_W   , DE_E   , DE_R   , DE_T
#define _________________QWERTY_L2_________________  KC_SYM  , DE_A   , DE_S   , DE_D   , DE_F   , DE_G
#define _________________QWERTY_L3_________________  KC_LSFT , DE_Y   , DE_X   , DE_C   , DE_V   , DE_B

#define _________________QWERTY_R1_________________  DE_Z    , DE_U   , DE_I   , DE_O   , DE_P     , DE___UE
#define _________________QWERTY_R2_________________  DE_H    , DE_J   , DE_K   , DE_L   , DE___OE  , KAE_SYM
#define _________________QWERTY_R3_________________  DE_N    , DE_M   , DE_COMM, DE_DOT , DE_MINS  , KC_RSFT

// VOUX
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define __________________VOUX__L1_________________  KC_TAB  , DE_V   , DE__DOT, DE_O   , DE_U   , DE_AE
#define __________________VOUX__L2_________________  KC_SYM  , DE_C   , DE_A   , DE_E   , DE_I   , DE_Z
#define __________________VOUX__L3_________________  KC_LSFT , DE_Y   , DE_COMM, DE_UE  , DE_UE  , DE_OE

#define __________________VOUX__R1_________________  DE_P    , DE_G   , DE_L   , DE_H   , DE_F   , DE_J
#define __________________VOUX__R2_________________  DE_B    , DE_T   , DE_R   , DE_N   , DE_S   , KSS_SYM
#define __________________VOUX__R3_________________  DE_Q    , DE_D   , DE_W   , DE_M   , DE_K   , KC_RSFT

// MINE
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define __________________MINE__L1_________________  KC_TAB  , DE_J   , DE_L   , DE_U   , DE_A   , DE_Q
#define __________________MINE__L2_________________  KC_SYM  , DE_C   , DE_I   , DE_A   , DE_E   , DE_O
#define __________________MINE__L3_________________  KC_LSFT , DE_V   , DE_X   , DE___UE, DE___AE, DE___OE

#define __________________MINE__R1_________________  DE_W    , DE_B   , DE_D   , DE_G   , DE_Y   , DE_SS
#define __________________MINE__R3_________________  DE_P    , DE_F   , DE_COMM, DE_DOT , DE_K   , KC_RSFT
#define __________________MINE__R2_________________  DE_M    , DE_N   , DE_T   , DE_S   , DE_H   , KCZ_SYM

// DVORAK
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________DVORAK_L1_________________  KC_TAB  ,KC_QUOTE, DE_COMM, DE__DOT, DE_P   , DE_Y
#define _________________DVORAK_L2_________________  KC_SYM  , DE_A   , DE_O   , DE_E   , DE_U   , DE_I
#define _________________DVORAK_L3_________________  KC_LSFT , KC_SCLN, DE_Q   , DE_J   , DE_K   , DE_X

#define _________________DVORAK_R1_________________  DE_F    , DE_G   , DE_C   , DE_R   , DE_L     , KC_BSPC
#define _________________DVORAK_R2_________________  DE_D    , DE_H   , DE_T   , DE_N   , DE_S     , MIN_SYM
#define _________________DVORAK_R3_________________  DE_B    , DE_M   , DE_W   , DE_V   , DE_Z     , KC_RSFT

// COLEMAK DH
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _____________COLEMAK_DH_L1_________________  KC_TAB  , DE_Q   , DE_W   , DE_F   , DE_P   , DE_B
#define _____________COLEMAK_DH_L2_________________  KC_SYM  , DE_A   , DE_R   , DE_S   , DE_T   , DE_G
#define _____________COLEMAK_DH_L3_________________  KC_LSFT , DE_Z   , DE_X   , DE_C   , DE_D   , DE_V

#define _____________COLEMAK_DH_R1_________________  DE_J    , DE_L   , DE_U   , DE_Y   , DE_SCLN  , KC_BSPC
#define _____________COLEMAK_DH_R2_________________  DE_H    , DE_N   , DE_E   , DE_I   , DE_O     , KC_SYM
#define _____________COLEMAK_DH_R3_________________  DE_K    , DE_H   , DE_COMM, DE_DOT , DE_MINS  , KC_RSFT

// GAMING
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
// |  Q  |  W  |  E  |  R  |  T  |   |  Y  |  U  |  I  |  O  |  P  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  A  |  S  |  D  |  F  |  G  |   |  H  |  J  |  K  |  L  |  ;  |
// ,-----+-----+-----x-----x-----,   ,-----x-----x-----+-----+-----,
// |  Z  |  X  |  C  |  V  |  B  |   |  N  |  M  |  ,  |  .  |  /  |
// ,-----+-----+-----+-----+-----,   ,-----+-----+-----+-----+-----,
#define _________________GAMING_L1_________________  KC_TAB  , DE_Q   , DE_W   , DE_E   , DE_R   , DE_T
#define _________________GAMING_L2_________________  KC_SYM  , DE_A   , DE_S   , DE_D   , DE_F   , DE_G
#define _________________GAMING_L3_________________  KC_LSFT , DE_Y   , DE_X   , DE_C   , DE_V   , DE_B

#define _________________GAMING_R1_________________  DE_Z    , DE_U   , DE_I   , DE_O   , DE_P     , DE___UE
#define _________________GAMING_R2_________________  DE_H    , DE_J   , DE_K   , DE_L   , DE___OE  , KAE_SYM
#define _________________GAMING_R3_________________  DE_N    , DE_M   , DE_COMM, DE_DOT , DE_MINS  , KC_RSFT
