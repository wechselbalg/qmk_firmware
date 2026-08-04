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
| ~~Lotus58~~ | `tweetydabird/lotus58` | AVR — **stillgelegt, siehe unten** |
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
  Momentary `_SYM`/`_NAV`/`_NUM` → `_ADJUST`.
- **Mac-Modus** — `MAC_TOG` (= QMKs `CG_TOGG`) auf dem Adjust-Layer tauscht
  Ctrl⇄GUI im aufgelösten Keycode, auf beiden Händen und **persistent im
  EEPROM**; OLEDs zeigen „MAC"/„PC". Kein Layer mehr — der frühere
  `_MAC`-Overlay ist am 2026-08-04 entfallen, siehe „KMK→QMK-Angleichung".
  Braucht `MAGIC_ENABLE = yes`, das nur die Nicht-AVR-Boards haben.
- **`process_record_keymap()`** — Board-Keymaps hängen sich hier ein; das
  gemeinsame `process_record_user()` liegt in `wechselbalg.c`. Eine Keymap darf
  `process_record_user` **nicht** selbst definieren (Doppel-Symbol).
- **Tap Dance** zentral in `tap_dance.h` / `tap_dance_actions.h` (per Include in
  jede Keymap eingebunden, kein separates `.c`).
- **Layouts**: Neo2-basiert (`keymap_neo2.h`), deutsche Sonderzeichen
  (`keymap_german_ia.h`). Umlaute setzen Caps Word fort (Hook in `wechselbalg.c`).
- **Features** (`rules.mk`): Layer Lock + Caps Word aus dem QMK-Core
  (`LAYER_LOCK_ENABLE`/`CAPS_WORD_ENABLE`); `F_LLOCK` = Alias für `QK_LLCK`.
  Caps Word geht zusätzlich per **beide Shifts halten und loslassen**
  (`BOTH_SHIFTS_TURNS_ON_CAPS_WORD`, überall außer sofle/rev1).
  `LTO_ENABLE = yes` (die AVR-Boards sind flash-eng).
- **Flash-Gating im Userspace**: `WB_LAYOUT_*` (welche Basis-Layouts),
  `WB_JIGGLER`/`WB_DF_PREV` (C4/C6, auf AVR aus),
  `WB_NO_ADVANCED_TAP_HOLD` (Lotus58), `WB_NO_BOTH_SHIFTS_CW` (sofle/rev1).
  Merkregel: `rules.mk`-Schalter mit `?=` im Userspace (Keymap wird *vorher*
  gelesen), `config.h`-Defines per `OPT_DEFS` im Board (Keymap-config.h wird
  *nachher* gelesen).

## Aktueller Stand (Stand: 2026-08-04, Schritt 3 der KMK-Angleichung)

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
  - Mouse Keys waren zwischenzeitlich global deaktiviert (Touchpad-Konflikt am Mac);
    seit 2026-08-04 wieder `MOUSEKEY_ENABLE = yes`, siehe „KMK→QMK-Angleichung" unten.
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
- **K3 Pro — gesichtet und bereinigt 2026-08-04.** Zwei der früher hier
  notierten Bedenken waren gegenstandslos, der Rest ist erledigt:
  - ~~„neu belegte `+`-Taste"~~ — **kein Unterschied**: `DE_PLUS` *ist*
    `KC_RBRC` (`keymap_german.h:61`). MINE-Zeile und QWERT-Zeile sind identisch.
  - ~~„NUBS = `MO__NUM` ist eine MINE-Annahme"~~ — **stimmt nicht**: das kommt
    aus den gemeinsamen 7-wide-Wrappern und gilt für QWERTZ genauso.
  - **`_TEST`-Layer gelöscht.** War von keiner Taste erreichbar und in diesem
    Build redundant (`BT_HST1..3` = `_______`, weil Bluetooth aus ist; RGB und
    `NK_TOGG` liegen auf `_ADJUST`). Bei aktiviertem Bluetooth-Build gehören
    `BT_HST1..3`/`BAT_LVL` auf `_ADJUST` — alte Belegung steht in der
    git-Historie.
  - **`RN_CODE` → `KC_MAIL`.** Die Taste hatte auf diesem Board nie einen
    Handler und tat nichts.
  - **MINE: ISO-Extratasten senden jetzt, was auf ihnen steht** — wie auf allen
    anderen Basis-Layouts. ⚠️ **Rückgängig zu machen, falls es stört** (die
    alten Werte stehen als Kommentar direkt an den Zeilen):
    - Zahlenreihe rechts der 0: `DE_MINS, DE_GRV` → `DE_SS, DE_ACUT`.
      Vorher war `-` eine Dublette (liegt auf MINE schon auf `RFT_MIN`) und
      `DE_GRV` = `S(DE_ACUT)` ein **Dead Key**.
    - ISO-`#`-Taste: `SYM_ACU` → `SYM_HSH`, sendet also `#` statt `´`.
      Der Akut ist seitdem wieder auf seiner eigenen Taste erreichbar.
- K3-Pro-Varianten ansi/jis/white bleiben ungebaut (alte LED-Tabellen-Makros).
- Idee: Mac-Variante des `_NAV`-Layers (Wort-Sprünge Opt+Pfeil statt Ctrl+Pfeil).
  Wird durch C1 (Ctrl⇄GUI-Swap) **wichtiger**, nicht überflüssig — siehe dort.

---

# KMK→QMK-Angleichung (laufend, Stand 2026-08-04 — Schritt 3 erledigt)

Parallel läuft unter `/Users/mike/dev/kmkfw` ein KMK-Port derselben Sofle Choc
(schwarz/Liatris). Dort sind seit Juli 2026 etliche Funktions- und
Belegungsänderungen entstanden, die hier nachgezogen werden sollen. Maßgeblich
ist dort der **Haupt-Worktree `kmkfw`** (`kmkfw-rebuild` ist ein Worktree
desselben Repos und hängt hinterher). KMKs eigene `CLAUDE.md` ist die
ausführliche Quelle für die Begründungen.

**Diese Liste bis zur vollständigen Abarbeitung pflegen.**

## Erledigt (2026-08-04, Schritt 1 — Bugfixes + entschiedene Kleinigkeiten)

- **`FN_EXIT` sprang auf `_MINE`.** `#define FN_EXIT TO(QWERT)` benutzte den
  *Custom-Keycode* `QWERT` aus `enum CustomKeys` (`SAFE_RANGE+3` = `0x7E43`)
  statt des Layer-Index. `TO()` maskiert mit `& 0x1F` → `TO(3)` = `_MINE`.
  Betraf die linke obere Ecke von SYM/NUM/NAV/ADJUST auf allen Boards.
  Jetzt `TO(_QWERT)`.
  **Merke:** QMK führt `default_layer_state` getrennt von `layer_state`, deshalb
  räumt `TO(_QWERT)` nur die momentanen/getoggelten Layer weg (auch `_MAC`) und
  lässt das gewählte Basis-Layout stehen — Layer 0 liegt dann wirkungslos
  darunter. Das ist nah genug an KMKs `KC.LCLR`, ein eigener Keycode ist nicht nötig.
- **GAMING-Ausstieg war wirkungslos.** `_GAMING` wird per `DF(_GAMING)` als
  *Default*-Layer betreten und liegt damit über Layer 0 — `FN_EXIT` kam da nicht
  raus. Jetzt `D_QWERT` (sofle_choc, sofle/rev1, kyria). Offen: `DF_PREV`, siehe C6.
