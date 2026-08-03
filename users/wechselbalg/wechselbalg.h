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

/*
Welche alternativen Basis-Layouts einkompiliert werden.

Jedes kostet rund 120 Byte Flash (60 Tasten x 2 Byte), auf den Split-Boards
also knapp 500 Byte fuer alle vier. Die ATmega32u4-Boards brauchen den Platz
fuer CHORDAL_HOLD und Retro Tapping -- dort ist deshalb nur eines dabei,
per Default COLEMAKDH. Dasselbe Modell wie KMKs ACTIVE_CANDIDATE.

**Umstellen = genau eine Zeile hier aendern und neu flashen.** Ein anderes
Layout will man: `#define WB_LAYOUT_MINE` statt `WB_LAYOUT_COLEMAKDH`. Ein
einzelnes Board abweichend: das passende WB_LAYOUT_* per
`OPT_DEFS += -DWB_LAYOUT_...` in dessen rules.mk setzen -- sobald hier
irgendeines von aussen definiert ist, gilt ausschliesslich die aeussere Wahl.

Die Layouts selbst bleiben in wrappers.h vollstaendig erhalten, es wird nur
kein Layer dafuer angelegt.
*/
#if !defined(WB_LAYOUT_DVORAK) && !defined(WB_LAYOUT_COLEMAKDH) && !defined(WB_LAYOUT_MINE) && !defined(WB_LAYOUT_VOU)
#    ifdef __AVR__
#        define WB_LAYOUT_COLEMAKDH
#    else
#        define WB_LAYOUT_DVORAK
#        define WB_LAYOUT_COLEMAKDH
#        define WB_LAYOUT_MINE
#        define WB_LAYOUT_VOU
#    endif
#endif

/*
Zusatzfunktionen, die eigenen Flash kosten:
  WB_JIGGLER  -- Mouse Jiggler auf A_MSJIG (C4)
  WB_DF_PREV  -- Ruecksprung auf das vorherige Basis-Layout (C6)

Gemessen 2026-08-04, beide zusammen rund 240 Byte. Das haben die AVR-Boards
nicht: sofle/rev1 waere 226 Byte drueber, sofle_choc (weiss) 50, nur die Kyria
haette mit 1230 Byte frei Platz. Deshalb dasselbe Muster wie bei
MAGIC_ENABLE/WB_LAYOUT_*: auf AVR aus, sonst an.

Ohne die Flags sind die Tasten wirkungslos statt falsch: A_MSJIG tut nichts
(wie bisher), D__PREV faellt unten per Alias auf D_QWERT zurueck.

Ein einzelnes Board abweichend: -DWB_JIGGLER / -DWB_DF_PREV per OPT_DEFS in
dessen Keymap-rules.mk. Faellt ersatzlos weg, sobald die AVR-Boards auf
bessere Controller umgezogen sind.
*/
#ifndef __AVR__
#    define WB_JIGGLER
#    define WB_DF_PREV
#endif

/*
Der Jiggler schickt einen Maus-Report -- ohne Mouse Keys gibt es den
HID-Endpunkt dafuer nicht. MOUSEKEY_ENABLE war in diesem Repo schon einmal
laengere Zeit `no` (macOS-Touchpad-Konflikt), deshalb hier abgesichert statt
auf einen Linkfehler zu warten.
*/
#if defined(WB_JIGGLER) && !defined(MOUSEKEY_ENABLE)
#    undef WB_JIGGLER
#endif

enum splitlayers {
    _DEFAULTS = 0,
    _QWERT = 0,
#ifdef WB_LAYOUT_DVORAK
    _DVORAK,
#endif
#ifdef WB_LAYOUT_COLEMAKDH
    _COLEMAKDH,
#endif
#ifdef WB_LAYOUT_MINE
    _MINE,
#endif
#ifdef WB_LAYOUT_VOU
    _VOU,
#endif
    _SYM,
    _NAV,
    _NUM,
    _GAMING,
    _ADJUST,
};
/*
Der frueherer `_MAC`-Overlay-Layer ist entfallen (2026-08-04). Er tauschte nur
GUI/Alt auf der Daumenreihe, war nicht persistent und erwischte weder die
LCTL(x)-Kombis im _NAV-Layer (Copy/Paste/Undo) noch dessen Modifier-Homerow.
Ersetzt durch MAC_TOG = CG_TOGG, siehe unten.
*/

/*
ACHTUNG bei Aenderungen an dieser Liste: die Werte sind Keycodes, keine
Layer-Indizes. Ein Custom-Keycode als Argument von TO()/MO()/LSFT_T() ergibt
stillschweigend Unsinn -- genau das war der FN_EXIT-Bug (TO(QWERT) -> TO(3)).
Neue Eintraege deshalb am Ende anhaengen, nicht dazwischen einfuegen.
*/
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
  RV_WORD,
  LR_EXIT,  // raeumt momentane Layer + Layer Lock ab, laesst das Basis-Layout stehen
  DF_PREV   // zurueck auf das Basis-Layout, das vor dem aktuellen aktiv war
};

/*
Board-Keymaps haengen sich hier ein statt process_record_user() selbst zu
definieren: das gemeinsame process_record_user() liegt in wechselbalg.c und
ruft diesen Hook am Ende auf.
*/
bool process_record_keymap(uint16_t keycode, keyrecord_t *record);

