SRC += wechselbalg.c
SRC += features/layer_lock.c
SRC += features/caps_word.c
#SRC += features/custom_shift_keys.c

# ENCODER_ENABLE = no
ENCODER_MAP_ENABLE = no
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
NKRO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
KEYBOARD_SHARED_EP = no
UNICODE_ENABLE = yes
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = no
LTO_ENABLE = yes
KEY_OVERRIDE_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
STARTUP_NUMLOCK_ON = yes

ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
  SRC += tap_dance.c
endif

EXTRAFLAGS += -flto
