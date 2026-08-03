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
- **Lotus58-Keymap** ist noch eigenständig (nicht auf das Wrapper-System umgestellt)
  → hat daher noch **keinen** `_MAC`-Layer. Nächster natürlicher Schritt.
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

## Schritt 2 — Tippgefühl (Details noch offen, zusammen zu bewerten)

KMK hat `TAP_TIME` von 600 auf **150 ms** gesenkt und das durch zwei Mechanismen
abgesichert. Die drei Punkte hängen zusammen und ergeben nur gemeinsam Sinn:

- **B1 `TAPPING_TERM` 600 → 150** (`users/wechselbalg/config.h`).
- **B2 `CHORDAL_HOLD`** + `chordal_hold_layout[][]` im Keymap. Regel: wird
  innerhalb des Tapping-Terms eine zweite Taste **derselben Hand** gedrückt,
  gilt der Tap-Hold sofort als *getippt*. Gegenhand → fällt auf das bereits
  aktive `PERMISSIVE_HOLD` zurück.
  **Kritisch: die Daumenreihe muss mit `'*'` ausgenommen werden.** KMK hat genau
  das auf Hardware falsch gehabt: NUM-Daumen halten + linke NUM-Taste drücken
  ergab Del + Basis-Layer-Buchstabe statt der NUM-Belegung.
  QMK-Vorteil gegenüber KMK: `chordal_hold_layout` wird mit dem **`LAYOUT`-Makro
  des Boards** geschrieben, also in *visueller* Reihenfolge — genau der
  Indexfehler, den KMK dort hatte (KMKs Tabelle ist nach Matrix-Koordinate
  indiziert), kann hier nicht passieren.
- **B3 `RETRO_TAPPING`.** KMKs `retro_tap_timeout` (1000 ms für Layer-Taps,
  500 ms für Mod-Taps) hat QMK **nicht**. Der Grund für das kurze Mod-Tap-Fenster
  gilt in QMK genauso: ein Klick mit einer *echten* Maus erreicht die Firmware
  nicht, also zählt er nicht als „Hold benutzt" — Shift+Klick würde beim Loslassen
  ein Leerzeichen nachschieben, Alt+Klick ein PrintScreen aus `RALT_PR`.
  Drei Wege, siehe Diskussion:
  1. `RETRO_TAPPING_PER_KEY` + `get_retro_tapping()` nur für Layer-Taps
     (`IS_QK_LAYER_TAP`) → Problem verschwindet ganz, aber die Space/Shift-Daumen
     verlieren den Retro-Tap.
  2. Zusätzlich ein Zeitfenster für Mod-Taps selbst bauen: `get_retro_tapping()`
     wird beim **Release** ausgewertet (`quantum/action.c:872`), Press-Zeitpunkt
     also selbst mitschreiben → ~15 Zeilen, KMK-Parität.
  3. Gar kein Retro-Tapping (heutiger Zustand).
  Falls Retro-Tapping auf Mod-Taps bleibt: `DUMMY_MOD_NEUTRALIZER_KEYCODE`
  einplanen, sonst löst ein Retro-Tap auf GUI/Alt Menüs am Host aus.
- QMK-eigene Optionen, die es in KMK nicht gibt und die hier evtl. besser passen:
  `FLOW_TAP_TERM` (Tap-Hold während schnellem Tippen abschalten) und
  `SPECULATIVE_HOLD` (Modifier sofort beim Keydown setzen — hilft gegen die
  Trägheit bei Shift+Klick, ändert die Tap/Hold-Entscheidung aber nicht).

## Schritt 3 — Feature-Entscheidungen

- **C1 Mac-Umschaltung: `_MAC`-Layer → Magic-Keycode.** ⚠️ Entscheidung offen.
  Der heutige `_MAC`-Layer tauscht **GUI⇄Alt** und nur auf der Daumenreihe;
  er erwischt weder `NX_COPY`/`NX_PAST`/`NX__CUT`/`N3_UNDO` (`LCTL(x)`) noch die
  Modifier-Homerow im `_NAV`-Layer, und ist nicht persistent.
  QMK-Core kann das vollständig: `keycode_config()`/`mod_config()` werden in
  `quantum/keymap_common.c` beim Auflösen jeder Taste angewandt und decken damit
  **einfache Modifier, Mod-Taps *und* `LCTL(KC_C)`-artige Kombis** ab — genau die
  drei Fälle, die KMKs `cg_swap.py` nennt. Persistenz im EEPROM inklusive.
  - `CG_TOGG` (Ctrl⇄GUI) = KMKs Wahl: derselbe Daumen ist auf beiden Hosts
    „die Befehlstaste", Copy/Paste/Undo im NAV-Layer werden automatisch richtig.
  - `AG_TOGG` (Alt⇄GUI) = das, was der heutige `_MAC`-Layer tut.
  - **Empfehlung: `CG_TOGG`.**
  - **Nicht abgedeckt:** `SEND_STRING(SS_LCTL(...))`, also `FF_WORD`/`RV_WORD` —
    die laufen am Keymap vorbei. Und Wort-/Zeilennavigation ist am Mac ohnehin
    ein anderer Modifier (Opt/Cmd+Pfeil) → braucht weiterhin eine Mac-Variante
    des `_NAV`-Layers, siehe „Offen" oben.
  - **Blocker/Kosten:** braucht `MAGIC_ENABLE = yes`. **Gemessen 2026-08-04:
    global in `users/wechselbalg/rules.mk` geht nicht — sofle/rev1 läuft über den
    AVR-Flash (Build-Abbruch), sofle_choc-AVR landet bei 99 % / 88 Bytes frei.**
    Also nur board-weise aktivieren (Liatris/RP2040 und die STM32-Boards).
    Nebeneffekt: `NK_TOGG` im ADJUST-Layer ist ohne `MAGIC_ENABLE` ebenfalls tot.
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
