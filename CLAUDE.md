# Wechselbalgs QMK-Fork — Projektkontext

Persönlicher Fork von QMK zur Pflege eigener Tastaturlayouts. Diese Datei ist der
zentrale, mit dem Repo mitreisende Statusspeicher, damit die Arbeit an jedem Rechner
fortgesetzt werden kann.

## 📌 Anweisung an Claude: Diese Datei pflegen

Nach jeder nennenswerten Änderung am Repo den Abschnitt **„Aktueller Stand"** unten
aktualisieren (Datum anpassen, Erledigtes/Offenes fortschreiben) und diese `CLAUDE.md`
**im selben Zug mit den Code-Commits committen und nach `origin mike` pushen**. So ist
der Projektstand auf jedem Rechner aktuell. Das gilt automatisch in jeder Session,
ohne dass Michael es erneut anfordern muss.

## ⚠️ Wichtigste Regel

**Niemals in das offizielle Repo `qmk/qmk_firmware` pushen.** Es wird nur in den
eigenen Fork `origin` (`git@github.com:wechselbalg/qmk_firmware.git`) gepusht.
Absicherung siehe „Setup auf neuem Rechner".

## Branch-Konventionen

- **`mike`** — der einzige Arbeits-Branch. Enthält alle eigenen Keymaps + den
  Userspace `users/wechselbalg`. Hier wird entwickelt und committet.
- **`master` / `develop`** — nur Spiegel des offiziellen QMK, werden ausschließlich
  per Fast-Forward mit `upstream` synchron gehalten, nie eigene Commits.
- Aktualisieren von `mike`: `git checkout mike && git merge upstream/master`
  (bei ChibiOS-Submodul-Konflikten `git checkout upstream/master -- lib/chibios lib/chibios-contrib`).
- `archive/*`-Tags sichern gelöschte Alt-Branches (retro_tap_timeout, sofle_choc,
  lotus58, gmmk_pro) und den Stand vor dem großen Merge.

## Setup auf neuem Rechner

```bash
git clone git@github.com:wechselbalg/qmk_firmware.git
cd qmk_firmware
git checkout mike

# Offizielles QMK als upstream, aber Push technisch unmöglich machen:
git remote add upstream https://github.com/qmk/qmk_firmware.git
git remote set-url --push upstream PUSH_DISABLED
git config remote.pushDefault origin

git submodule update --init --recursive     # oder: make git-submodule
qmk config user.qmk_home=$(pwd)             # qmk-CLI auf diesen Clone zeigen lassen
```

## Tastaturen & Build-Targets

Alle Keymaps heißen `wechselbalg`. Bauen mit `qmk compile -kb <KB> -km wechselbalg`
oder `make <KB>:wechselbalg`:

| Tastatur | Build-Target (`<KB>`) | MCU |
|---|---|---|
| Sofle RGB | `sofle/rev1` | AVR (voll, ~95 %) |
| Sofle Choc (weiß) | `sofle_choc` | AVR — Atmel/atmega32u4 (rechts Elite-C, links Pro Micro) |
| Sofle Choc (schwarz) | `sofle_choc` + `-e CONVERT_TO=liatris` | RP2040 (2× splitkb Liatris) |
| splitkb Kyria | `splitkb/kyria/rev1` | AVR (voll, ~92 %) |
| GMMK Pro ISO | `gmmk/pro/rev1/iso` | STM32 |
| Lotus58 | `tweetydabird/lotus58` | AVR (voll, ~95 %) |
| Keychron K3 Pro ISO RGB | `keychron/k3_pro/iso/rgb` | STM32 |

**Zwei physische Sofle-Choc-Boards, farblich unterschieden:**
- **Weiß** = Original-Bestückung mit Atmel-Chips (rechts Elite-C/Atmel-DFU,
  links Standard-Pro-Micro/Caterina). Build ganz normal ohne `CONVERT_TO`.
- **Schwarz** = auf 2× splitkb-Liatris (RP2040) umgebaut, vorher KMK-Firmware.
  Build mit `qmk compile -kb sofle_choc -km wechselbalg -e CONVERT_TO=liatris`,
  Flash ganz einfach per UF2 (BOOTSEL halten + einstecken → Laufwerk `RPI-RP2`
  erscheint, `.uf2`-Datei draufkopieren, kein Timing-Fenster wie bei AVR).