- **Linker Ctrl-Daumen: `CTL_ENT` → `KC_LCTL`** (auch in der `_MAC`-Variante).
  Letzter offener Teil der Thumb-Umstellung; KMK hat das am 2026-07-31 gemacht
  (Commit `fd73595`), weil der Mod-Tap beim Halten von Ctrl Streu-Enter erzeugte.
  Enter sitzt auf `NUM_ENT`, Ctrl+Enter/Shift+Enter sind jetzt Daumen-Chords.
- **`MOUSEKEY_ENABLE = yes`** (war `no`). Grund für die Abschaltung war der
  macOS-Touchpad-Konflikt, den Michael inzwischen per Systemeinstellung gelöst
  hat. Vorher waren im `_NAV`-Layer die komplette rechte Daumenreihe
  (`MS_BTN2/BTN1/LEFT/DOWN/RGHT`) und drei Tasten in Reihe 3 stumm.
- **Liatris-Power-LED (GP24) wird beim Boot ausgeschaltet.** Invertiert
  (HIGH = aus), leuchtet undriven auf voller Helligkeit. In
  `keyboard_post_init_user()` hinter `#ifdef CONVERT_TO_LIATRIS`; **numerisch
  `24U` statt `GP24`**, weil `CONVERT_TO=liatris` das Pro-Micro-Pinmapping
  (`promicro_to_rp2040_ce/_pin_defs.h`) einzieht und die `GPxx`-Namen dort
  nicht existieren. Dafür wanderte `keyboard_post_init_user()` aus dem
  `#ifdef RGBLIGHT_ENABLE`-Block heraus.
- **`TAPPING_FORCE_HOLD` entfernt** (sofle_choc keymap config.h): existiert seit
  QMK 2023-02 nicht mehr, war wirkungslos und widersprach dem `QUICK_TAP_TERM 80`
  aus `users/wechselbalg/config.h`, das es abgelöst hat.
- Tri-Layer-Argumentreihenfolge in beiden `#ifdef`-Zweigen vereinheitlicht (kosmetisch).
- Alle 6 Boards + der Liatris-Build kompilieren. AVR-Flash danach: sofle_choc 96 %,
  sofle/rev1 96 %, kyria 92 %, lotus58 95 %.

## Erledigt (2026-08-04, Schritt 2 + C1) — noch nicht auf Hardware getestet

### Userspace-Refactor: `process_record_keymap()`

`process_record_user()` lag als Kopie in **jeder** Board-Keymap (dieselben
QWERT/DVORAK/…/DBRACES/FF_WORD/RV_WORD-Fälle). Es liegt jetzt einmal in
[users/wechselbalg/wechselbalg.c](users/wechselbalg/wechselbalg.c) und ruft am
Ende den schwachen Hook `process_record_keymap()` auf, den die Boards für ihr
Eigenes überschreiben. Alle sechs Keymaps sind entsprechend umbenannt —
**wichtig**: eine Board-Keymap darf `process_record_user` nicht mehr selbst
definieren, sonst Doppel-Symbol beim Linken.

### C1 — `_MAC`-Layer raus, `MAC_TOG` = `CG_TOGG`

- `_MAC` ist aus dem Layer-Enum, aus `wrappers.h` (MAC-Daumenreihen) und aus
  allen fünf Board-Keymaps entfernt, ebenso die `& ~(1 << _MAC)`-Masken im
  Encoder-Handler und der OLED-`case`.
- `MAC_TOG` (= `CG_TOGG`) sitzt auf dem Platz von `TG__MAC` in `ADJUST_L0`.
  Tauscht Ctrl⇄GUI auf **beiden** Händen (`process_magic.c` setzt
  `swap_lctl_lgui` *und* `swap_rctl_rgui`), persistent im EEPROM.
- **Ctrl geht nicht verloren** — es ist ein Tausch: der GUI-Daumen (außen)
  sendet dann Ctrl, der Ctrl-Daumen (innen) sendet Cmd.
- Deckt automatisch mit ab, was der `_MAC`-Layer nie erreichte: die
  `LCTL(x)`-Kombis (`NX_COPY`/`NX_PAST`/`NX__CUT`/`N3_UNDO`/`N3_REDO`/`NX_CENT`),
  Mod-Taps und die Modifier-Homerow im `_NAV`-Layer. Grund:
  `quantum/keymap_common.c` wendet `mod_config()`/`keycode_config()` beim
  Auflösen **jeder** Taste an.
- OLED zeigt statt des Layer-`case` jetzt eine eigene Zeile `MAC` / `PC` aus
  `WB_HOST_IS_MAC()` (= `keymap_config.swap_lctl_lgui`).
- **Wo die Taste sitzt** (seit 2026-08-04 auf der **C-Position**): untere
  Buchstabenreihe, vierte Taste von links — dort, wo auf QWERTZ das **C**
  liegt. Merksatz: „C wie Cmd", und genau das macht `CG_TOGG` aus der
  Ctrl-Taste. Hinkommen: beide inneren Daumen halten (links `NUM_ENT`, rechts
  `NAV_BSC`) → Tri-Layer `_ADJUST` → C tippen, Daumen dabei gehalten lassen.
  Auf den ISO-Boards (GMMK Pro, K3 Pro) steht sie im Block `ADJUST__3` auf
  Position **5** statt 4, weil deren Reihe zusätzlich die NUBS-Taste hat.
  Kontrolle: das OLED springt von `PC` auf `MAC`. Falls die Taste nichts tut,
  zuerst prüfen ob `MAGIC_ENABLE` im Build ist:
  `tr ' ' '\n' < .build/obj_<target>/cflags.txt | grep -x -- -DMAGIC_ENABLE`.
- **Ein Ctrl⇄Alt-Magic-Keycode existiert in QMK nicht** (nur Ctrl⇄CapsLock,
  Alt⇄GUI, Ctrl⇄GUI, Esc⇄CapsLock, Grave⇄Esc, Backslash⇄Backspace, no_gui) —
  wird hier aber auch nicht gebraucht, siehe oben.

### `FN_EXIT` → `LR_EXIT`

Springt **nicht** mehr auf ein festes Layout, sondern räumt nur `layer_state`
ab (`layer_clear()`) und lässt `default_layer_state` stehen → du landest auf
dem Basis-Layout, das gerade persistent hinterlegt ist. Dazu
`layer_lock_all_off()`: QMKs Layer Lock führt seinen `locked_layers`-Bitmask
**getrennt**, ein bloßes `layer_clear()` würde den Layer abschalten und das
Lock-Bit gesetzt lassen — danach würde `QK_LLCK` auf demselben Layer ent-
statt sperren.

### Wortsprünge host-abhängig statt Mac-`_NAV`-Layer

`FF_WORD`/`RV_WORD` sind `SEND_STRING(SS_LCTL(...))` und laufen am Keymap
vorbei, `CG_TOGG` erreicht sie also nicht. Sie sind aber ohnehin kein Fall für
einen Modifier-*Tausch*: wortweise Bewegung ist Ctrl+Pfeil (Windows/Linux) vs.
Opt+Pfeil (macOS) — ein **anderer** Modifier. Deshalb `wb_word_jump()` mit
einer echten Fallunterscheidung auf `WB_HOST_IS_MAC()`. Ein eigener
Mac-`_NAV`-Layer ist damit vom Tisch; falls später mehr Tasten host-abhängig
werden (Home/End, PrintScreen), nach demselben Muster erweitern.

### Schritt 2 — Tap-Hold

