# MCU name
MCU = atmega32u4

# Build Options
#   change yes to no to disable
#
SPLIT_KEYBOARD = yes
OLED_ENABLE = yes
ENCODER_ENABLE = yes
CONSOLE_ENABLE = no
LTO_ENABLE = yes

RGB_MATRIX_ENABLE = no
RGBLIGHT_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

# Change keymaps without flashing
# https://caniusevia.com/
# VIA_ENABLE = yes

# Caps Word per Shift+Shift kostet 14 Byte (gemessen 2026-08-04). Bei 22 Byte
# frei bliebe danach nichts mehr -- deshalb hier aus. Anschalten heisst: diese
# Zeile loeschen und vorher mit dem Notausgang unten Platz schaffen.
OPT_DEFS += -DWB_NO_BOTH_SHIFTS_CW

# ---------------------------------------------------------------------------
# NOTAUSGANG, falls der Flash ueberlaeuft
#
# Dieses Board ist mit CHORDAL_HOLD + RETRO_TAPPING_PER_KEY am engsten von
# allen: Stand 2026-08-04 sind es 28650/28672 Byte, also **22 Byte frei**. Das
# baut heute, kippt aber sehr wahrscheinlich beim naechsten Upstream-Merge.
#
# Wenn der Build mit "The firmware is too large!" abbricht: die naechste Zeile
# einkommentieren. Dann verhaelt sich das Board wieder wie vor 2026-08-04
# (TAPPING_TERM 600, kein Chordal Hold, kein Retro Tapping) -- siehe
# users/wechselbalg/config.h.
#
# OPT_DEFS und nicht keymaps/.../config.h: build_keyboard.mk haengt die
# Keymap-config.h NACH der Userspace-config.h ein, ein #define dort kaeme zu spaet.
#
# Alternative, wenn das Tippgefuehl wichtiger ist als die RGB-Effekte:
# stattdessen in config.h die RGBLIGHT_EFFECT_*-Animationen ausduennen.
# ---------------------------------------------------------------------------
# OPT_DEFS += -DWB_NO_ADVANCED_TAP_HOLD
