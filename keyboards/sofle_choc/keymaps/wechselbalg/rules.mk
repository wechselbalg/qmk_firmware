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


# Change keymaps without flashing
# https://caniusevia.com/
# VIA_ENABLE = yes

# MAC_TOG (CG_TOGG) und NK_TOGG brauchen MAGIC_ENABLE (~1 KB). Passt nur in den
# RP2040-Build (schwarzes Board, CONVERT_TO=liatris); der AVR-Build fuer das
# weisse Board hat den Flash dafuer nicht -- dort ist die Taste bis zum
# Controller-Wechsel wirkungslos.
ifeq ($(strip $(CONVERT_TO)),liatris)
    MAGIC_ENABLE = yes
endif