/*
"Tippe ich gerade an einem Mac?" -- eine Frage, keine zweite Wahrheit: der
Zustand ist derselbe, den CG_TOGG umschaltet und QMK im EEPROM haelt.
Fuer OLED-Anzeige und die host-abhaengigen Makros in wechselbalg.c.
*/
#ifdef MAGIC_ENABLE
#    define WB_HOST_IS_MAC() (keymap_config.swap_lctl_lgui)
#else
#    define WB_HOST_IS_MAC() false
#endif

// Aliases for readability

/*
Der Layout-Umschalt-Block auf dem ADJUST-Layer, in zwei Reihen:
  P_* (obere Reihe)  = persistent, ueberlebt den Neustart (set_single_persistent_default_layer)
  D_* (Home-Row)     = nur diese Sitzung (DF)
Ein nicht einkompiliertes Layout (siehe WB_LAYOUT_* oben) faellt in beiden
Reihen auf ___NO__ zurueck, statt still auf die falsche Ebene zu zeigen.
*/
#define D_QWERT  DF(_QWERT)
#define P_QWERT  QWERT
#define D__GAME  DF(_GAMING)

/*
Der Ausstieg aus _GAMING (dessen obere rechte Ecke). Nicht persistent, genau
wie der Einstieg per D__GAME: beide fassen nur default_layer_state an.

ACHTUNG: `D__PREV` ist der Rasterplatz in den Keymaps, `DF_PREV` der
Custom-Keycode aus dem Enum oben. Zwei Namen, ein Buchstabe Unterschied --
in den Keymaps steht immer D__PREV.

Ohne WB_DF_PREV (AVR, siehe oben) faellt es auf das alte, feste D_QWERT
zurueck: dann springt die Taste wie bisher hart auf QWERT statt auf das
Layout, von dem aus GAMING betreten wurde.
*/
#ifdef WB_DF_PREV
#    define D__PREV  DF_PREV
#else
#    define D__PREV  D_QWERT
#endif

#ifdef WB_LAYOUT_DVORAK
#    define D_DVORK  DF(_DVORAK)
#    define P_DVORK  DVORAK
#else
#    define D_DVORK  ___NO__
#    define P_DVORK  ___NO__
#endif

#ifdef WB_LAYOUT_COLEMAKDH
#    define D_COLMK  DF(_COLEMAKDH)
#    define P_COLMK  COLEMAK
#else
#    define D_COLMK  ___NO__
#    define P_COLMK  ___NO__
#endif

#ifdef WB_LAYOUT_MINE
#    define D__MINE  DF(_MINE)
#    define P__MINE  MINE
#else
#    define D__MINE  ___NO__
#    define P__MINE  ___NO__
#endif

#ifdef WB_LAYOUT_VOU
#    define D___VOU  DF(_VOU)
#    define P___VOU  VOU
#else
#    define D___VOU  ___NO__
#    define P___VOU  ___NO__
#endif

#define MO__SYM  MO(_SYM)
#define MO__NAV  MO(_NAV)
#define MO__NUM  MO(_NUM)
#define MO__ADJ  MO(_ADJUST)

/*
Mac-Umschaltung: Ctrl und GUI tauschen im aufgeloesten Keycode die Rolle
(CG_TOGG schaltet swap_lctl_lgui *und* swap_rctl_rgui, also beide Haende).

Warum das die vollstaendige Loesung ist und der alte _MAC-Layer nicht:
quantum/keymap_common.c wendet mod_config()/keycode_config() beim Aufloesen
JEDER Taste an -- das erfasst einfache Modifier, Mod-Taps *und* Kombis wie
LCTL(KC_C). Also werden NX_COPY/NX_PAST/NX__CUT/N3_UNDO/N3_REDO/NX_CENT und
die Modifier-Homerow im _NAV-Layer automatisch mit umgestellt. Persistenz
im EEPROM ist eingebaut.

Ctrl geht dabei NICHT verloren: es ist ein Tausch, kein Ersetzen -- der
GUI-Daumen (aussen) sendet dann Ctrl, der Ctrl-Daumen (innen) sendet Cmd.

Braucht MAGIC_ENABLE = yes (siehe rules.mk); ohne das ist die Taste tot.
Nicht erfasst: SEND_STRING(SS_LCTL(...)), also FF_WORD/RV_WORD -- die laufen
am Keymap vorbei und sind in wechselbalg.c stattdessen host-abhaengig.
*/
#define MAC_TOG  CG_TOGG

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

Springt NICHT auf ein festes Layout: LR_EXIT raeumt nur `layer_state` ab
(layer_clear) und laesst `default_layer_state` in Ruhe -- du landest also auf
dem Basis-Layout, das gerade persistent hinterlegt ist. Dazu
layer_lock_all_off(), weil QMKs Layer Lock seinen `locked_layers`-Bitmask
getrennt fuehrt: ein blosses layer_clear() wuerde den Layer abschalten, das
Lock-Bit aber gesetzt lassen -- danach wuerde QK_LLCK auf demselben Layer
ent- statt sperren. Implementierung in wechselbalg.c.

Historie: hier stand TO(QWERT) mit dem *Custom-Keycode* QWERT statt dem
Layer-Index -> TO(3) -> die Taste sprang auf _MINE.

Gilt nicht fuer _GAMING: das wird per DF(_GAMING) als *Default*-Layer betreten
und liegt damit ueber Layer 0, LR_EXIT kommt da nicht raus. GAMING benutzt
D__PREV (siehe oben).
*/
#define FN_EXIT      LR_EXIT
