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
- **K3 Pro**: MINE-Ebene enthält Annahmen (neu belegte `+`-Taste, NUBS = `MO__NUM`);
  `_TEST`-Layer ist von keiner Taste erreichbar. Vor Nutzung sichten.
- **Lotus58: nicht mehr anfassen.** Michael besitzt das Board nicht mehr
  (2026-08-04). Die Keymap bleibt im Repo, wird aber **nicht** gepflegt: nicht
  auf das Wrapper-System umstellen, nicht in Feature-Rollouts einbeziehen,
  Build-Fehler dort nicht priorisieren. Sie steht bewusst über
  `OPT_DEFS += -DWB_NO_ADVANCED_TAP_HOLD` in ihrer `rules.mk` auf dem alten
  Tap-Hold-Verhalten. Erst wieder aufnehmen, wenn Michael es ausdrücklich sagt.
- K3-Pro-Varianten ansi/jis/white bleiben ungebaut (alte LED-Tabellen-Makros).
- Idee: Mac-Variante des `_NAV`-Layers (Wort-Sprünge Opt+Pfeil statt Ctrl+Pfeil).
  Wird durch C1 (Ctrl⇄GUI-Swap) **wichtiger**, nicht überflüssig — siehe dort.

---

# KMK→QMK-Angleichung (laufend, Stand 2026-08-04)

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
- **Wo die Taste sitzt** (Sofle Choc, `ADJUST_L0` Position 4): oberste Reihe,
  vierte Taste von links — dort, wo auf der Basisebene **F3** liegt.
  Hinkommen: beide inneren Daumen halten (links `NUM_ENT`, rechts `NAV_BSC`)
  → Tri-Layer `_ADJUST` → F3 tippen, Daumen dabei gehalten lassen.
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
- **Hardware-Test der schwarzen Sofle Choc steht aus** — beide Hälften sind am
  2026-08-04 geflasht (je 96768 Byte UF2), aber noch nicht benutzt. Zu prüfen:
  Tippgefühl bei 150 ms, Daumen-Chords unter Chordal Hold, Retro-Tap-Fenster,
  `MAC_TOG` inkl. EEPROM-Persistenz, `FN_EXIT` auf einem Nicht-QWERT-Layout,
  die OLED-Zeile MAC/PC, Power-LED aus, Mouse Keys im `_NAV`-Layer.
- **`chordal_hold_layout` — nur die Kyria braucht wirklich eine.**
  (Korrektur einer früheren Notiz hier: QMKs automatische Tabelle ist besser
  als angenommen. `lib/python/qmk/cli/generate/keyboard_c.py` markiert die
  **Leertaste automatisch mit `'*'`** und zieht bei Boards mit Spacebar eine
  leicht schräge Trennlinie durch sie hindurch.)
  - **GMMK Pro / K3 Pro: unkritisch.** Der einzige Tap-Hold in deren
    Daumenreihe ist `NAV_SPC` aus dem `7_THUMBS`-Wrapper, und der sitzt auf
    der Leertaste — die Auto-Tabelle gibt ihr `'*'`. Die übrigen Tap-Holds
    (`NAV_TAB`, `NUM__UE`, `SYM__AE`) liegen auf den Außenspalten und werden
    korrekt L/R geraten. Eine eigene Tabelle ist Feinschliff, kein Fix.
  - **Kyria: echtes Problem.** Als symmetrischer Split bekommt sie
    `'L','L','L','L','L','R','R','R','R','R'` für die Daumenreihe — **kein
    `'*'`**. Damit ist exakt der KMK-Hardware-Bug reproduzierbar: `NUM_ENT`
    (linker Daumen) halten + eine linke Taste drücken settlet den Daumen als
    Tap. Vor ihrem nächsten Flash die Tabelle nachtragen.
    Prüfen mit `qmk generate-keyboard-c -kb <board>`.

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

- **C2 Caps Word per Shift+Shift** (`COMBO_ENABLE`). KMK: Timeout 150 ms statt 50,
  weil der Split-Link Latenz addiert und jede Hälfte gegen ihre eigene Sicht timet.
- **C4 Mouse Jiggler.** `A_MSJIG` ist im ADJUST-Layer verdrahtet, hat aber
  **nirgends einen Handler** — die Taste tut nichts. KMK: 5 s Takt, 1 px,
  Richtung alternierend (sonst driftet der Zeiger über den Tag).
- **C6 `DF_PREV`** — zurück zum *vorherigen* Basis-Layout statt hart QWERT
  (GAMING-Ausstieg, siehe Schritt 1). In QMK selbst zu bauen.
- **C7 Encoder auf `_ADJUST` = RGB-Helligkeit.** `encoder_update_user()` hat
  keinen `_ADJUST`-Fall, fällt auf PgUp/PgDn + Volume zurück. KMK gibt beiden
  Encodern dort die Helligkeit, weil Seiten/Lautstärke auf jedem anderen Layer
  erreichbar sind.
- **C8 Per-Key-RGB-Farbsprache** (eigenes, größeres Paket). KMK leitet alle
  LED-Farben aus dem Keymap ab: Modifier-Farben (GUI blau / Alt orange /
  Ctrl magenta / Shift gelb, Mischfarbe bei Shift+Ctrl), jede Taste die einen
  Layer *erreicht* trägt dessen Farbe dauerhaft, Tri-Layer-Hinweis (NUM-Taste
  wird rot solange NAV gehalten wird), `KC_NO` → aus, Zustandsfarben für
  Layer-Lock/Caps-Word/Jiggler/Mac, Maus-Block neutralgrau.
  QMK-Seite: `keyboards/sofle_choc/keyboard.json` hat eine vollständige
  `rgb_matrix`-Definition mit 58 Per-Key-LEDs; der Keymap nutzt aber
  `RGBLIGHT` mit handgepflegten LED-Index-Makros. Umsetzung über
  `rgb_matrix_indicators_advanced_user()`, Umstellung RGBLIGHT→RGB Matrix nötig.
  Sinnvoll in Stufen (erst „tote Tasten dunkel" + Modifier-Farben).
- **C-Status-LED.** Liatris hat eine eigene WS2812 an **GP25** (KMK zeigt dort
  Layer-Farbe / Caps Word / Jiggler / Mac-Modus). ⚠️ Technische Hürde: QMKs
  WS2812-Treiber kennt genau **eine** Kette (`WS2812_DI_PIN`), und GP0 ist schon
  von der Per-Key-Kette belegt. Braucht also entweder eine zweite PIO-Instanz
  oder einen kleinen eigenen Treiber → eigenes Arbeitspaket, vor der Umsetzung
  Machbarkeit klären.

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
