# Diese Keymap ist noch eigenstaendig (nicht auf das Wrapper-System aus
# users/wechselbalg umgestellt) und bringt ihre eigenen sechs Basis-Layouts mit
# -- die WB_LAYOUT_*-Reduktion in wechselbalg.h greift hier also nicht, und der
# ATmega32u4-Flash reicht fuer CHORDAL_HOLD + RETRO_TAPPING_PER_KEY nicht
# (524 Bytes drueber, gemessen 2026-08-04). Damit bleibt es beim alten
# Tap-Hold-Verhalten (TAPPING_TERM 600).
#
# Per OPT_DEFS und nicht per keymaps/.../config.h: build_keyboard.mk haengt die
# Keymap-config.h NACH der Userspace-config.h ein (Zeile 459 vs. 431), ein
# #define dort kaeme also zu spaet.
#
# Faellt weg, sobald die Keymap auf die Wrapper umgestellt oder das Board auf
# einen groesseren Controller umgezogen ist.
OPT_DEFS += -DWB_NO_ADVANCED_TAP_HOLD
