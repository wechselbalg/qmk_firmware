SPLIT_KEYBOARD = yes
OLED_DRIVER_ENABLE = yes
ENCODER_ENABLE = yes
EXTRAKEY_ENABLE = yes

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

# Reduce firmware size
# https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/#link-time-optimization-and-disabling-core-functionality
EXTRAFLAGS += -flto

# Change keymaps without flashing
# https://caniusevia.com/
# VIA_ENABLE = yes