**Flash-Skript:** `util/wechselbalg/flash.py` (Python, macOS/Linux/Windows)
kompiliert automatisch beide Hälften und führt durch den Flash-Vorgang —
fordert nacheinander zum Anschließen jeder Hälfte auf und wartet dann
selbständig auf das Gerät (nutzt `qmk flash`s eingebaute Wait-Logik).
Enthält auch den `avrdude`-Workaround (siehe unten) für die weiße Sofle Choc.
Boards sind dort in einem Dict eingetragen und leicht um weitere Tastaturen
(Kyria, GMMK Pro, …) erweiterbar. **Claude sollte dieses Skript für alle
künftigen Flash-Vorgänge dieser Boards nutzen statt manueller `make`/`qmk
flash`-Aufrufe.**
```bash
python3 util/wechselbalg/flash.py --list
python3 util/wechselbalg/flash.py sofle_choc_black
python3 util/wechselbalg/flash.py sofle_choc_white --side right
```
Kann den Bootloader nicht selbst auslösen (kein generischer QMK-Software-Weg
dafür) — der physische Taster (oder die `QK_BOOT`-Tastenkombo im Adjust-Layer)
muss weiterhin von Hand gedrückt werden, wenn das Skript dazu auffordert.

`keyboards/keychron/k3_pro` + `keyboards/keychron/bluetooth` sind aus dem
Keychron-Fork einvendort (das K3 Pro existiert nicht im offiziellen QMK) →
bei Upstream-Merges immer die eigene Version behalten.

## Architektur des gemeinsamen Layouts (`users/wechselbalg`)

Ein modulares Grundlayout für alle Tastaturen, damit Layer nur an einer Stelle
gepflegt werden:

- **`wrappers.h`** — pro Layer ein Makroblock (z. B. `5_QWERTZ_L`,
  `7_THUMBS`), den jede Keymap über eine board-spezifische Layout-Makro einsetzt.
  Achtung: die Wrapper-Blocknamen sind fehleranfällig bei Suchen/Ersetzen
  (Kommentar in der Datei beachten).
- **Layer** (`wechselbalg.h`): Basis-Layouts `_QWERT`/`_MINE`/`_VOU`/… →
  Momentary `_SYM`/`_NAV`/`_NUM` → `_ADJUST` → `_MAC`.
- **`_MAC`** — Overlay über allen Layern, nur Daumenreihe belegt (Cmd⇄Opt
  getauscht), Rest transparent → funktioniert auf jeder Basis. Umschalten mit
  `TG__MAC` auf dem Adjust-Layer; OLEDs zeigen „MAC". Encoder-Handler maskieren
  das `_MAC`-Bit. **Toggle ist nicht persistent** (startet nach Reconnect im PC-Modus).
- **Tap Dance** zentral in `tap_dance.h` / `tap_dance_actions.h` (per Include in
  jede Keymap eingebunden, kein separates `.c`).
- **Layouts**: Neo2-basiert (`keymap_neo2.h`), deutsche Sonderzeichen
  (`keymap_german_ia.h`). Umlaute setzen Caps Word fort (Hook in `wechselbalg.c`).
- **Features** (`rules.mk`): Layer Lock + Caps Word aus dem QMK-Core
  (`LAYER_LOCK_ENABLE`/`CAPS_WORD_ENABLE`); `F_LLOCK` = Alias für `QK_LLCK`.
  `LTO_ENABLE = yes` (die AVR-Boards sind flash-eng).

## Aktueller Stand (Stand: 2026-08-03, Liatris-Migration ergänzt)

**Fertig:**
- Fork aufgeräumt (nur noch master/develop/mike), auf aktuellen QMK-Stand gemergt.
- Alle 6 Boards kompilieren.
- Layer Lock + Caps Word auf QMK-Core umgestellt; altes `features/layer_lock.c`
  und Duplikat `users/wechselbalg_` entfernt.
