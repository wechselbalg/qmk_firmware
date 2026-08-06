# Kein ENCODER_MAP_ENABLE: der Encoder laeuft ueber encoder_update_user() in der
# keymap.c, weil die Helligkeit auf _ADJUST die *_noeeprom-Varianten braucht.
# Begruendung steht dort am Handler.
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

# Die abgeleitete Per-Key-Farbsprache (C8) -- bis 2026-08-06 lief dieses Board
# noch auf QMKs Standard-Animationen. Die Farbsprache ist ausdruecklich opt-in,
# damit sie kein Board ungefragt umfaerbt; hier ist sie jetzt gefragt (Michael,
# 2026-08-06). Anders als beim K3 Pro bleiben die Animationen einkompiliert --
# Begruendung in der config.h daneben (Lichtbalken).
WB_RGB_LANGUAGE = yes

# Der seitliche Lichtbalken (16 LEDs ohne Matrixposition) als umschaltbare
# Statusanzeige -- Taste A_SIDEBR auf _ADJUST, persistent im EEPROM.
# Vorgabe bleibt die Animation (Michael, 2026-08-06).
WB_SIDEBAR_STATUS = yes

# Kein Tri-Layer auf diesem Board: nach _ADJUST fuehrt allein die MO__ADJ-Taste
# (physisch End). Ohne das wuerde die _NUM-Taste rot leuchten, solange _NAV
# gehalten wird -- und damit einen Weg nach _ADJUST behaupten, den es hier
# nicht gibt. Dass es keinen Tri-Layer gibt, ist entschieden (Michael,
# 2026-08-06), nicht versehentlich.
OPT_DEFS += -DWB_NO_TRI_LAYER_HINT