- `TAPPING_TERM` 600 → **150**, `CHORDAL_HOLD`, `RETRO_TAPPING_PER_KEY` mit
  eigenen Zeitfenstern (`get_retro_tapping()` in `wechselbalg.c`: 1000 ms für
  Layer-Taps, 500 ms für Mod-Taps — baut KMKs `retro_tap_timeout` nach, das QMK
  nicht kennt; der Callback wird beim *Release* ausgewertet, die Press-Zeit
  schreiben wir selbst mit).
- `chordal_hold_layout` mit `'*'` für die zehn Daumentasten und die beiden
  Encoder-Klicks in sofle_choc und sofle/rev1, geschrieben mit `LAYOUT_wrapper`
  (also in visueller Reihenfolge — der Indexfehler, den KMK dort hatte, ist so
  strukturell ausgeschlossen).
- `DUMMY_MOD_NEUTRALIZER_KEYCODE` **nicht** gesetzt: es schützt davor, dass ein
  Retro-Tap auf einem GUI/Alt-Mod-Tap am Host ein Menü öffnet — die Mod-Taps
  dieses Layouts sind aber nur Shift und AltGr. Bei einem neuen GUI/Alt-Mod-Tap
  wieder aufnehmen.

### AVR: alternative Layouts reduziert, Features dafür rein (2026-08-04)

Erster Ansatz war, den AVR-Boards die neuen Tap-Hold-Features ganz zu
verweigern. Stattdessen jetzt nach KMKs Vorbild: **weniger Basis-Layouts,
dafür die Features.** Jedes alternative Layout kostet ~120 Byte
(60 Tasten × 2 Byte), vier davon also knapp 500.

**`WB_LAYOUT_DVORAK` / `WB_LAYOUT_COLEMAKDH` / `WB_LAYOUT_MINE` /
`WB_LAYOUT_VOU`** in [wechselbalg.h](users/wechselbalg/wechselbalg.h) steuern,
welche Layer überhaupt angelegt werden. Default: auf AVR nur **COLEMAKDH**, auf
RP2040/STM32 alle vier. Die Layouts selbst bleiben in `wrappers.h` vollständig
erhalten — es wird nur kein Layer dafür gebaut.

- **Umstellen = eine Zeile.** Statt `WB_LAYOUT_COLEMAKDH` z. B.
  `WB_LAYOUT_MINE` definieren, neu flashen. Sobald *irgendeines* der vier von
  außen definiert ist, gilt ausschließlich die äußere Wahl.
- Ein einzelnes Board abweichend: `OPT_DEFS += -DWB_LAYOUT_...` in dessen
  Keymap-`rules.mk`.
- Der Umschalt-Block auf ADJUST hat jetzt symmetrische Aliase: `P_*` (obere
  Reihe, persistent) und `D_*` (Home-Row, nur die Sitzung). Ein nicht
  einkompiliertes Layout fällt in **beiden** Reihen auf `___NO__` zurück,
  statt still auf die falsche Ebene zu zeigen.
- Die Layer-Blöcke und OLED-`case`s in den Board-Keymaps sind entsprechend
  mit `#ifdef WB_LAYOUT_*` geklammert.

**Ergebnis — was welches Board jetzt bekommt:**

| | sofle_choc weiß, sofle/rev1, kyria | lotus58 | RP2040/STM32 |
|---|---|---|---|
| Basis-Layouts | QWERT + COLEMAKDH | eigene sechs | alle fünf |
| `TAPPING_TERM` | **150** | 600 | 150 |
| `CHORDAL_HOLD` | **an** | aus | an |
| `RETRO_TAPPING_PER_KEY` | **an** | aus | an |
| `MAGIC_ENABLE` / `MAC_TOG` | aus | aus | an |

`MAGIC_ENABLE` (~1056 Byte) passt auf AVR weiterhin nicht — das weiße Sofle
Choc hat also keinen Mac-Modus. Ist so entschieden (Michael, 2026-08-04).

**Lotus58 bleibt außen vor:** sie ist noch nicht auf das Wrapper-System
umgestellt und bringt ihre eigenen sechs Basis-Layouts mit, die
`WB_LAYOUT_*`-Reduktion greift dort also nicht (524 Byte drüber, gemessen).
Abgeschaltet über `OPT_DEFS += -DWB_NO_ADVANCED_TAP_HOLD` in ihrer
Keymap-`rules.mk`. **Per `OPT_DEFS` und nicht per `keymaps/.../config.h`:**
`build_keyboard.mk` hängt die Keymap-`config.h` **nach** der
Userspace-`config.h` ein (Zeile 459 vs. 431) — ein `#define` dort käme zu spät.
Dasselbe Muster gilt für jedes weitere Board, dem der Flash ausgeht.

`MAGIC_ENABLE` läuft umgekehrt: im Userspace `?= no`, in den Keymap-`rules.mk`
der großen Boards auf `yes` — **`?=` ist nötig**, weil die Keymap-`rules.mk`
*vor* dem Userspace eingelesen wird (Zeile 146 vs. 429). Bei sofle_choc hängt
es an `ifeq ($(strip $(CONVERT_TO)),liatris)`, weil sich weißes und schwarzes
Board dieselbe `rules.mk` teilen.

**Sobald die AVR-Boards auf bessere Controller umgezogen sind, fallen alle
diese Zweige ersatzlos weg** — `WB_LAYOUT_*` auf „alle" und
`WB_NO_ADVANCED_TAP_HOLD` löschen.

Flash danach: **sofle/rev1 99 % / 22 Bytes frei** ⚠️, sofle_choc 99 % /
194 Bytes, kyria 95 % / 1230 Bytes, lotus58 96 % / 912 Bytes.

> ⚠️ **sofle/rev1 hat 22 Bytes Luft.** Das baut heute, aber der nächste
> Upstream-Merge kippt es mit hoher Wahrscheinlichkeit. Der Notausgang liegt
> **auskommentiert bereit** am Ende von
> [keyboards/sofle/keymaps/wechselbalg/rules.mk](keyboards/sofle/keymaps/wechselbalg/rules.mk):
> bei „The firmware is too large!" die `OPT_DEFS`-Zeile dort einkommentieren,
> dann fällt das Board auf TAPPING_TERM 600 ohne Chordal Hold zurück.
> Alternativ dort die `RGBLIGHT_EFFECT_*`-Animationen ausdünnen, wenn das
> Tippgefühl wichtiger ist.

**Offen aus diesem Paket:**
- **Hardware-Test erledigt (2026-08-04).** Beide Hälften geflasht und geprüft.
  Bestanden: LED-Reihenfolge (Nummernblock, Pfeilkreuz, ADJUST-Farben sitzen
  richtig), Layer-Farben auf der Peripherie-Hälfte, Encoder dimmt beide
  Hälften, Jiggler, `D__PREV` zurück aufs vorherige Basis-Layout.
  Zwei Befunde, beide behandelt: `_GAMING` war nicht eingefärbt (siehe C8),
  und die Shift+Shift-Reihenfolge ist asymmetrisch (siehe unten).
  **Noch nicht bewertet:** das Tippgefühl bei `TAPPING_TERM 150` im Alltag.

