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

# Change keymaps without flashing
# https://caniusevia.com/
# VIA_ENABLE = yes

# Beide Sofle-Choc-Boards teilen sich diese Datei, deshalb haengt alles, was
# nicht auf den ATmega32u4 passt, an CONVERT_TO=liatris:
#
#   MAGIC_ENABLE       ~1 KB   traegt MAC_TOG (CG_TOGG) und NK_TOGG
#   RGB_MATRIX_ENABLE  ~3,3 KB gemessen 2026-08-04: auf dem weissen Board
#                              3270 Byte zu gross
#
# Das weisse Board behaelt RGBLIGHT mit den handgepflegten SET_*-Makros im
# keymap.c -- beide Welten koexistieren dort per #ifdef.
ifeq ($(strip $(CONVERT_TO)),liatris)
    MAGIC_ENABLE = yes

    RGB_MATRIX_ENABLE = yes
    RGBLIGHT_ENABLE = no
    # Die abgeleitete Farbsprache (users/wechselbalg/rgb_language.c)
    WB_RGB_LANGUAGE = yes
    # Die Status-LED auf dem Liatris-NeoPixel (users/wechselbalg/status_led.c)
    WB_STATUS_LED = yes
else
    RGB_MATRIX_ENABLE = no
    RGBLIGHT_ENABLE = yes
endif
