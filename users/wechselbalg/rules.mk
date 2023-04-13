SRC += wechselbalg.c
# SRC += custom_shift_keys.c

# ENCODER_ENABLE = no
ENCODER_MAP_ENABLE = no
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
NKRO_ENABLE = yes
CONSOLE_ENABLE = no
KEYBOARD_SHARED_EP = no
UNICODE_ENABLE = yes
TAP_DANCE_ENABLE = yes
# KEY_OVERRIDE_ENABLE = yes

ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
  SRC += tap_dance.c
endif

EXTRAFLAGS += -flto