- **⚠️ Caps Word per Shift+Shift ist reihenfolgeabhängig.** Beobachtet
  2026-08-04: erst rechts halten, dann links *tippen* → es kommt der Tap
  heraus, kein Caps Word. Andersherum geht es.
  **Ursache, kein Bug:** auf QWERT/COLEMAKDH ist die rechte Pinky-Shift ein
  blankes `KC_RSFT` (registriert sofort), die linke aber `SFT_PIP` =
  `LSFT_T(DE_PIPE)`, ein Mod-Tap. QMKs Feature prüft den Modifier-*Zustand*,
  also muss der Mod-Tap über den Tapping-Term **gehalten** werden. Wer zuerst
  links hält, ist beim Drücken der rechten ohnehin darüber — daher die
  Asymmetrie. **Lösung ohne Codeänderung: beide halten, dann loslassen.**
  - `SPECULATIVE_HOLD` hilft hier **nicht** (geprüft): es führt die
    spekulativen Modifier in einer eigenen Variable (`speculative_mods`,
    `action_tapping.c:65`), die nur in den HID-Report einfließt —
    `get_mods()` sieht sie nicht, und genau das prüft Caps Word.
  - Ein eigener Hook in `pre_process_record_user()` sähe zwar die rohen
    Drücke vor der Tap-Hold-Auflösung, müsste dann aber die Taps selbst
    unterdrücken — genau die Buchhaltung, für die COMBO seinen Key-Buffer
    hat. Halbgar gebaut gibt das Streuzeichen.
  - Bleibt als echte Alternative nur **`COMBO_ENABLE` nur für den
    Liatris-Build** (1794 Byte, dort belanglos): das ergäbe KMKs Geste
    (beide drücken, kein Halten). Preis: jeder Shift-Druck wird bis zu
    `COMBO_TERM` gepuffert, was das Tippgefühl beeinflussen kann. Nur
    machen, wenn „beide halten" im Alltag nervt.

- **`chordal_hold_layout` — nur die Kyria braucht wirklich eine.**
  (Korrektur einer früheren Notiz hier: QMKs automatische Tabelle ist besser
  als angenommen. `lib/python/qmk/cli/generate/keyboard_c.py` markiert die
  **Leertaste automatisch mit `'*'`** und zieht bei Boards mit Spacebar eine
  leicht schräge Trennlinie durch sie hindurch.)
  - **GMMK Pro / K3 Pro: erledigt 2026-08-04.** Beide haben jetzt eine
    explizite Tabelle im keymap.c — inhaltlich identisch mit der generierten,
    aber sichtbar und änderbar statt von einer Heuristik abhängig. Der einzige
    Tap-Hold ihrer Daumenreihe ist `NAV_SPC` auf der Leertaste (`'*'`); die
    übrigen (`NAV_TAB`, `NUM__UE`, `SYM__AE`) liegen auf den Außenspalten.
  - **Kyria: echtes Problem, noch offen** (Board wird derzeit nicht benutzt).
     Als symmetrischer Split bekommt sie
    `'L','L','L','L','L','R','R','R','R','R'` für die Daumenreihe — **kein
    `'*'`**. Damit ist exakt der KMK-Hardware-Bug reproduzierbar: `NUM_ENT`
    (linker Daumen) halten + eine linke Taste drücken settlet den Daumen als
    Tap. Vor ihrem nächsten Flash die Tabelle nachtragen.
    Prüfen mit `qmk generate-keyboard-c -kb <board>`.

## Erledigt (2026-08-04, Schritt 3 — C2 + C4 + C6) — noch nicht auf Hardware getestet

### C2 — Caps Word per Shift+Shift = `BOTH_SHIFTS_TURNS_ON_CAPS_WORD`

**Kein Combo.** QMK bringt das Feature mit
(`quantum/process_keycode/process_caps_word.c:99`) und prüft dort
`mods == MOD_MASK_SHIFT`, also den **Modifier-Zustand** — nicht Keycodes und
nicht Koordinaten. Damit ist das Problem, an dem KMK hing (jedes Basis-Layout
baut ein eigenes Shift-Objekt auf dem rechten Pinky), strukturell weg:
`SFT_PIP`/`KC_LSFT` links und `KC_RSFT`/`RFT_MIN` rechts liefern alle LSFT bzw.
RSFT. Steht in [users/wechselbalg/config.h](users/wechselbalg/config.h).

**Gemessen auf sofle/rev1 (2026-08-04):**

| Weg | Flash |
|---|---|
| `BOTH_SHIFTS_TURNS_ON_CAPS_WORD` | **14 Byte** |
| `COMBO_ENABLE` + ein Combo + `combo_ref_from_layer()` | **1794 Byte** |

Der Combo-Weg hätte außerdem **jeden Shift-Druck bis zu `COMBO_TERM` gepuffert**:
`process_combo` sitzt in `pre_process_record_quantum`, also *vor* der
Tap-Hold-Auflösung, und hält Combo-Tasten zurück. Falls doch je Combos
gebraucht werden: `key_combos` muss für `keymap_introspection.c` sichtbar sein
(das macht `#include KEYMAP_C`) — der Userspace-Weg dafür ist
`INTROSPECTION_KEYMAP_C`, so festgelegt (Michael, 2026-08-04).

- **Geste anders als in KMK**: beide Shifts über den Tapping-Term (150 ms)
  **halten** und loslassen, statt beide anzutippen. Bei Mod-Taps ist Halten
  ohnehin nötig, sonst kommen `|` und `-` heraus.
- ⚠️ **Feuert bei jedem LSFT+RSFT**, also auch wenn beide Shift-*Daumen*
  (`SFT_SPC` + `RFT_SPC`) zusammen gehalten werden. Im Hardware-Test
  beobachten. `SFT_CTL` ist unkritisch (LSFT+LCTL ≠ `MOD_MASK_SHIFT`).
  Rückfallebene, falls es stört: eigene positionsbasierte Prüfung über
  `record->event.key` in `process_record_user()` (~40 Byte, aber pro Board
  eigene Koordinaten).
- `COMMAND_ENABLE = no` → keine `IS_COMMAND`-Kollision.
- Auf **sofle/rev1 abgeschaltet** (`OPT_DEFS += -DWB_NO_BOTH_SHIFTS_CW` in ihrer
  `rules.mk`): 14 von 22 Byte wollten wir dort nicht abgeben.

### C4 — Mouse Jiggler auf `A_MSJIG`

Handler in [wechselbalg.c](users/wechselbalg/wechselbalg.c), 5 s Takt, 1 px,
Richtung alternierend (KMKs Anti-Drift 1:1). Der Keycode saß schon im Enum, nur
der Handler fehlte.

- **Takt per `housekeeping_task_user()`**, nicht `DEFERRED_EXEC_ENABLE`: für
  genau einen Timer sind ein `uint32_t` und `timer_elapsed32()` billiger als
  `quantum/deferred_exec.c`, und der Callback liefe im selben
  Schleifendurchlauf.
- **Bewegung als roher `host_mouse_send()`-Report**, nicht `tap_code(MS_LEFT)`:
  Mouse Keys bewegen um `MOUSEKEY_MOVE_DELTA` *mit Beschleunigung*, hier soll
  es genau ein Pixel sein.
- Der Report trägt auch die Maustasten — ein `{0}`-Report würde eine gehaltene
  Taste loslassen. Deshalb setzt der Jiggler einen Takt aus, wenn
  `mousekey_get_report().buttons != 0`.
- Ein Tastendruck beendet ihn **nicht** (Michael, 2026-08-04), nur `A_MSJIG`.
- **Einstellbar:** `WB_JIGGLE_PERIOD_MS` (Takt, Default 5000) und
  `WB_JIGGLE_STEP` (Pixel pro Takt, Default 1) oben im `#ifdef WB_JIGGLER`-Block
  von `wechselbalg.c`; ein einzelnes Board abweichend per `OPT_DEFS`.
  Der Schritt darf beliebig groß sein — die Richtung wechselt, der Zeiger
  kommt also immer wieder an den Ausgangspunkt zurück.