- `_MAC`-Overlay-Layer für alle Wrapper-Keymaps hinzugefügt.
- Lotus58 vom eigenen Port auf das offizielle `tweetydabird/lotus58` migriert.
- Sofle Choc (weiß, Atmel/AVR) auf aktuellen Stand geflasht und getestet (beide Hälften):
  - Linke Hälfte, äußerste Daumentaste: Del/Num → Enter/Num (`NUM_ENT`).
  - Rechte Hälfte, zweite Daumentaste von links: Enter/Shift → Space/Shift (`RFT_SPC`).
  - `_NAV`-Layer: reine Enter-Taste an der bisher freien Stelle direkt links
    neben der Shift-Position (unterste Reihe, vorletzte rechte Spalte) hinzugefügt.
    (`_NUM`-Layer an der gleichen Stelle bleibt unverändert bei `;`.)
  - Mouse Keys global deaktiviert (`MOUSEKEY_ENABLE = no` in `users/wechselbalg/rules.mk`)
    — kollidierten am Mac mit dem Touchpad.
  - **Wichtig**: die zwei Hälften haben unterschiedliche Controller/Bootloader:
    rechte Hälfte = Elite-C (Atmel-DFU, `make sofle_choc:wechselbalg:dfu`),
    linke Hälfte = Standard-Arduino-Pro-Micro (Caterina/seriell,
    `make sofle_choc:wechselbalg:flash`, Bootloader nur ~8s aktiv → Flash-Befehl
    vor dem Auslösen des Bootloaders starten, er pollt automatisch auf den
    neuen seriellen Port). Details in Claude-Memory `sofle-choc-dfu-bootloader`.
  - Lokales `avrdude` (Homebrew, 8.2) hatte eine kaputte `avrdude.conf`
    (mehrere veraltete FTDI-Programmer-Blöcke + fehlende `prog_modes` bei
    `avr109`/`ATmega32U4` verursachen Parse-/Kompatibilitätsfehler) → Workaround
    ist eine minimale eigene Config statt der System-`avrdude.conf`
    (siehe Claude-Memory `sofle-choc-dfu-bootloader` für Details/Pfad).
- Sofle Choc (schwarz, 2× splitkb Liatris/RP2040) von KMK auf QMK migriert,
  gleicher `wechselbalg`-Keymap wie das weiße Board, per `CONVERT_TO=liatris`
  kompiliert und beide Hälften per UF2 geflasht:
  - `SPLIT_USB_DETECT` aktiv → beliebige Hälfte kann per USB Master sein.
  - Zusätzlicher Draht (Elite-C-Pin B7 = GP12) über den 4. TRRS-Kanal wird für
    echtes Full-Duplex-Serial genutzt (`SERIAL_USART_FULL_DUPLEX`, TX=GP1/D2,
    RX=GP12/B7, `SERIAL_USART_PIN_SWAP` da Verkabelung durchverbunden statt
    gekreuzt, `SERIAL_PIO_USE_PIO1` wegen WS2812 auf PIO0).
  - **Wichtig gelernt**: `SPLIT_USB_DETECT` (dynamischer Master) und Händigkeit
    (welche Hälfte "links"/"rechts" für die Matrix-Pins ist) sind zwei getrennte
    Dinge. Ohne eigene Händigkeits-Angabe fallen beide Hälften auf "ich bin
    links" zurück → gespiegelte/vertauschte Tasten. Fix: `EE_HANDS` + je
    einmalig mit `-bl uf2-split-left` / `-bl uf2-split-right` flashen.
  - Alle Liatris-spezifischen Config-Zeilen stehen in
    [keyboards/sofle_choc/keymaps/wechselbalg/config.h](../../../keyboards/sofle_choc/keymaps/wechselbalg/config.h)
    hinter `#ifdef CONVERT_TO_LIATRIS`, damit das weiße (AVR-)Board unangetastet bleibt.
  - Details siehe Claude-Memory `sofle-choc-liatris-rp2040`.

**Offen / vor dem Flashen prüfen:**
- **K3 Pro**: MINE-Ebene enthält Annahmen (neu belegte `+`-Taste, NUBS = `MO__NUM`);
  `_TEST`-Layer ist von keiner Taste erreichbar. Vor Nutzung sichten.
- **Lotus58-Keymap** ist noch eigenständig (nicht auf das Wrapper-System umgestellt)
  → hat daher noch **keinen** `_MAC`-Layer. Nächster natürlicher Schritt.
- K3-Pro-Varianten ansi/jis/white bleiben ungebaut (alte LED-Tabellen-Makros).
- Idee: Mac-Variante des `_NAV`-Layers (Wort-Sprünge Opt+Pfeil statt Ctrl+Pfeil).
- Anderen Wrapper-Keymaps (Kyria, GMMK Pro, K3 Pro) fehlt noch der Test, ob die
  globale Mouse-Keys-Deaktivierung dort Layer-Belegungen (`MS_*` im `_NAV`-Layer)
  stumm werden lässt (keine Compile-Fehler, aber Tasten tun dann nichts mehr).
