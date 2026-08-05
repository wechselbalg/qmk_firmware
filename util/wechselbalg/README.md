# Flash-Skript für die wechselbalg-Boards

`flash.py` kompiliert und flasht die Split-Tastaturen automatisch — kompiliert
beide Hälften, fordert nacheinander zum Anschließen jeder Seite auf und
wartet dann selbständig auf das Gerät (nutzt `qmk flash`s eingebaute
Bootloader-Erkennung, egal ob UF2-Laufwerk, Caterina-Port oder DFU-Gerät).

Läuft auf macOS, Linux und Windows — vorausgesetzt `qmk`-CLI und die
Build-Toolchain sind installiert (siehe Haupt-[CLAUDE.md](../../CLAUDE.md),
Abschnitt „Setup auf neuem Rechner").

## Benutzung

```bash
python3 util/wechselbalg/flash.py --list                        # verfügbare Boards anzeigen
python3 util/wechselbalg/flash.py sofle_choc_black               # beide Hälften nacheinander
python3 util/wechselbalg/flash.py sofle_choc_white --side right   # nur eine Hälfte
```

Das Skript fragt vor jeder Hälfte per Prompt nach und startet dann den
Flash-Befehl, der selbständig auf das Gerät wartet. Wenn die Meldung kommt,
den Boot-Taster der genannten Hälfte drücken (siehe unten) — der Rest läuft
automatisch.

## Boot-Modus auslösen

Es gibt **keinen Software-Weg**, den Bootloader vom Skript aus fernzustarten
— das muss physisch am jeweiligen Board passieren:

- **Weiße Sofle Choc, linke Hälfte (Pro Micro/Caterina):** Boot-Taster
  gedrückt halten, dabei USB einstecken. Bootloader bleibt nur **~8 Sekunden**
  aktiv — das Skript startet den Flash-Befehl deshalb schon *vor* der
  Aufforderung, damit es rechtzeitig erkennt.
- **Weiße Sofle Choc, rechte Hälfte (Elite-C/Atmel-DFU):** genauso, kein enges
  Zeitfenster.
- **Schwarze Sofle Choc (2× Liatris/RP2040):** BOOTSEL-Taste gedrückt halten,
  dabei USB einstecken. Kein Zeitfenster, Board bleibt im Bootloader bis
  geflasht wird.
- **K3 Pro und GMMK Pro (beide STM32):** **Esc** gedrückt halten, dabei USB
  einstecken — Bootmagic, hängt an keiner Keymap und an keinem Layer, sondern
  nur an der Matrix. Meldet sich als `STM32 BOOTLOADER` (0483:df11). Setzt
  dabei das EEPROM zurück; genau das macht diesen Weg zum Rettungsanker, wenn
  das Board je auf einem toten Layer landet. Das Skript druckt die Anleitung
  vor dem Flashen selbst noch einmal aus.
- Alternativ auf laufender Tastatur, statt Taster:
  - **Splits (Tri-Layer):** `_NUM`+`_NAV`-Daumentasten gleichzeitig halten
    (aktiviert `_ADJUST`), dann „B"-Taste (linke Hälfte, unterste Reihe)
    drücken → `QK_BOOT`.
  - **K3 Pro / GMMK Pro (kein Tri-Layer):** **End** halten — das ist dort die
    dedizierte `MO__ADJ`-Taste — dann Esc, Backspace oder B.

## Neues Board ergänzen

In `flash.py` im Dict `BOARDS` einen neuen Eintrag hinzufügen (Vorlagen für
Kyria/GMMK Pro stehen als Kommentar direkt im Skript):

```python
"mein_board": {
    "label": "Anzeigename",
    "kb": "qmk-keyboard-pfad",
    "km": "wechselbalg",
    "convert_to": None,        # oder z.B. "liatris" bei Controller-Umbau
    "sides": {                 # oder None bei nicht gesplitteten Boards
        "left":  {"bootloader": None, "prompt": "..."},
        "right": {"bootloader": None, "prompt": "..."},
    },
},
```

`bootloader` ist der `-bl`-Wert für `qmk flash` (`None` = Standard-Bootloader
des Boards verwenden). Für AVR-Boards mit dem hier bekannten
avrdude.conf-Problem kann `"avrdude_conf_workaround": True` bei der
betroffenen Seite gesetzt werden (siehe unten).

## Warum liegt hier eine eigene `avrdude.conf`?

`avrdude_minimal_atmega32u4.conf` ist eine minimale, funktionierende
Alternative zur System-`avrdude.conf` von Homebrew (Stand avrdude 8.2), die
auf diesem Rechner für `avr109`/`ATmega32U4` kaputt ist (mehrere veraltete
Programmer-Blöcke lösen Parse-Fehler aus). Das Skript bindet sie automatisch
per `AVRDUDE_PROGRAMMER`-Umgebungsvariable ein, wenn eine Board-Seite
`"avrdude_conf_workaround": True` gesetzt hat und die Datei existiert — sonst
bleibt sie folgenlos. Betrifft aktuell nur die weiße Sofle Choc (linke
Hälfte, Caterina/avr109).