- Läuft nur auf dem Master — QMK verarbeitet alle Key-Events dort. Die
  *Anzeige* auf der Peripherie-Hälfte braucht den Zustands-Sync aus C8.

### C6 — `DF_PREV`

```c
layer_state_t default_layer_state_set_user(layer_state_t state) {
    const uint8_t next    = get_highest_layer(state);
    const uint8_t current = get_highest_layer(default_layer_state);  // noch alt
    if (next != current) wb_prev_default_layer = current;
    return state;
}
```

Der Trick ist QMKs Reihenfolge: `default_layer_state_set()`
(`quantum/action_layer.c:42-48`) ruft erst die `_kb`/`_user`-Kette und weist
`default_layer_state` **danach** zu — im Callback steht also noch der alte Wert.
Fängt `DF()` und `set_single_persistent_default_layer()` gleichermaßen ab, weil
beide dort durchlaufen; und weil der Rücksprung selbst wieder durch den Hook
geht, bringt zweimal Drücken einen zurück (wie KMK).

- Bewusst **nicht persistent** — `_GAMING` wird per `DF()` betreten.
- Direkt nach dem Boot zeigt es auf `_QWERT`.
- Keycode **am Ende** von `enum CustomKeys` angehängt (FN_EXIT-Lehre).
- **In den Keymaps steht `D__PREV`, nicht `DF_PREV`** — ein Buchstabe
  Unterschied, aufpassen. `D__PREV` ist der Rasterplatz und fällt ohne
  `WB_DF_PREV` auf das alte `D_QWERT` zurück.
- Belegt in sofle_choc, sofle/rev1, kyria (obere rechte Ecke von `_GAMING`).
  GMMK Pro und K3 Pro haben in `_GAMING` gar keine Ausstiegstaste — sie kommen
  über ADJUST (`MO__NAV`+`MO__NUM` → `D_QWERT`) raus, das bleibt so.

### AVR-Gating: `WB_JIGGLER` / `WB_DF_PREV`

C4+C6 kosten zusammen rund 240 Byte, gemessen: sofle/rev1 wäre **226 Byte
drüber**, sofle_choc (weiß) **50**, nur die Kyria hätte Platz. Deshalb dasselbe
Muster wie `MAGIC_ENABLE`/`WB_LAYOUT_*` — in
[wechselbalg.h](users/wechselbalg/wechselbalg.h): `#ifndef __AVR__` → an, sonst
aus. Ein einzelnes Board abweichend: `-DWB_JIGGLER` / `-DWB_DF_PREV` per
`OPT_DEFS`. Ohne die Flags sind die Tasten **wirkungslos statt falsch**.

**Flash danach:** sofle/rev1 **22 Byte frei (unverändert)**, sofle_choc weiß
180 (−14, nur C2), kyria 1212, lotus58 896. Alle sechs Boards + der
Liatris-Build kompilieren.

## Reihenfolge für die nächste Session (Stand 2026-08-04)

**Die KMK→QMK-Angleichung ist inhaltlich durch.** C1–C8, C7 und die Status-LED
sind umgesetzt und am 2026-08-04 auf der schwarzen Sofle Choc bestätigt,
inklusive Helligkeitskurve und Split-Sync der Statusflags. Was bleibt:

1. **Tippgefühl bei `TAPPING_TERM 150`** im Alltag beurteilen — der einzige
   Punkt, der sich nur über längere Benutzung zeigt. Bei versehentlichen
   Modifiern liegen `FLOW_TAP_TERM` und `SPECULATIVE_HOLD` dokumentiert bereit
   (siehe unten).
2. Kyria-Handedness (`chordal_hold_layout`), dann ganz zuletzt das OLED.

Offen als *Entscheidung*, nicht als Arbeit: ob die Shift+Shift-Geste bei
„beide halten" bleibt oder den Combo-Weg bekommt (siehe oben).

## Noch offen — Rest von Schritt 2/3

- **⚠️ TODO `FLOW_TAP_TERM` — noch NICHT aktiviert, ggf. nachziehen.**
  Schaltet Tap-Hold ab, solange schnell getippt wird: liegt der vorherige
  Tastendruck weniger als *n* ms zurück (empfohlener Startwert 150) und sind
  beide beteiligten Tasten Buchstaben oder Space, wird sofort als Tap
  entschieden. Zielt auf dasselbe wie Chordal Hold, aber über Zeit statt über
  Händigkeit — fängt damit auch Roll-Overs auf der **Gegen**hand, die Chordal
  Hold durchlässt. Anzuschalten, wenn nach dem Hardware-Test der 150 ms noch
  versehentliche Modifier bei schnellem Tippen auftreten. Feineinstellung über
  `is_flow_tap_key()` und `get_flow_tap_term()`.
- **⚠️ TODO `SPECULATIVE_HOLD` — noch NICHT aktiviert, ggf. nachziehen.**
  Setzt den Modifier eines Mod-Taps sofort beim Keydown und nimmt ihn zurück,
  falls es doch ein Tap wird. Behebt die Trägheit von Shift+Klick (der
  Modifier steht sonst erst nach dem Tapping-Term). Ändert die
  Tap/Hold-Entscheidung **nicht** und löst deshalb das Retro-Tap-Problem
  nicht — die beiden sind unabhängig voneinander. Anzuschalten, wenn sich
  Shift+Klick/Ctrl+Klick am Mod-Tap-Shift träge anfühlt.

### C8 Per-Key-RGB-Farbsprache — umgesetzt 2026-08-04, nicht auf Hardware getestet

KMK leitet alle LED-Farben aus dem Keymap **ab**, statt sie zu pflegen:
Modifier-Farben (GUI blau / Alt orange / Ctrl magenta / Shift gelb, Mischfarbe
bei Shift+Ctrl), jede Taste die einen Layer *erreicht* trägt dessen Farbe
dauerhaft, Tri-Layer-Hinweis (NUM-Taste wird rot solange NAV gehalten wird),
`KC_NO` → aus, Maus-Block neutralgrau. Regeln in
`/Users/mike/dev/kmkfw/boards/wechselbalg/design/palette.py` + `rgb_rules.py`.

**Ausgangslage ist besser als früher hier notiert:**
`keyboards/sofle_choc/keyboard.json` hat `features.rgb_matrix: true` und eine
`rgb_matrix.layout` mit **allen 58 LEDs samt `matrix: [row, col]`** und
`split_count: [29,29]`. Die Reverse-Map LED→Taste, die KMK in `led_tables.py`
von Hand herleiten und per Chase-Test korrigieren musste, ist also schon da:
`g_led_config.matrix_co[row][col]`.

**Gemessen 2026-08-04:**
- `RGB_MATRIX_ENABLE = yes` / `RGBLIGHT_ENABLE = no` **baut auf dem
  Liatris-Target sauber durch**, sobald der tote `#ifdef RGB_MATRIX_ENABLE`-Block
  in [keymaps/wechselbalg/config.h](keyboards/sofle_choc/keymaps/wechselbalg/config.h)
  raus ist — der redefiniert `RGB_MATRIX_SPLIT`, das keyboard.json schon
  liefert (`-Werror`). Mit allen Animationen aus keyboard.json wuchs die UF2
  von 96768 auf 104448 Byte; **ohne sie und mit der Farbsprache sind es
  97280** — der Umstieg kostet also praktisch nichts, die Animationen waren
  der ganze Aufschlag.
