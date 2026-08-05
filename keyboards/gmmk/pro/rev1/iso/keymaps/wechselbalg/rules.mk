#ENCODER_MAP_ENABLE = yes
STARTUP_NUMLOCK_ON = yes

# MAC_TOG (CG_TOGG) und NK_TOGG brauchen MAGIC_ENABLE; auf STM32 ist Platz.
MAGIC_ENABLE = yes

# Nur die Layouts anlegen, die dieses Board auch wirklich hat: QWERTZ (immer)
# und Colemak-DH. Ohne diese Zeile gilt der Default aus wechselbalg.h -- auf
# Nicht-AVR "alle vier" -- und _DVORAK/_COLEMAKDH/_MINE/_VOU stuenden als Layer
# im Enum, ohne dass die Keymap sie definiert: 192 Byte KC_NO pro Stueck.
#
# Das war nicht nur Verschnitt, sondern eine Falle. Auf _ADJUST lagen acht
# scharfe Tasten (physisch Q/W/E/R und A/S/D/F): P_DVORK/P_COLMK/P___VOU/P__MINE
# rufen set_single_persistent_default_layer() -- ein Druck darauf machte das
# Board *dauerhaft* tot, ueber Neustarts hinweg; die D_*-Reihe (DF()) bis zum
# Ausstecken. Nicht einkompilierte Layouts fallen jetzt auf ___NO__ zurueck
# (wechselbalg.h), aus den Falltueren werden also tote Tasten.
#
# Umstellen ist eine Zeile: WB_LAYOUT_MINE statt WB_LAYOUT_COLEMAKDH -- dann
# muss aber auch der Layer-Block in der keymap.c mitgezogen werden, dort steht
# bisher nur _QWERT und _COLEMAKDH.
OPT_DEFS += -DWB_LAYOUT_COLEMAKDH
