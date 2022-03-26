# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no     # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes            # USB Nkey Rollover
BACKLIGHT_ENABLE = no      # Enable keyboard backlight functionality
AUDIO_ENABLE = no           # Audio output
RGBLIGHT_ENABLE = no       # Enable keyboard RGB underglow
OLED_ENABLE = yes
# OLED_DRIVER = SSD1306    # OLED display
SPLIT_KEYBOARD = yes
TAP_DANCE_ENABLE = yes
AUTO_SHIFT_ENABLE = no
ENCODER_ENABLE = yes