- **Auf dem weißen AVR-Board ist RGB Matrix 3270 Byte zu groß.** Der Umschalter
  muss also an `ifeq ($(strip $(CONVERT_TO)),liatris)` hängen wie
  `MAGIC_ENABLE`; alte und neue Welt koexistieren im keymap.c per `#ifdef`.

**KMKs Host-Build-Schritt ist in QMK nicht nötig** — der existierte nur wegen
CircuitPythons Heap-Fragmentierung. QMK-Keycodes sind `uint16_t` mit
Bitfeldern, und QMK liefert genau die zwei Funktionen, die die Ableitung zur
Laufzeit trivial machen:
- `layer_switch_get_layer(keypos_t)` (`quantum/action_layer.h:170`) — der
  höchste aktive Layer, der dort **nicht** `KC_TRNS` ist. Das ist KMKs
  „TRNS fällt auf base_layer durch", nur korrekt statt als Näherung.
- `keymap_key_to_keycode(layer, keypos_t)` (`quantum/keymap_common.h:10`)

Damit ist `rgb_matrix_indicators_advanced_user()` ~60 Zeilen. Klassifikation
statt KMKs `isinstance`: `IS_QK_MOD_TAP` + `QK_MOD_TAP_GET_MODS`,
`IS_MODIFIER_KEYCODE`, `IS_QK_MODS(kc) && QK_MODS_GET_BASIC_KEYCODE(kc)==0`
für reine Modifier-Combos (`SFT_CTL`) — genau KMKs `key.key is None` —, sowie
`IS_QK_LAYER_TAP`/`IS_QK_MOMENTARY`/`IS_QK_TO`/… fürs Layer-Ziel.

**Zusätzliche Entscheidungen (Michael, 2026-08-04):**
- Die **Zustände (Caps Word, Layer Lock, Jiggler, Mac) wandern auf die
  Status-LED**, statt die Tastenmatrix zu fluten. Die Status-LED soll
  ergänzende Information tragen statt den Layer nochmal zu wiederholen.
  Damit schrumpft die per-Key-Seite auf Identität + Layer + Semantik.
- **Indikatoren unabhängig konfigurierbar.** Nebenbefund: die beiden Zeilen
  `RGB_DISABLE_AFTER_TIMEOUT` / `RGB_DISABLE_WHEN_USB_SUSPENDED` in der
  Keymap-config.h sind **tote Namen** — RGB Matrix kennt nur
  `RGB_MATRIX_TIMEOUT` (Default 0 = nie) und `RGB_MATRIX_SLEEP`. Ohne die
  laufen die Indikatoren immer.
- **Alle LEDs werden mitgedimmt**, nicht nur das Grundleuchten:
  `rgb_matrix_set_color()` schreibt rohes RGB, also müssen wir jede
  Indikatorfarbe selbst mit `rgb_matrix_get_val()` skalieren. Das ist die
  Nahtstelle zu C7.
- **Die QMK-Beleuchtungsschemata bleiben erhalten und werden abgeleitet
  statt gepflegt**: der beleuchtete Nummernblock auf `_NUM` und das Pfeilkreuz
  auf `_NAV` (heute `SET_NUMPAD`/`SET_GAMING` als handgepflegte LED-Indizes)
  sind reine Keycode-Regeln, weil `keymap_neo2.h` auf Standard-Keycodes
  abbildet: `N3_NUM0..9` = `KC_P0..P9` (zusammenhängend `KC_KP_1 ... KC_KP_0`)
  und `N3___UP/LEFT/DOWN/RGHT` = `KC_UP/LEFT/DOWN/RIGHT` (zusammenhängend
  `KC_RIGHT ... KC_UP`). Also: **semantische Tastenklassen** als eigene
  Regelstufe neben KMKs Modifier- und Layer-Regeln.
- LED-Reihenfolge **sichtprüfen**, nicht der Board-Definition blind vertrauen —
  genau der Fehler, den KMKs Chase-Test gefunden hat.

**Die Palette und ihre Regel** (alle Werte oben in `rgb_language.c`,
Nachjustieren am Board ist eine Zeile):

> **Farbton = Bedeutung, Sättigung = Art.**
> Gesättigter Farbton = Rolle (welcher Layer, welcher Modifier) · Weiß =
> Inhalt (der Nutzblock des aktiven Layers) · Grau = neutraler Inhalt (Maus)
> · gedimmtes Warm = nichts Besonderes (Grundleuchten)

Der Farbkreis ist mit fünf Layern + vier Modifiern + Cyan praktisch voll.
Deshalb liegen Nummernblock und Pfeilkreuz bewusst auf der *Sättigungs*achse
statt auf noch einem Farbton — so kollidieren sie mit nichts, und der Kontrast
zur Layer-Farbe ist maximal, egal welche das ist. Damit ersetzt Weiß das
frühere Magenta (`SET_NUMPAD`) und Rot (`SET_GAMING` auf `_NAV`); Rot hätte
dort außerdem „diese Taste erreicht ADJUST" bedeutet.

Zwei Abweichungen von KMKs Werten, beide wegen einer echten Kollision:
`_NUM` (255,80,0) → **(255,140,0)**, weil KMKs Orange neben Alts Orangerot lag
und `KC_RALT` auf `_NUM` sitzt; `Ctrl` (255,0,90) → **(255,0,140)**, weil es zu
nah an ADJUSTs Rot lag und die Ctrl-Daumen auf `_ADJUST` liegen.

**Split:** QMK synchronisiert von sich aus `layer_state`
(`SPLIT_LAYER_STATE_ENABLE` — steht heute im `#ifdef RGBLIGHT_ENABLE`-Block der
Keymap-config.h und muss dort raus) und die komplette RGB-Matrix-Config
(`PUT_RGB_MATRIX`, `quantum/split_common/transactions.c:576` — Helligkeit also
automatisch, wichtig für C7). **Nicht** synchronisiert: Caps Word, Layer Lock,
Jiggler, Mac-Modus → eine eigene Transaktion (`SPLIT_TRANSACTION_IDS_USER`,
`docs/features/split_keyboard.md:335`), **ein Byte Statusflags** reicht für
alle vier. Gehört jetzt zum Status-LED-Paket, nicht mehr zu C8.

**Umsetzung:** [users/wechselbalg/rgb_language.c](users/wechselbalg/rgb_language.c),
rund 350 Zeilen mit Kommentar. Angeschaltet per `WB_RGB_LANGUAGE = yes` in der
Keymap-`rules.mk` — **ausdrücklich opt-in und nicht an `RGB_MATRIX_ENABLE`
gehängt**, weil GMMK Pro und K3 Pro RGB Matrix ebenfalls anhaben und ihre
Beleuchtung nicht ungefragt wechseln sollen.

**Die Prioritätskette** (pro LED einmal `layer_switch_get_layer` +
`keymap_key_to_keycode`, dann von oben):

| | Regel |
|---|---|
| 1 | `KC_NO` → **aus** (macht einen Overlay-Layer lesbar: er zeigt nur, was er belegt) |
| 2 | Einzeltasten: `QK_BOOT` magenta, `CW_TOGG` weiß, `QK_LLCK`/`LR_EXIT`/`DF_PREV` cyan, `A_MSJIG`, `MAC_TOG` |
| 3 | Modifier-Rolle, Mischfarbe bei Kombination (`SFT_CTL`) |
| 4 | Layer-Ziel + Tri-Layer-Hinweis |
| 5 | Semantische Klasse: Numpad, Pfeilkreuz, GAMINGs WASD (weiß), Maus (grau) |
| 6 | Aktiver Overlay-Layer — **und `_GAMING`**, siehe unten |
| 7 | Grundleuchten |

