# MCU name
MCU = atmega32u4

# Build Options
#   change yes to no to disable
#
SPLIT_KEYBOARD = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
ENCODER_ENABLE = yes
CONSOLE_ENABLE = no
EXTRAKEY_ENABLE = yes
TAP_DANCE_ENABLE = yes
NKRO_ENABLE = yes            # USB Nkey Rollover

RGB_MATRIX_ENABLE = no
RGBLIGHT_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

# Reduce firmware size
# https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/#link-time-optimization-and-disabling-core-functionality
EXTRAFLAGS += -flto

# Change keymaps without flashing
# https://caniusevia.com/
# VIA_ENABLE = yes
