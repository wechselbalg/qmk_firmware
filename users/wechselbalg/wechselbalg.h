#pragma once

#include "keymap_german_ia.h"
#include "keymap_neo2.h"
#include "quantum.h"
#include "wrappers.h"
#include "tap_dance.h"
#include "features/layer_lock.h"
#include "features/caps_word.h"
#include "features/custom_shift_keys.h"

#ifdef TAP_DANCE_ENABLE

#endif

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
    _COLEMAKDH,
    _MINE,
    _VOU,
    _SYM,
    _NAV,
    _NUM,
    _GAMING,
    _ADJUST,
};

enum CustomKeys {
  RN_STEM = SAFE_RANGE,
  RN_CODE,
  A_MSJIG,
  QWERT,
  DVORAK,
  COLEMAK,
  VOU,
  MINE,
  KC_D_MUTE,
  F_LLOCK,
  DBRACES,
  FF_WORD,
  RV_WORD
};

// Aliases for readability

#define D_QWERT  DF(_QWERT)
#define D_COLMK  DF(_COLEMAKDH)
#define D_DVORK  DF(_DVORAK)
#define D__MINE  DF(_MINE)
#define D___VOU  DF(_VOU)
#define D__GAME  DF(_GAMING)

#define MO__SYM  MO(_SYM)
#define MO__NAV  MO(_NAV)
#define MO__NUM  MO(_NUM)
#define MO__ADJ  MO(_ADJUST)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_QUOT MT(MOD_RCTL, DE_QUOTE)

#define RALT_PR ALGR_T(KC_PSCR)

#define SFT_SPC  LSFT_T( KC_SPC)
#define SFT_ENT  LSFT_T( KC_ENT)
#define SFT_NUM  LSFT_T( NUM)
#define SFT_PIP  LSFT_T( DE_PIPE)
#define RFT_ENT  RSFT_T( KC_ENT)
#define RFT_SPC  RSFT_T( KC_SPC)
#define RFT__SS  RSFT_T( DE_SS)
#define RFT_MIN  RSFT_T( DE_MINS)

#define SFT_CTL  LSFT(KC_LCTL)

#define SYM__AE  LT(_SYM, DE___AE)
#define SYM_MIN  LT(_SYM, DE_MINS)
#define SYM__SS  LT(_SYM, DE_SS)
#define SYM___Y  LT(_SYM, DE_Y)
#define SYM___F  LT(_SYM, DE_F)
#define SYM___Z  LT(_SYM, DE_Z)
#define SYM_HSH  LT(_SYM,DE_HASH)
#define SYM_ACU  LT(_SYM,DE_ACUT)


#define NUM___Y  LT(_NUM, DE_Y)
#define NUM_SCL  LT(_NUM, DE_SCLN)
#define NUM__UE  LT(_NUM, DE___UE)
#define NUM___X  LT(_NUM, DE_X)
#define NUM_DEL  LT(_NUM, KC_DEL)
#define NUM_BSC  LT(_NUM, KC_BSPC)
#define NUM__SS  LT(_NUM, DE_SS)
#define NUM___Z  LT(_NUM, DE_Z)
#define NUM___J  LT(_NUM, DE_J)
#define NUM_NM0  LT(_NUM, N3_NUM0)

#define NAV_TAB  LT(_NAV, KC_TAB)
#define NAV_BSC  LT(_NAV, KC_BSPC)
#define NAV_NM0  LT(_NAV, N3_NUM0)

#define SC_BSPC  C_S_T(KC_BSPC)
#define CTL_ENT  LCTL_T(KC_ENT)
#define RTL_ENT  RCTL_T(KC_ENT)
#define CTL_SPC  LCTL_T(KC_SPC)
#define RALT_AP  RALT_T(KC_APP)

#define FN_EXIT        TO(QWERT)      // Turns off all layers, except the default