**Zwei Nachbesserungen aus dem Hardware-Test (2026-08-04):**
- **`_GAMING` färbt sich jetzt selbst.** Es ist ein *Default*-Layer, also galt
  `layer == base_layer` und Stufe 6 griff nicht — das Board sah in GAMING aus
  wie ein Basis-Layout. Jetzt zählt `_GAMING` dort ausdrücklich mit: es ist ein
  Modus, in dem man ist, kein Layout, auf dem man tippt. Die übrigen
  Basis-Layouts bleiben bewusst farblos.
- **WASD auf `_GAMING` in Weiß.** Anders als Nummernblock und Pfeilkreuz ist
  das *keine* layerfreie Keycode-Regel — `KC_W/A/S/D` sind gewöhnliche
  Buchstaben und stehen auf jedem Basis-Layout. Deshalb an `_GAMING` gebunden;
  innerhalb des Layers wandern sie trotzdem mit.

**Stufe 6 ist in QMK geschenkt.** „Der Layer färbt, was er belegt" ist
`layer_switch_get_layer(pos) != get_highest_layer(default_layer_state)` — an
einer durchgereichten Position hätte die Funktion das Basis-Layout geliefert.
KMK brauchte dafür `overlay_positions()`, die Funktion, an der der Boot damals
an der Speicherzuteilung scheiterte.

**Der Tri-Layer-Hinweis trifft von selbst die richtigen zwei Tasten**: nur die
inneren Daumen (`NUM_ENT` links, `NAV_BSC` rechts) werden auf dem jeweils
anderen Layer nicht überschrieben — ohne dass eine Koordinate im Code steht.

**Prüfung ohne Hardware:** `/private/tmp/.../scratchpad/preview.py` (nicht
eingecheckt) liest das *echte* `keymaps`-Array aus dem ELF (`.rodata`, Symbol
`keymaps`) und bildet die Kette in Python nach — damit ist belegt, dass
Nummernblock, Pfeilkreuz, Maus-Block, `SFT_CTL`-Mischfarbe, die `KC_NO`-Regel
und der Tri-Layer-Hinweis wirklich herauskommen. Bei Zweifeln am nächsten
Umbau wieder so machen, das ist billiger als flashen.

### C7 Encoder auf `_ADJUST` = RGB-Helligkeit — umgesetzt 2026-08-04

Beide Encoder regeln auf `_ADJUST` die Helligkeit (`wb_brightness()` im
sofle_choc-keymap.c, gilt für beide Sofle-Choc-Boards). Seiten und Lautstärke
sind auf jedem anderen Layer erreichbar, es geht also nichts verloren.

⚠️ **`rgb_matrix_increase_val_noeeprom()` / `_decrease_val_noeeprom()`** — nicht
die EEPROM-Variante, sonst schreibt jede Encoder-Rastung ins EEPROM (auf
RP2040 emuliert im Flash). Direkt die Funktion statt `tap_code16(RGB_VAI)`.
Über den Split kostenlos: die RGB-Matrix-Config wird automatisch
synchronisiert, beide Hälften dimmen zusammen.

Auf dem weißen Board läuft derselbe Code über `rgblight_*_noeeprom()`
(32 Byte, danach noch 148 frei). sofle/rev1 und Kyria haben den Fall bewusst
nicht — dort ist kein Platz bzw. kein Bedarf.

### Status-LED (Liatris-NeoPixel GP25) — umgesetzt und geprüft 2026-08-04

Jede Liatris-Hälfte hat eine eigene WS2812 an GP25. Sie soll **die Zustände
tragen, die nicht auf die Tastenmatrix sollen** (Caps Word, Layer Lock,
Jiggler, Mac-Modus) und perspektivisch weitere ergänzende Information — nicht
nur den Layer wiederholen.

⚠️ **Die Hürde ist real:** `platforms/chibios/drivers/vendor/RP/RP2040/ws2812_vendor.c`
ist eindeutig Single-Instance — ein `WS2812_DI_PIN`, ein statisches
`WS2812_BUFFER[WS2812_LED_COUNT]`, eine `STATE_MACHINE`, ein DMA-Kanal, ein
Semaphor. Keine zweite Kette, kein Parameter dafür.

Belegung bestätigt: Per-Key-Kette auf **GP0** (keyboard.json `ws2812.pin = "D3"`,
`platforms/chibios/converters/promicro_to_rp2040_ce/_pin_defs.h:7` sagt
`#define D3 0U`) → PIO0. Split-Serial auf PIO1. Status-LED GP25.

**Der Weg: PIO0 hat vier State Machines, der Treiber belegt genau eine.** Eine
zweite SM auf PIO0 mit eigener Kopie des 6-Instruktionen-WS2812-Programms
(Instruktionsspeicher: 32 Wörter) und `sideset` auf GP25. Für **eine** LED
braucht es weder DMA noch Interrupt — ein `pio_sm_put_blocking()` mit einem
32-Bit-Wort, nur bei Farbwechsel. Geschätzt 60–80 Zeilen im Keymap, ohne
Eingriff in den QMK-Core. Bit-Bang mit `chSysLock()` wären ~30 µs Interrupts
aus pro Update — machbar, aber schlechter neben Split-Link und USB.

**Umsetzung:** [users/wechselbalg/status_led.c](users/wechselbalg/status_led.c),
angeschaltet per `WB_STATUS_LED = yes` in der Keymap-`rules.mk`.

Die Init-Reihenfolge ist belegt: `keyboard_init()` ruft `rgb_matrix_init()`
(→ `ws2812_init()`, PIO0 aus dem Reset, erste SM belegt) und erst danach
`keyboard_post_init_quantum()`. `wb_status_led_init()` hängt deshalb in
`keyboard_post_init_user()` des Boards. Findet `pio_claim_unused_sm(pio0,
false)` keine SM, tut die Datei schlicht nichts — eine Status-LED ist kein
Grund, eine Tastatur nicht booten zu lassen.

⚠️ **Include-Reihenfolge:** die pico-sdk-Header müssen **vor** `wechselbalg.h`
stehen. Andersherum scheitert schon `pico/assert.h` an einem
Makro-Namenskonflikt mit dem, was `quantum.h` an ChibiOS/rp2040.h hereinzieht
— derselbe Grund, aus dem `ws2812_vendor.c` seine Reihenfolge kommentiert.

**Was sie zeigt** (Prioritätskette, Momentzustände über Dauerzuständen):

| | Zustand | Farbe |
|---|---|---|
| 1 | Caps Word | weiß |
| 2 | Layer Lock | cyan (wie die Taste) |
| 3 | Jiggler | gelbgrün |
| 4 | Ruhe + Mac-Modus | azur |
| 5 | Ruhe + PC | **aus** |

Punkt 5 ist die eigentliche Idee (angepasst 2026-08-04 nach dem ersten
Hardware-Eindruck): die LED leuchtet nur, wenn es etwas zu sagen gibt — damit
ist ihr Leuchten selbst schon die erste Information, statt dass ein Punkt
dauerhaft brennt und nichts mitteilt.

