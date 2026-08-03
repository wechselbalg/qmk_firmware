#pragma once

#include "keymap_german_ia.h"
#include "keymap_neo2.h"
#include "quantum.h"
#include "wrappers.h"
#include "tap_dance.h"
#include "features/custom_shift_keys.h"

#ifdef TAP_DANCE_ENABLE
    #include "tap_dance.h"
#endif

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
    _MAC,   // Mac-Modifier-Overlay: liegt ueber allen anderen Layern, nur Daumenreihe belegt (TG__MAC)
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
#define TG__MAC  TG(_MAC)

// Layer Lock kommt aus dem QMK-Core (LAYER_LOCK_ENABLE); Alias behaelt die 7-Zeichen-Rasterbreite
#define F_LLOCK  QK_LLCK

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
#define NUM_ENT  LT(_NUM, KC_ENT)

#define NAV_TAB  LT(_NAV, KC_TAB)
#define NAV_BSC  LT(_NAV, KC_BSPC)
#define NAV_NM0  LT(_NAV, N3_NUM0)
#define NAV_ENT  LT(_NAV, KC_ENT)
#define NAV_SPC  LT(_NAV, KC_SPC)

#define SC_BSPC  C_S_T(KC_BSPC)
#define CTL_ENT  LCTL_T(KC_ENT)
#define RTL_ENT  RCTL_T(KC_ENT)

#define CTL_SPC  LCTL_T(KC_SPC)
#define RALT_AP  RALT_T(KC_APP)

/*
Escape-Taste der Overlay-Layer (linke obere Ecke von SYM/NUM/NAV/ADJUST).

ACHTUNG, hier stand frueher TO(QWERT): `QWERT` ist der *Custom-Keycode* aus
enum CustomKeys (SAFE_RANGE+3 = 0x7E43), nicht der Layer-Index. TO() maskiert
mit & 0x1F -> TO(3) -> Layer _MINE. Die Taste sprang also auf MINE statt
aufzuraeumen. Richtig ist der Layer _QWERT.

Semantik in QMK: TO(_QWERT) = layer_move(0), loescht also alle momentanen und
getoggelten Layer (auch _MAC). Das gewaehlte Basis-Layout bleibt erhalten,
weil QMK default_layer_state getrennt fuehrt und jedes Basis-Layout einen
Index >= 0 hat -- Layer 0 liegt dann nur wirkungslos darunter.

Das gilt NICHT fuer _GAMING: das wird per DF(_GAMING) als *Default*-Layer
betreten und liegt damit ueber Layer 0. GAMING braucht deshalb D_QWERT
(bzw. spaeter ein DF_PREV) als Ausstieg, nicht FN_EXIT.
*/
#define FN_EXIT      TO(_QWERT)
