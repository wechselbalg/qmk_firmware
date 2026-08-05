#ENCODER_MAP_ENABLE = yes
STARTUP_NUMLOCK_ON = yes

# MAC_TOG (CG_TOGG) und NK_TOGG brauchen MAGIC_ENABLE; auf STM32 ist Platz.
MAGIC_ENABLE = yes

# Nur die Layouts anlegen, die dieses Board auch wirklich hat: QWERTZ (immer)
# und Colemak-DH. Ohne diese Zeile gilt der Default aus wechselbalg.h -- auf
# Nicht-AVR "alle vier" -- und _DVORAK/_COLEMAKDH/_VOU stuenden als Layer im
# Enum, ohne dass die Keymap sie definiert: 192 Byte KC_NO pro Stueck.
#
# Das war nicht nur Verschnitt, sondern eine Falle: P_DVORK/P_COLMK/P___VOU auf
# _ADJUST (physisch Q/W/E) rufen set_single_persistent_default_layer() -- ein
# Druck darauf machte das Board *dauerhaft* tot, ueber Neustarts hinweg. Nicht
# einkompilierte Layouts fallen jetzt auf ___NO__ zurueck (wechselbalg.h).
#
# Umstellen ist eine Zeile: WB_LAYOUT_MINE statt WB_LAYOUT_COLEMAKDH (der
# _MINE-Layer steht weiterhin in der keymap.c, nur hinter seinem #ifdef).
OPT_DEFS += -DWB_LAYOUT_COLEMAKDH