**Helligkeit** hängt am selben Regler wie die Matrix (`rgb_matrix_get_val()`),
mit einem **vorzeichenbehafteten** Offset darauf: `WB_STATUS_VAL_OFFSET`,
negativ = dunkler, positiv = heller, Default **−24**. Die Platinen-LED ist
ohne Abdeckung verbaut und blendet bei gleicher nomineller Helligkeit, sobald
die Tasten angenehm eingestellt sind — ein einzelner Helligkeitsschritt, wie
KMK ihn abzog, reicht dafür nicht (Hardware-Befund 2026-08-04). `RM_TOGG`
schaltet alles mit ab.

⚠️ **Der Offset verschiebt die Helligkeit, nicht den Abschaltpunkt.** Ein
reiner Abzug hat den Fehler, den auch der KMK-Port hat: bei −24 wäre die LED
schon dunkel, während die Matrix noch drei Stufen vor sich hat — man dimmt die
Tasten angenehm und die Zustandsanzeige ist unbemerkt weg (Hardware-Befund
2026-08-04). Deshalb fällt sie beim Dimmen nur bis `WB_STATUS_MIN_VAL`
(Default = ein `RGB_MATRIX_VAL_STEP`), bleibt dort stehen und geht erst
relativ zur Tastenbeleuchtung aus:

| Offset | Status-LED geht aus |
|---|---|
| negativ | eine Helligkeitsstufe **vor** den Tasten |
| 0 | gleichzeitig mit den Tasten |
| positiv | eine Stufe **später** — bleibt an, wenn die Tasten schon dunkel sind |

Mit den Defaults (Step 8, Offset −24) ergibt das: Matrix 40 → 16, 32 → 8,
24 → 8, 16 → 8, 8 → aus. **Am Board bestätigt (2026-08-04).** Der Mac-Hinweis zusätzlich geteilt durch
`WB_STATUS_IDLE_DIV` (Default 2), aber ebenfalls nicht unter
`WB_STATUS_MIN_VAL` — sonst wäre ausgerechnet er als erstes unsichtbar.

**Split:** eine eigene Transaktion (`SPLIT_TRANSACTION_IDS_USER WB_SYNC_STATUS`,
in der Keymap-`config.h`) mit **einem Byte Statusflags**. Geht nur bei
Änderung raus, plus ein Auffrischen im Sekundentakt, damit eine verlorene
Übertragung keine dauerhaft falsche Anzeige hinterlässt. Layer Lock meldet
sich über den `layer_lock_set_user()`-Hook, statt `is_layer_locked()` je Layer
abzufragen.

**Auf Hardware bestätigt (2026-08-04): die zweite PIO0-State-Machine ist zu
haben und die LED leuchtet.** Damit ist der einzige Punkt geklärt, der sich
nicht am Schreibtisch belegen ließ. Falls das nach einem Upstream-Merge je
kippt: zuerst prüfen, ob `wb_status_sm` negativ bleibt (dann ist keine SM
frei), danach Byte-Reihenfolge (GRB) und Pin.

## Zuletzt: OLED (bewusst als letzter Punkt)

Die OLEDs sollen auf dem schwarzen Board wieder laufen (`OLED_ENABLE = yes` ist
gesetzt, die `oled_task_user()`-Anzeige inkl. Layer-Namen existiert schon).
**Blocker: sobald das OLED aufgesteckt ist, kommt Michael am Liatris nicht mehr
an den BOOT-Taster** — und ohne BOOT-Taster kein UF2-Flash. Vor dem OLED-Paket
muss dafür eine Lösung stehen, z. B.:
- `QK_BOOT` ist im ADJUST-Layer bereits belegt (linke Hälfte, Reihe 3 innen) —
  reicht, solange die Firmware bootet und der Layer erreichbar ist; hilft aber
  nicht bei einer kaputt geflashten Firmware.
- Doppelter Reset-Tap / `RP2040_BOOTLOADER_DOUBLE_TAP_RESET` als zweiter Weg.
- Reset-/BOOT-Taster nach außen verlängern, oder das OLED steckbar/geklappt montieren.

## Geklärt, nichts zu tun

- **Option-Taste am Mac = Alt-Taste am PC.** Auf HID-Ebene gibt es nur
  `LALT`/`RALT`; macOS nennt sie Option (⌥). `CG_TOGG` fasst sie nicht an, der
  mittlere Daumen bleibt in beiden Modi gleich. `RALT_PR` (`ALGR_T`) ist am Mac
  die rechte Option-Taste und dort genau wie AltGr der Sonderzeichen-Modifier.
- **Apples fn-Taste ist per USB-HID nicht sendbar** und fehlt deshalb
  zwangsläufig im Layout — sie ist kein HID-Keycode, sondern wird in
  Apple-Tastaturen im Controller ausgewertet und über einen proprietären
  Vendor-Report gemeldet. Keine QMK-Lücke, sondern eine Protokollgrenze.
  Ersatz: Medientasten direkt (ADJUST-Layer + Encoder), PgUp/PgDn/Home/End/Del
  im `_NAV`-Layer. Einzig der fn+fn-Doppeldruck (Diktat/Emoji-Picker) hat kein
  Äquivalent — in den macOS-Systemeinstellungen auf eine sendbare Kombo legen.
  **Nicht erneut untersuchen.**

## Ausdrücklich NICHT übertragen

- Reduktion auf zwei Basis-Layouts (`ACTIVE_CANDIDATE`) — reine KMK-RAM-Not.
- Split-Link-Arbeit (CRC-8, State-Beacon, 115200 Baud, `LINK_STATS`,
  4-Byte-Frame-Limit) — löst Probleme, die QMKs Transport nicht hat.
- USB-Lockdown, `boot.py`, NVM-Bootlog, Dead-Boot-Diagnose — CircuitPython-spezifisch.
- `.mpy`/Firmware-Freeze/Fragmentierung — CircuitPython-spezifisch.
- Encoder-Richtung rechts: KMK musste das per Koordinaten-Swap korrigieren,
  `keyboards/sofle_choc/keyboard.json` dreht `pin_a`/`pin_b` für die rechte
  Hälfte bereits selbst.
- `KC.PDF` (persistentes Default-Layout) — QMK hat das Äquivalent
  (`set_single_persistent_default_layer`) längst im ADJUST-Grid.
- **KMK hinkt an drei Stellen hinterher**, das ist *kein* Portierungsbedarf:
  NUM linke Innenspalte (`REDO/UNDO/ENT` statt `CUT/COPY/PASTE`), NAV Zeile 0
  (drei `KC.NO`) und NAV r1 (`NO` statt `NX__CUT`, `CUT` statt `NX_FIND`).
  Der KMK-Port übernahm einen Stand von vor QMK-Commit `96cc9e7c31` (2025-01-30).
  Ggf. umgekehrt nach KMK zurückportieren.

## Aufräum-Reste (klein, unkritisch)

- `enum CustomKeys` enthält Leichen: `RN_STEM`, `RN_CODE`, `KC_D_MUTE` unbenutzt.
  **Achtung:** Entfernen von `RN_STEM`/`RN_CODE` verschiebt alle folgenden Werte —
  das war die Ursache des `FN_EXIT`-Bugs. Erst nach dessen Fix aufräumen (erledigt),
  aber trotzdem prüfen, ob noch irgendwo ein Custom-Keycode als Layer-Index missbraucht wird.
- `#define SFT_NUM LSFT_T(NUM)` (`wechselbalg.h`) ist derselbe Fehlertyp
  (Custom-Keycode als Argument), aber unbenutzt.
