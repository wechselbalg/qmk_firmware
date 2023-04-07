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

enum k3layers{
  MINE = 0,
  TEST,
  QWERTY,
  SYM,
  NUM,
  NAV,
  FN,
};

enum splitlayers {
    _QWERTY = 0,
    _DVORAK,
    _COLEMAK_DH,
    _MINE,
    _VOUX,
    _NAV,
    _SYM,
    _NUM,
    _GAMING,
    _ADJUST,
};

enum CustomKeys {
  RN_STEM = SAFE_RANGE,
  RN_CODE,
  F_LLOCK,
};
