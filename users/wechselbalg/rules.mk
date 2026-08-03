SRC += wechselbalg.c
#SRC += features/custom_shift_keys.c

# ENCODER_ENABLE = no
ENCODER_MAP_ENABLE = no
EXTRAKEY_ENABLE = yes
# Mouse Keys wieder an: der Grund fuer die Abschaltung (macOS deaktivierte bei
# angeschlossener Maus das Touchpad) ist per macOS-Einstellung erledigt. Ohne
# das waren im _NAV-Layer die komplette rechte Daumenreihe und drei Tasten in
# Reihe 3 stumm -- ohne Compile-Fehler.
MOUSEKEY_ENABLE = yes
NKRO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
KEYBOARD_SHARED_EP = no
UNICODE_ENABLE = yes
TAP_DANCE_ENABLE = yes
LAYER_LOCK_ENABLE = yes
CAPS_WORD_ENABLE = yes
LTO_ENABLE = yes
KEY_OVERRIDE_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
STARTUP_NUMLOCK_ON = yes

# ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
#   SRC += tap_dance.c
# endif

EXTRAFLAGS += -flto
