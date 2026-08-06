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
(Kyria, GMMK Pro, …) erweiterbar. Nicht-Split-Boards tragen `"sides": None` und
optional einen eigenen `"prompt"` mit ihrem Bootloader-Weg. **Claude sollte
dieses Skript für alle künftigen Flash-Vorgänge dieser Boards nutzen statt
manueller `make`/`qmk flash`-Aufrufe.**
```bash
python3 util/wechselbalg/flash.py --list
python3 util/wechselbalg/flash.py sofle_choc_black
python3 util/wechselbalg/flash.py sofle_choc_white --side right
python3 util/wechselbalg/flash.py k3_pro
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

## Aktueller Stand (Stand: 2026-08-06, GMMK Pro entschärft, Encoder-`_GAMING`-Fix überall)

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

- **Keychron K3 Pro ISO RGB — geflasht 2026-08-05.** Erster Flash dieses Boards
  aus dem aufgeräumten Repo; vorher lief noch Keychrons Werks-Firmware (sie
  meldete sich als `Keychron K3 Pro`, `0x3434:0x0231`). Board bootet, meldet
  sich jetzt als `Dell KB216 Wired Keyboard` (`0x413C:0x2113`, Michaels eigene
  Änderung von 2023, bewusst beibehalten). Details siehe „K3 Pro: Bootloader
  und die zwei Aussperr-Fallen" weiter unten.
  **Win/Mac-Schalter am Board bestätigt (2026-08-05)** — die angenommene
  Polarität (`active` = Mac) stimmt.
  Zweiter Flash am selben Tag: die Farbsprache (C8) ist jetzt auch hier an,
  siehe „Die Beleuchtung" im K3-Pro-Kapitel.
  **Noch nicht beurteilt:** das Tippgefühl bei `TAPPING_TERM 150`.

- **Encoder-`_GAMING`-Bug in sofle_choc und sofle/rev1 behoben (2026-08-06).**
  Der `case _GAMING:` in `encoder_update_user()` war unerreichbar (`DF()` setzt
  `default_layer_state`, der Switch las `layer_state`) — auf dem **rechten**
  Encoder kam im Gaming-Layout deshalb Lautstärke statt Pfeil hoch/runter.
  Beide Boards kompilieren, Kosten je **+4 Byte**; sofle/rev1 hat damit nur
  noch **18 Byte** frei. Details, Messwerte und die Boards ohne Befund im
  Abschnitt „Encoder + `_GAMING`" weiter unten. **Noch nicht geflasht.**

**Offen / vor dem Flashen prüfen:**
- **GMMK Pro ISO — geflasht 2026-08-06.** Zweiter Daily Driver, deshalb vorher
  vollständig gegen den K3 Pro durchgeprüft; Details in den drei Abschnitten
  „GMMK Pro" im K3-Pro-Kapitel. Geflasht wurde: Aussperr-Falle geschlossen,
  QWERTZ + **Colemak-DH**, Drehencoder belegt (Lautstärke im Basis-Layer),
  Print-Taste sendet wieder `KC_PSCR`. Die acht scharfen `_ADJUST`-Tasten sind
  damit vom Board verschwunden.
  ⚠️ **Noch nicht auf dem Board**: die Beleuchtungs-Umstellung vom selben Tag
  (Farbsprache + umschaltbarer Lichtbalken) — sie entstand erst nach dem Flash,
  weil Michael das alte Schema bemerkte. Ein weiterer Flash steht also aus.
- **K3 Pro — Keymap gesichtet und bereinigt 2026-08-04.** Zwei der früher hier
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
  - ⚠️ Seit 2026-08-05 baut das Board **`_MINE` gar nicht mehr** (Entscheidung
    Michael: Colemak-DH statt MINE, siehe unten). Der Layer-Block steht
    weiterhin in der keymap.c hinter `#ifdef WB_LAYOUT_MINE` — die obigen
    MINE-Punkte gelten also erst wieder, wenn in der Keymap-`rules.mk`
    `WB_LAYOUT_MINE` statt `WB_LAYOUT_COLEMAKDH` gesetzt wird.
- K3-Pro-Varianten ansi/jis/white bleiben ungebaut (alte LED-Tabellen-Makros).
- Idee: Mac-Variante des `_NAV`-Layers (Wort-Sprünge Opt+Pfeil statt Ctrl+Pfeil).
  Wird durch C1 (Ctrl⇄GUI-Swap) **wichtiger**, nicht überflüssig — siehe dort.

---

# K3 Pro: Bootloader und die zwei Aussperr-Fallen (2026-08-05)

## Wie man in den Bootloader kommt

**Der verlässliche Weg: Esc gedrückt halten und dabei USB einstecken.**
`BOOTMAGIC_ENABLE = yes` steht in
[keyboards/keychron/k3_pro/rules.mk](keyboards/keychron/k3_pro/rules.mk), die
Defaults sind Row 0 / Col 0, und Matrix `[0,0]` ist auf diesem Board die
Esc-Taste (nachprüfbar an Keychrons eigener default-Keymap). `bootmagic()` läuft
in `quantum_init()` **nach** `matrix_init()` (`quantum/keyboard.c:470-471`), die
Matrix ist also schon wach. Das Gerät meldet sich danach als `0483:df11`.

Bootmagic ruft dabei `eeconfig_disable()` — das EEPROM wird zurückgesetzt. Genau
das macht diesen Weg zum **Rettungsanker**: er hängt an keiner Keymap und an
keinem Layer, sondern nur an der Matrix.

**Zweiter Weg: `QK_BOOT` im `_ADJUST`-Layer**, dort auf drei Tasten — **Esc**,
**Backspace** und **B**. Nach `_ADJUST` kommt man **nur** über die dedizierte
`MO__ADJ`-Taste, und die liegt auf der physischen **End**-Taste. Kein Tri-Layer
auf diesem Board (siehe Korrektur bei C6).

Dritter Weg: Reset-Taster auf der Platine, Gehäuse öffnen. Nicht nötig.

**Im Flash-Skript hinterlegt**: `python3 util/wechselbalg/flash.py k3_pro`
druckt genau diese Anleitung und wartet dann selbständig auf das DFU-Gerät.

## Falle 1: der Win/Mac-Schiebeschalter machte das Board beim Booten tot

`dip_switch_update_kb()` in
[keyboards/keychron/k3_pro/k3_pro.c](keyboards/keychron/k3_pro/k3_pro.c) setzte
unbedingt `default_layer_set(1UL << (active ? 0 : 2))` — Keychrons eigene
`MAC_BASE`/`WIN_BASE`. In unserem Layer-Enum zeigte die 2 auf `_COLEMAKDH`, und
das war in dieser Keymap gar nicht definiert: **192 Byte `KC_NO`**. Weil
`keyboard_post_init_kb()` `dip_switch_read(true)` aufruft, passierte das bei
**jedem Boot**. Schalter auf Windows ⇒ komplett totes Board, inklusive der
End-Taste ⇒ kein `QK_BOOT` mehr, nur noch Esc-Bootmagic.

**Fix, zweiteilig:**
- `dip_switch_update_kb()` fragt jetzt zuerst `dip_switch_update_user()` und
  bricht bei `false` ab — das Muster, das Keychrons neuere Boards längst
  verwenden (`keyboards/keychron/v1/v1.c:20`, ebenso v6/v7/v8).
- Die Keymap definiert `dip_switch_update_user()` und gibt `false` zurück. Der
  Schalter fasst die Layer damit nicht mehr an, sondern schaltet **den
  Mac-Modus** — denselben Zustand wie `MAC_TOG`/`CG_TOGG`
  (`keymap_config.swap_lctl_lgui` + `swap_rctl_rgui`), an dem auch
  `WB_HOST_IS_MAC()` und die host-abhängigen Wortsprünge hängen. Der beschriftete
  Schalter tut damit endlich, was draufsteht.

Bewusst **ohne** `eeconfig_update_keymap()`: der Schalter wird bei jedem Boot
gelesen (und zwar *nach* dem `eeconfig_read_keymap()` in `quantum_init()`),
stellt den Zustand also von selbst wieder her. Kein Flash-Verschleiß, und
`MAC_TOG` bleibt als Override für die laufende Sitzung nutzbar.

⚠️ **Polarität noch nicht auf Hardware geprüft.** Angenommen ist Keychrons
Lesart (`active` = Mac). Falls vertauscht: `WB_DIP_ACTIVE_IS_MAC` in der
keymap.c umdrehen, eine Zeile.

## Falle 2: sechs Tasten auf `_ADJUST` führten in leere Layer

Das Layer-Enum legt auf Nicht-AVR **alle vier** Alternativ-Layouts an, die
K3-Pro-Keymap definierte aber nur `_QWERT` und `_MINE`. `_DVORAK`, `_COLEMAKDH`
und `_VOU` waren also vorhanden und leer — und der Umschaltblock auf `_ADJUST`
zeigte trotzdem darauf:

| Physische Taste auf `_ADJUST` | war | Wirkung |
|---|---|---|
| Q / W / E | `P_DVORK` / `P_COLMK` / `P___VOU` | `set_single_persistent_default_layer()` → **dauerhaft** totes Board, überlebt das Ausstecken |
| A / S / D | `D_DVORK` / `D_COLMK` / `D___VOU` | tot bis zum nächsten Ausstecken |

**Fix:** `OPT_DEFS += -DWB_LAYOUT_COLEMAKDH` in der Keymap-`rules.mk`. Sobald
*irgendeines* der vier von außen definiert ist, gilt ausschließlich die äußere
Wahl — die drei ungenutzten Layer entstehen gar nicht erst, und die Aliase
fallen über die `#else`-Zweige in `wechselbalg.h` auf `___NO__` zurück. Aus den
Falltüren werden also tote Tasten statt falscher Ziele.

**Dasselbe Loch hatte die GMMK Pro** (`gmmk/pro/rev1/iso`) — geschlossen am
2026-08-06, siehe den Abschnitt „GMMK Pro" unten.

## Colemak-DH statt MINE — und ein Bugfix im Wrapper

Entscheidung Michael 2026-08-05: dieses Board bekommt QWERTZ + **Colemak-DH**
statt QWERTZ + MINE. Der `_MINE`-Block bleibt hinter seinem `#ifdef` in der
keymap.c stehen, Zurückschalten ist eine Zeile in der `rules.mk`.

⚠️ **Dabei ist ein alter Fehler aufgefallen:**
`______________COLEMAKDH_R2_________________` begann seit dem Anlegen der Datei
(`bf83436d68`, 2023-04-11) mit `DE_H` statt `DE_M`. Colemak-DH hatte damit
**kein M** und **H doppelt** (auch in `R3`). Das betrifft nicht nur dieses
Board: laut AVR-Gating ist Colemak-DH auf **sofle/rev1, sofle_choc (weiß) und
Kyria das einzige Alternativ-Layout**. Behoben in
[users/wechselbalg/wrappers.h](users/wechselbalg/wrappers.h).

Neu dort außerdem die 12/13-breiten Blöcke **`COLMAK_1/2/3`** für die großen
ISO-Boards, analog zu `QWERTY_1/2/3` und `MINE___1/2/3`. Zwei bewusste
Abweichungen von den 6er-Hälften, beide weil das ISO-Board an der Stelle eine
*zusätzliche* Taste hat statt einer Pinky-Doppelrolle: letzte Taste in Reihe 1
`NUM__UE` statt `MO__NUM` (auf dem Split trägt die Pinky-Spalte den
Layer-Zugang; hier ist es die physische Ü-Taste, und `MO__NUM` sitzt ohnehin auf
NUBS und Fn) und in Reihe 2 `SYM__AE` statt `SYM_HSH` (sonst wäre `#` doppelt
und Ä unerreichbar). Begründung steht im Kommentar an der Definition.

## Bluetooth ist weg — und bleibt es vorerst

Der USB-only-Build war schon vorher gesetzt
(`# OPT_DEFS += -DKC_BLUETOOTH_ENABLE`), aber auf dem Board lief bis 2026-08-05
noch Keychrons Werks-Firmware **mit** Bluetooth. Nach diesem Flash funktioniert
nur noch die Kabel-Stellung des Seitenschalters.

Testweise angeschaltet (2026-08-05): das einvendorte Modul unter
`keyboards/keychron/bluetooth/` **kompiliert nicht** gegen aktuelles QMK —
`setPinInputHigh` entfernt, `eeconfig_read_keymap()` hat ein Argument bekommen,
die Host-Driver-Signaturen (`report_nkro_t`/`report_mouse_t`/`report_extra_t`)
haben sich verschoben. Dieselbe API-Drift, die in `k3_pro.c`/`matrix.c` schon
nachgezogen wurde, nur in 13 weiteren Dateien — und `bluetooth.c` war erst die
erste. **TODO (Michael, 2026-08-05): nachziehen oder neu bauen**, aber als
eigenes Vorhaben, nicht nebenbei.

Nebenbefund für den Fall, dass es angegangen wird: in
[k3_pro.c](keyboards/keychron/k3_pro/k3_pro.c) steht bei `ENABLE_FACTORY_TEST`
ein verirrter Schrägstrich (`/#    include "factory_test.h"`). Stört heute
nicht, weil das `#ifdef` nie greift — beim BT-Build aber sofort ein Syntaxfehler.

## Die Beleuchtung: Farbsprache angeschaltet (zweiter Flash, 2026-08-05)

Bis hierhin lief auf dem Board noch Keychrons Standard-Animation — die
Farbsprache aus C8 war nie eingeschaltet, weil sie ausdrücklich opt-in ist.
Michael hat sie angefordert; `WB_RGB_LANGUAGE = yes` steht jetzt in der
Keymap-`rules.mk`. Beim Portieren auf das zweite Board kamen drei Dinge dazu:

### ⚠️ Ohne eigene `RGB_MATRIX_DEFAULT_*` kollidiert die Palette mit sich selbst

Das Grundleuchten kommt aus der laufenden Hue/Sat-Konfiguration. Die Sofle Choc
setzt die Startwerte ausdrücklich (`SOLID_COLOR`, Hue 25 / Sat 135 / Val 40 —
das warme Cremeweiß); dem K3 Pro fehlte das. QMKs Vorgaben sind **Hue 0 +
Sat 255** und `DEFAULT_VAL = RGB_MATRIX_MAXIMUM_BRIGHTNESS`, also **knallrot bei
voller Helligkeit** — ausgerechnet die Farbe, die in der Palette `_ADJUST`
bedeutet. Steht jetzt in der neuen
[keymaps/wechselbalg/config.h](keyboards/keychron/k3_pro/iso/rgb/keymaps/wechselbalg/config.h).

**Merkregel für jedes weitere Board:** die Farbsprache bringt ihr Grundleuchten
*nicht* mit, sie liest es. Wer `WB_RGB_LANGUAGE` anschaltet, muss die vier
`RGB_MATRIX_DEFAULT_*`-Zeilen mitliefern.

Dort werden auch die 21 Animationen aus
`keyboards/keychron/k3_pro/iso/rgb/config.h` per `#undef` abgeräumt (die
Board-`config.h` bleibt unangetastet — `default` und `via` teilen sie sich mit
uns). Sie wären ohnehin unsichtbar, weil die Farbsprache jeden Frame über alle
85 Tasten-LEDs schreibt; übrig blieben nur `RGB_MOD`/`RGB_RMOD` auf `_ADJUST`,
die zwischen Modi umschalten, die man nicht sieht. **Gemessen:** 42724 → 37036
Byte, und im ELF ist kein Animations-Symbol mehr übrig.

### ⚠️ Bugfix: die ISO-LED-Tabelle war in Reihe 4 um eine Spalte verschoben

In `g_led_config.matrix_co` ([rgb.c](keyboards/keychron/k3_pro/iso/rgb/rgb.c))
stand `NO_LED` eine Spalte zu weit links: **`[4,11]` — die Minus-Taste — hatte
gar keine LED**, und LED 71 hing an Spalte 12, wo keine Taste sitzt.

Die Reihe ist erkennbar aus der ANSI-Variante abgeleitet (dort korrekt: deren
Reihe 4 hat wegen der 2.25u-LShift keine Spalte 1), beim Einfügen der
ISO-NUBS-Taste wurde die Lücke nicht mitgeschoben. Zwei unabhängige Belege:
LED 71 steht im Positions-Array auf `{168,51}`, was bei 224/15 px pro Einheit
genau `x=11.25` aus der `keyboard.json` ist — also `[4,11]`; und der direkte
ANSI/ISO-Vergleich zeigt das Verschiebemuster.

**Warum das nie auffiel:** Animationen rechnen nach x/y und ignorieren
`matrix_co`. Erst die Farbsprache liest die Zuordnung — dort wäre die Taste
schlicht dunkel geblieben. Wer eine LED-Tabelle vom Nachbar-Layout ableitet,
sollte sie gegen die `layout`-Liste prüfen; das sind zehn Zeilen Python.

### Ü und Ä sind keine Layer-Taps mehr (2026-08-05)

Auf den Splits muss die Pinky-Spalte den Layer-Zugang mittragen, deshalb sind
dort `NUM__UE` (Ü tippen, NUM halten) und `SYM__AE`. Auf den großen ISO-Boards
ist das eine **Dublette mit Kosten**: jeder Umlaut läuft durch die
Tap-Hold-Auflösung über `TAPPING_TERM` (150 ms) — für einen Layer, der ohnehin
auf einer eigenen Taste liegt. Hier hat jedes Overlay einen dedizierten Zugang:

| Layer | dedizierte Taste auf dem K3 Pro |
|---|---|
| `_SYM` | Caps (`MO__SYM`) |
| `_NAV` | RAlt (`MO__NAV`) |
| `_NUM` | NUBS `<` **und** Fn — doppelt |
| `_ADJUST` | End (`MO__ADJ`) |

Deshalb senden `QWERTY_1`/`QWERTY_2`, `COLMAK_1`/`COLMAK_2` und `GAMING_2` jetzt
blankes `DE___UE`/`DE___AE`. **Die Splits bleiben unangetastet**, weil das reine
Großboard-Blöcke sind — die Splits setzen direkt die 6er-Hälften
`QWERTY_R1`/`QWERTY_R2` ein, und die tragen weiterhin die Doppelrolle.
Belegt: alle vier AVR-Binaries blieben nach der Änderung bytegleich
(22 / 148 / 1212 / 896 Byte frei).

Dass die R1/R2-Hälften in diesen Blöcken jetzt **ausgeschrieben** statt
eingesetzt sind, ist genau der Preis dafür — und der Grund steht als Kommentar
an `QWERTY_1`.

- **`GAMING_2` musste mit** (Michael, 2026-08-05): es zog sich `QWERTY_R2`
  herein, also hätte `_GAMING` blankes Ü aus `QWERTY_1` neben einem Ä mit
  Layer-Tap gehabt.
- **Bewusst geblieben:** `SYM_HSH` auf der ISO-#-Taste (steht direkt in den
  Board-Keymaps, nicht in den Blöcken), `NAV_TAB` und `NAV_SPC` — keine
  Buchstaben und der klassische Griff; das `'*'` der `chordal_hold_layout`
  sitzt auf der Leertaste.
- **`MINE___1`/`MINE___2` nicht angefasst.** Dort stehen an denselben Stellen
  `NUM__SS` (ß) und `SYM___Z` (z) — dasselbe Muster, aber `_MINE` ist auf
  diesem Board nicht einkompiliert. Beim Zurückschalten auf `WB_LAYOUT_MINE`
  mitziehen.
- **GMMK Pro zieht mit**, weil sie sich `QWERTY_1/2` und `GAMING_2` teilt.
  Geprüft am ELF: `NUM__UE`/`SYM__AE` kommen dort im gesamten `keymaps`-Array
  **nicht mehr vor**. Auf dem Board ist das seit dem Flash vom 2026-08-06.

### Kein Tri-Layer-Hinweis auf diesem Board

`-DWB_NO_TRI_LAYER_HINT`, siehe C8 weiter unten. Ohne das würde die NUM-Taste
rot leuchten, solange `_NAV` gehalten wird, und damit einen Weg nach `_ADJUST`
behaupten, den es hier nicht gibt.

**Entschieden 2026-08-06 (Michael): die großen ISO-Boards bekommen kein
Tri-Layer.** Zur Wahl stand, ihnen `update_tri_layer_state` zu geben statt den
Hinweis abzuschalten — das hätte einen zweiten Weg nach `_ADJUST` gebracht, also
Redundanz gegen genau die Aussperr-Fallen von oben. Dagegen sprach, dass es
ändert, was `MO__NAV`+`MO__NUM` zusammengehalten tun; die Redundanz war den
Eingriff in einen eingespielten Griff nicht wert.

Damit steht fest, und zwar für **K3 Pro und GMMK Pro gleichermaßen**:

- Nach `_ADJUST` führt auf beiden Boards **allein die `MO__ADJ`-Taste** (physisch
  End). Sie liegt auf jedem Basis-Layout, der Weg überlebt also das Umschalten
  zwischen QWERTZ und Colemak-DH — am Binary beider Boards geprüft.
- Der Rettungsanker bleibt Esc-Bootmagic, unabhängig von jeder Keymap.
- **`WB_NO_TRI_LAYER_HINT` ist damit Pflicht, nicht Geschmackssache**, sobald
  eines der beiden die Farbsprache bekommt. Beim K3 Pro gesetzt; bei der GMMK
  Pro mitzusetzen, falls sie je von ihren Animationen wegwechselt.

### Nebenbefund

`CAPS_LOCK_INDEX`, `DIM_CAPS_LOCK` und `RGB_MATRIX_BRIGHTNESS_TURN_OFF_VAL` in
der Board-`config.h` sind in diesem Build **tote Namen** — sie werden nur vom
nicht kompilierten Bluetooth-`indicator.c` gelesen. Dieselbe Sorte Fund wie die
`RGB_DISABLE_AFTER_TIMEOUT`-Zeilen bei der Sofle Choc. Caps Lock hat trotzdem
eine Anzeige: die eigene LED an `LED_CAPS_LOCK_PIN` (A0).

## Wie das alles ohne Hardware geprüft wurde

Nicht am Quelltext, sondern am **fertigen Binary**: ein Skript liest das echte
`keymaps`-Array aus dem ELF (`arm-none-eabi-nm` → Symbol `keymaps`, Offset über
`objdump -h`) und ordnet es mit der `layout`-Liste aus der `keyboard.json`
physisch an. Damit sind Wrapper-Breiten, `#ifdef`-Zweige und LTO schon
aufgelöst — genau die Stellen, an denen Raten schiefgeht. So ist belegt, dass
Colemak-DH alle 26 Buchstaben hat, dass Ü/Ä/# je auf ihrer eigenen ISO-Taste
liegen und dass die sechs Falltüren auf `_ADJUST` jetzt `KC_NO` sind.
Dasselbe Vorgehen wie bei C8 auf der Sofle Choc; bei Zweifeln wieder so machen,
das ist billiger als flashen.

Ergänzend: ob ein `#define` wirklich im Build ankommt, sagt
`tr ' ' '\n' < .build/obj_<target>/cflags.txt | grep -x -- -DDIP_SWITCH_ENABLE`.

## GMMK Pro: dieselbe Falle, geschlossen 2026-08-06

Die GMMK Pro (`gmmk/pro/rev1/iso`) definierte als Basis-Layout **nur `_QWERT`**.
Auf Nicht-AVR legt das Enum aber alle vier Alternativ-Layouts an — hier waren
also **vier** Layer komplett leer (je 176 Byte `KC_NO`; die Matrix ist 11×8, nicht
6×16 wie beim K3 Pro) und entsprechend **acht** Tasten auf `_ADJUST` scharf:

| Physisch auf `_ADJUST` | war | Wirkung |
|---|---|---|
| Q / W / E / R | `P_DVORK` / `P_COLMK` / `P___VOU` / `P__MINE` | `set_single_persistent_default_layer()` → **dauerhaft** totes Board |
| A / S / D / F | `D_DVORK` / `D_COLMK` / `D___VOU` / `D__MINE` | `DF()`, tot bis zum Ausstecken |

**Fix, wie beim K3 Pro:** `OPT_DEFS += -DWB_LAYOUT_COLEMAKDH` in der
Keymap-[rules.mk](keyboards/gmmk/pro/rev1/iso/keymaps/wechselbalg/rules.mk).
Dazu — Entscheidung Michael 2026-08-06 — bekommt das Board **Colemak-DH als
zweites Basis-Layout**, genau wie der K3 Pro: ein `[_COLEMAKDH]`-Block aus den
12/13-breiten ISO-Wrappern `COLMAK_1/2/3`, zeilenweise identisch mit `_QWERT`.
Aus sechs Falltüren werden damit tote Tasten, die anderen zwei (W und S) zeigen
auf einen **echten** Layer.

**Kein Gegenstück zu Falle 1**: die GMMK Pro hat keinen Dip-Switch, und im
Board-Code steht kein `default_layer_set` (nachgeprüft per grep über
`keyboards/gmmk/pro/`). Der Rettungsanker ist derselbe: `BOOTMAGIC_ENABLE` ist
an, und `BOOTMAGIC_ROW 1` / `BOOTMAGIC_COLUMN 3` ist laut `keyboard.json`
Layout-Index 0 — also **Esc gedrückt halten und einstecken**.

Nebenbei: der `encoder_map` in der keymap.c hat jetzt auch einen
`[_COLEMAKDH]`-Eintrag. Er ist heute toter Code (`ENCODER_MAP_ENABLE` ist in der
rules.mk auskommentiert, per `cflags.txt` bestätigt) — aber wenn er je
angeschaltet wird, wäre der Encoder auf dem Colemak-Layer sonst stumm.

**Am Binary geprüft** (Methode wie oben), vorher/nachher:

| | vorher | nachher |
|---|---|---|
| `keymaps` | 1760 Byte, 10 Layer | 1232 Byte, 7 Layer |
| leere Layer | `_DVORAK`/`_COLEMAKDH`/`_MINE`/`_VOU`, je 0/88 Tasten | keine |
| `_ADJUST` belegt | 70/88 | 64/88 (die 6 Falltüren sind `KC_NO`) |
| Firmware | 44228 Byte | 43676 Byte |

`_COLEMAKDH` hat 84/88 belegte Tasten wie `_QWERT`, alle 26 Buchstaben je einmal
(inklusive des reparierten `M`), Ü/Ä/# je auf ihrer eigenen ISO-Taste. Ü und Ä
senden dabei **blank**, ohne Layer-Tap — der neue Layer erbt das über
`COLMAK_1/2` automatisch mit (Änderung vom 2026-08-05, siehe dort); im gesamten
`keymaps`-Array kommen `NUM__UE`/`SYM__AE` null mal vor. `QK_BOOT`
liegt weiterhin auf Esc, Backspace und B; nach `_ADJUST` führt `MO(6)` auf der
**End**-Taste, und zwar auf `_QWERT` *und* auf `_COLEMAKDH` — der Weg in den
Bootloader überlebt das Umschalten also.

## GMMK Pro: Flash-Vorbereitung als Daily Driver (2026-08-06)

Michael benutzt das Board als zweiten Daily Driver und wollte es vor dem Flashen
vollständig durchgeprüft haben. Der Vergleich lief wieder am Binary, aber diesmal
**gegen den K3 Pro**: beide ELFs auslesen, physisch anordnen, Reihe für Reihe
diffen (Skript im Scratchpad, nicht eingecheckt).

**Befund: alle sieben Layer sind auf den Reihen 1–5 zeichengleich mit dem
K3 Pro.** Die beiden ISO-Boards sind also bereits vollständig aneinander
ausgerichtet — die gemeinsamen Wrapper leisten genau das. Unterschiede gibt es
nur in **Reihe 0**, und die sind baulich: der K3 Pro hat dort 16 Tasten
(`KC_MAIL`/`KC_CALC`/`RGB_MOD` rechts), die GMMK Pro 15, weil ihre rechte obere
Ecke der **Drehencoder** ist.

Scheinbare Unterschiede, die keine sind (nachgeprüft in `keymap_german.h`):
`DE_SS` **ist** `KC_MINS`, `DE_ACUT` **ist** `KC_EQL`, `DE_PLUS` **ist**
`KC_RBRC`. Die Zahlenreihe und die `+`-Taste sind auf beiden Boards derselbe
Keycode, nur unterschiedlich geschrieben.

### Der Drehencoder drehte ins Leere

`ENCODER_ENABLE` ist über `keyboard.json` an, aber `ENCODER_MAP_ENABLE` stand in
der Keymap-`rules.mk` auskommentiert — und der `encoder_map` in der keymap.c
hing hinter `#if defined(ENCODER_MAP_ENABLE)`. Die Drehung wurde also eingelesen
und verworfen; **nur der Druck tat etwas** (`KC_MUTE` aus Reihe 0).

Jetzt ein `encoder_update_user()` nach dem Vorbild des **rechten** Sofle-Encoders
(Entscheidung Michael: Lautstärke gehört ins Basis-Layer):

| Layer | Drehung | Druck |
|---|---|---|
| `_QWERT` / `_COLEMAKDH` | **Lautstärke** | Mute |
| `_SYM` / `_NAV` / `_NUM` | Tab / Shift+Tab | Mute |
| `_GAMING` | Pfeil hoch/runter | Mute |
| `_ADJUST` | RGB-Helligkeit | **RGB an/aus** |

**Bewusst `encoder_update_user()` statt `ENCODER_MAP_ENABLE`:** die Helligkeit
muss über die `*_noeeprom`-Varianten laufen, sonst schreibt jede einzelne
Rastung ins EEPROM. Ein `encoder_map` könnte nur `RGB_VAI`/`RGB_VAD` eintragen —
also genau die EEPROM-schreibende Variante. Dieselbe Überlegung wie bei C7 auf
der Sofle Choc. Der tote `encoder_map` ist entfallen, damit es nicht zwei
Quellen der Wahrheit gibt.

⚠️ **Dabei ein Fallstrick, der auch die Sofle betrifft:** `_GAMING` wird per
`DF()` betreten und liegt damit in `default_layer_state`, **nicht** in
`layer_state` — zwei getrennte Variablen (`quantum/action_layer.c:12` und
`:101`). Ein `switch (get_highest_layer(layer_state))` erreicht den
`_GAMING`-Zweig also **nie**. Hier gelöst mit

```c
const uint8_t layer = layer_state ? get_highest_layer(layer_state)
                                  : get_highest_layer(default_layer_state);
```

Die Sofle-Choc-Keymap hatte denselben toten Zweig — auf ihrem linken Encoder
folgenlos (beide Zweige machen PgUp/PgDn), auf dem **rechten** nicht: gedacht
ist hoch/runter, es kam Lautstärke. **Nachgezogen am 2026-08-06** in sofle_choc
und sofle/rev1, mit Messwerten und den geprüft nicht betroffenen Boards im
Abschnitt „Encoder + `_GAMING`" weiter unten.

### Print-Taste war tot

Die Taste links neben dem Encoder trug `RN_CODE` — einen Custom-Keycode, dessen
einziger Handler in dieser keymap.c unter `#ifdef CONSOLE_ENABLE` steht. Der
Build hat `CONSOLE_ENABLE` aus und setzt `-DNO_DEBUG`; die Taste tat also
**nichts**. Jetzt `KC_PSCR` — sie sendet, was auf ihr steht (Entscheidung
Michael, dieselbe Regel wie bei den ISO-Extratasten des K3 Pro). `RN_CODE` kommt
damit in keiner Keymap mehr vor; der Enum-Eintrag bleibt trotzdem stehen, weil
Entfernen alle folgenden Werte verschiebt (das war die Ursache des
`FN_EXIT`-Bugs).

Ebenfalls entfallen: ein `keyboard_post_init_user()`, das `debug_enable`,
`debug_matrix` und `debug_keyboard` auf `true` setzte. Mit `-DNO_DEBUG` sind die
Ausgaben nicht einkompiliert, die Zuweisungen liefen ins Leere.

### Mac-Modus ohne Schiebeschalter

Die GMMK Pro hat kein Gegenstück zum Win/Mac-Schalter des K3 Pro — der Modus
läuft hier **allein** über `MAC_TOG` (= `CG_TOGG`) auf `_ADJUST`, physisch auf
der **C**-Taste („C wie Cmd"). Das ist kein Nachteil, sondern eher der sauberere
Weg: `CG_TOGG` schreibt ins EEPROM und **überlebt damit den Neustart**, während
der K3-Pro-Schalter bei jedem Boot neu gelesen wird und `MAC_TOG` dort nur für
die laufende Sitzung überschreibt.

⚠️ Zwei Folgen davon: die GMMK Pro hat **keine Anzeige** des Modus (kein OLED,
und die Farbsprache ist hier nicht an) — der Zustand ist nur am Verhalten von
Ctrl/Cmd erkennbar. Und **Esc-Bootmagic setzt das EEPROM zurück**, der Mac-Modus
steht nach jedem Flash-Vorgang über diesen Weg wieder auf PC.

### Am Binary geprüft

Zusätzlich zur Layer-Tabelle wurde der Encoder-Handler **im Maschinencode**
nachgesehen, weil LTO ihn in `main` inlinet und er deshalb kein eigenes Symbol
hat. Alle fünf Zweige sitzen richtig: `rgb_matrix_*_val_helper` mit Argument `0`
(= kein EEPROM-Schreiben), `0x51`/`0x52` für Down/Up, `0x2b`/`0x22b` für
Tab/Shift+Tab, `0xaa`/`0xa9` für die Lautstärke — und die `tbb`-Sprungtabelle
deckt genau die Layer 2–6 ab. Firmware 43676 → **43808 Byte**.

## GMMK Pro: Farbsprache + Lichtbalken als Statusanzeige (2026-08-06)

Nach dem ersten Flash fiel Michael auf, dass die Beleuchtung „nach dem alten
Schema" aussah — zu Recht: die Farbsprache (C8) ist **opt-in pro Board**, und
für die GMMK Pro war sie nie eingeschaltet. Das Board lief auf QMKs
Standard-Satz von **43 Animationen** (die `keyboard.json` gibt keine Liste vor,
also aktiviert QMK seine Vorgaben). Genau dieselbe Lage wie beim K3 Pro vor dem
2026-08-05.

**Die LED-Tabelle ist hier sauber** — kein Versatz wie beim K3 Pro. Geprüft
gegen die `layout`-Liste: 84 Tasten, 83 Tasten-LEDs, keine Dublette, keine LED
auf einer Nicht-Taste. Genau eine Taste hat keine LED: Matrix `(0,1)`, der
Encoder-Knopf — und der hat physisch keine.

### Der Lichtbalken: was dieses Board kann und die anderen nicht

Die GMMK Pro hat **99 LEDs**: 83 unter Tasten und **16 im seitlichen
Lichtbalken**, die keiner Matrixposition zugeordnet sind (`flags: 2`,
`LED_FLAG_UNDERGLOW`). `rgb_language.c` läuft über die **Matrix** und schreibt
darum ausschließlich die 83 Tasten-LEDs — der Balken bleibt so, wie ihn der
laufende Effekt gemalt hat.

Daraus ergibt sich etwas, das Sofle und K3 Pro nicht können: **die Tasten
sprechen die Farbsprache, während der Balken weiterläuft.** Deshalb bleiben hier
alle Animationen einkompiliert (kein einziges `#undef`, anders als beim K3 Pro);
`RGB_MOD`/`RGB_RMOD` auf `_ADJUST` wechseln dann sichtbar die Balken-Animation,
statt zwischen unsichtbaren Modi umzuschalten. Die rund 6 KB sind bei 256 KB
Flash belanglos.

Das **Grundleuchten der Tasten** hängt trotzdem nicht am Effekt: `rgb_language.c`
baut es aus `rgb_matrix_get_hue()/get_sat()`, also aus der *Konfiguration*. Eine
laufende Animation färbt darum den Balken, nicht die Tasten.

⚠️ Die `RGB_MATRIX_DEFAULT_*`-Zeilen sind trotzdem Pflicht (neue
[config.h](keyboards/gmmk/pro/rev1/iso/keymaps/wechselbalg/config.h), Hue 25 /
Sat 135 / Val 40 wie die anderen Boards) — ohne sie gilt QMKs Vorgabe Hue 0 /
Sat 255 / volle Helligkeit, also knallrot. Dieselbe Lehre wie beim K3 Pro.

### Umschaltbar: Animation ↔ Statusanzeige

Michaels Vorgabe: „grundsätzlich Animation, im Zweifel aber Funktion vor Optik."
Deshalb trägt der Balken wahlweise **dieselben Zustände wie die Status-LED der
Liatris-Sofle**, aus derselben Palette:

| | Balken |
|---|---|
| Caps Word | weiß |
| Layer Lock | cyan |
| Jiggler | gelbgrün |
| Mac-Modus | azur (gedämpft) |
| sonst | **aus** |

Umschalten mit `A_SIDEBR` auf `_ADJUST`, physisch die **Print**-Taste direkt
neben dem Encoder — die ganze obere rechte Ecke ist dort „Beleuchtung": Print
schaltet den Balken um, der Encoder-Druck die Beleuchtung ganz aus/an, die
Encoder-Drehung regelt die Helligkeit. Der Zustand ist **persistent im EEPROM**
(`eeconfig_read_user()`), Vorgabe ist die Animation.

⚠️ Auf diesem Board ist der Status-Modus die **einzige** Anzeige des Mac-Modus —
weder OLED noch Win/Mac-Schalter vorhanden.

### Zwei Umbauten im Userspace

1. **`status_state.{h,c}`** (neu): Palette, Flags, `layer_lock_set_user()` und
   `wb_status_flags_local()` sind aus `status_led.c` herausgezogen. Es gibt jetzt
   zwei Anzeigen derselben Zustände, und die Palette gehört an eine Stelle.
   Eingeschaltet über `WB_STATUS_LED` **oder** `WB_SIDEBAR_STATUS`.
2. **`wb_rgb_extra_leds()`** (neu, schwach in `rgb_language.c`): Haken für LEDs
   ohne Matrixposition, gerufen am Ende der Farbsprache. Die schwache Vorgabe
   tut bewusst nichts — ohne Board-Code animiert der Balken einfach weiter.

⚠️ **Byte-Gleichheit war nicht zu halten, Größengleichheit schon.** Der
`status_state`-Umbau verschiebt Code zwischen Übersetzungseinheiten, und das
ordnet unter LTO neu: der Liatris-Build unterscheidet sich in 217 von 49708
Bytes. Belegt wurde die Folgenlosigkeit stattdessen über **identische Größe**
(49708 → 49708), **identische Instruktionszahl** (19320) und einen normalisierten
Disassembly-Vergleich: von 196 abweichenden Zeilen sind 148 Sprungziele
(Linker-Veneers), der Rest ist ein verschobener Block. `wb_status_color()` wurde
dafür **zeichengleich** übernommen, inklusive der frühen `return`s — ein
Umschreiben auf if/else kostete prompt 8 Byte Differenz.

**Alle sechs Boards nachgemessen, keines hat sich durch diese Änderung
gewandelt.** Stand nach dem Zusammenführen mit dem `_GAMING`-Encoder-Fix
(3c5032d689, der die AVRs je 4 Byte kostet): sofle/rev1 **18 Byte frei**,
sofle_choc 144, kyria 1212, lotus58 896, K3 Pro 37036, Liatris 49716. Die
Userspace-Umbauten hier kosten davon **null** — der schwache Haken verschwindet
per LTO, und `status_state.c` ersetzt nur, was vorher in `status_led.c` stand.
Nur die GMMK Pro wächst: 43808 → **44936 Byte**.

⚠️ **Diese Beleuchtungs-Umstellung ist noch NICHT auf dem Board.** Alles davor
(Aussperr-Falle, Colemak-DH, Encoder, Print-Taste) wurde am 2026-08-06 geflasht;
die Farbsprache entstand erst danach, weil Michael am frisch geflashten Board
das alte Animationsschema bemerkte. Ein weiterer Flash steht aus.

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

> ⚠️ **sofle/rev1 hat 22 Bytes Luft** (Stand 2026-08-04; seit dem Encoder-Fix
> vom 2026-08-06 sind es **18**). Das baut heute, aber der nächste
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
  über ADJUST → `D_QWERT` raus, das bleibt so.
  ⚠️ **Korrektur 2026-08-05:** hier stand, ADJUST sei auf diesen beiden Boards
  per `MO__NAV`+`MO__NUM` erreichbar. **Stimmt nicht** — Tri-Layer ist nur in
  den Split-Keymaps konfiguriert (`update_tri_layer_state` in sofle_choc,
  sofle/rev1; `TRI_LAYER_ENABLE` bei der Kyria), GMMK Pro und K3 Pro haben es
  nicht. Auf dem K3 Pro führt allein die dedizierte **`MO__ADJ`-Taste** dorthin,
  und die liegt auf der physischen **End**-Taste (`[4,15]`).
  Dass das auch aus `_GAMING` heraus funktioniert, ist kein Zufall, sondern
  `layer_switch_get_layer()`: `_GAMING` hat dort `KC_TRNS`, und die Suche fällt
  am Ende auf Layer 0 zurück — wo `MO(_ADJUST)` steht.

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

## Reihenfolge für die nächste Session (Stand 2026-08-05)

**Die KMK→QMK-Angleichung ist inhaltlich durch.** C1–C8, C7 und die Status-LED
sind umgesetzt und am 2026-08-04 auf der schwarzen Sofle Choc bestätigt,
inklusive Helligkeitskurve und Split-Sync der Statusflags. Was bleibt:

1. **K3 Pro am Board nachprüfen** (2026-08-05 dreimal geflasht, siehe eigenes
   Kapitel): ob Colemak-DH mit dem korrigierten `M` sauber tippt, und ob die
   Farbsprache in der Praxis trägt — besonders die Helligkeit
   (`RGB_MATRIX_DEFAULT_VAL 40`, änderbar per `RGB_VAI`/`RGB_VAD` auf `_ADJUST`
   ohne Neuflashen) und ob die Minus-Taste jetzt wirklich leuchtet.
   Der Win/Mac-Schalter ist bestätigt.
2. **GMMK Pro am 2026-08-06 geflasht** — blankes Ü/Ä, entschärfte
   `_ADJUST`-Tasten, Colemak-DH, Encoder und `KC_PSCR` sind auf dem Board.
   ⚠️ **Ein weiterer Flash steht aus**: die Beleuchtungs-Umstellung desselben
   Tages (Farbsprache + umschaltbarer Lichtbalken) entstand erst danach.
   Am Board zu beurteilen: Grundhelligkeit (`RGB_MATRIX_DEFAULT_VAL 40`) und ob
   der Statusbalken im Alltag trägt.
3. **Tippgefühl bei `TAPPING_TERM 150`** im Alltag beurteilen — der einzige
   Punkt, der sich nur über längere Benutzung zeigt. Bei versehentlichen
   Modifiern liegen `FLOW_TAP_TERM` und `SPECULATIVE_HOLD` dokumentiert bereit
   (siehe unten). Auf dem K3 Pro sind die Umlaute seit 2026-08-05 aus der
   Tap-Hold-Auflösung heraus, das ändert dort das Bild.
4. ~~**GMMK Pro: dieselbe Aussperr-Falle schließen**~~ — **erledigt 2026-08-06**,
   siehe eigenen Abschnitt im K3-Pro-Kapitel. Am 2026-08-06 geflasht.
5. ~~**Sofle Choc: toter `_GAMING`-Zweig im Encoder-Handler**~~ — **erledigt
   2026-08-06** in sofle_choc **und** sofle/rev1 (dieselbe Stelle, gleicher
   Fehler), siehe „Encoder + `_GAMING`" unten. Kostet je 4 Byte;
   ⚠️ **sofle/rev1 hat danach nur noch 18 Byte frei.** Noch nicht geflasht.
6. Kyria-Handedness (`chordal_hold_layout`), dann ganz zuletzt das OLED.
7. **Keychron-Bluetooth-Modul nachziehen** (eigenes Vorhaben, siehe K3-Pro-Kapitel).

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
Seit 2026-08-05 ist sie auch auf dem **K3 Pro** an (Michael hat sie dort
angefordert); **GMMK Pro bleibt vorerst bei ihren Animationen.** Was beim
Portieren auf ein zweites Board nötig war, steht im K3-Pro-Kapitel weiter oben.

**Neuer Schalter `WB_NO_TRI_LAYER_HINT`** (per `OPT_DEFS` in der
Keymap-`rules.mk`): schaltet den Tri-Layer-Hinweis ab. Nötig auf Boards ohne
Tri-Layer — dort führt allein eine eigene `MO__ADJ`-Taste nach `_ADJUST`, und
der Hinweis würde einen Weg behaupten, den es nicht gibt. Gesetzt für den
K3 Pro; bei der GMMK Pro **zwingend mitzusetzen**, falls sie die Farbsprache je
bekommt — dass beide Boards kein Tri-Layer haben, ist seit 2026-08-06 eine
Entscheidung und kein Zufall mehr (siehe K3-Pro-Kapitel).

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

### ⚠️ Encoder + `_GAMING`: `layer_state` ≠ `default_layer_state` — behoben 2026-08-06

`encoder_update_user()` schaltete in **sofle_choc** und **sofle/rev1** per
`switch (get_highest_layer(layer_state))`. Der `case _GAMING:` darin war
**unerreichbar**: `_GAMING` wird per `DF()` betreten und steht damit in
`default_layer_state`, nicht in `layer_state` — zwei getrennte Variablen
(`quantum/action_layer.c:12` bzw. `:101`). Ist nur das Default-Layer gesetzt,
ist `layer_state` schlicht `0` und `get_highest_layer()` liefert `0`, der
Switch landete also immer im `default:`-Zweig.

- **Linker Encoder:** folgenlos, `_GAMING` und `default` machen beide PgUp/PgDn.
- **Rechter Encoder:** gedacht war Pfeil hoch/runter, tatsächlich kam
  **Lautstärke** — der eigentliche Fehler, den man am Board merkt.

**Der Fix** (identisch in beiden Keymaps, dasselbe Muster wie im
GMMK-Pro-Handler):

```c
const uint8_t layer = layer_state ? get_highest_layer(layer_state)
                                  : get_highest_layer(default_layer_state);
```

Einmal oben in der Funktion, beide Encoder teilen sich den Wert. Ein
gehaltener/getoggelter Layer gewinnt, sonst zählt das Default-Layer — das ist
zugleich für die Basis-Layouts richtig, weil deren Index dort ebenso steht.

⚠️ **Nicht** `layer_state | default_layer_state` nehmen (was die OLED-`case`s
tun): im Layer-Enum steht `_GAMING` **hinter** `_SYM`/`_NAV`/`_NUM`
([wechselbalg.h](users/wechselbalg/wechselbalg.h)), das ODER würde also bei
gehaltenem `_NAV` auf Gaming-Default trotzdem `_GAMING` liefern. Für die OLEDs
ist das nur kosmetisch (sie zeigen dann „GAME" statt „NAV"), für die Encoder
wäre es falsches Verhalten. Falls die OLED-Anzeige eines Tages stört: dort
dasselbe Ternary einsetzen — auf AVR kostet es aber wieder Bytes.

**Flash-Kosten, sauber vorher/nachher gemessen (2026-08-06):**

| Board | vorher | nachher | Δ |
|---|---|---|---|
| sofle/rev1 | 28650 / 22 Byte frei | 28654 / **18 Byte frei** ⚠️ | +4 |
| sofle_choc (weiß, AVR) | 28524 / 148 frei | 28528 / 144 frei | +4 |
| sofle_choc (schwarz, Liatris) | — | baut (UF2) | irrelevant |

Nachgemessen und **nichts zu holen**: den auf dem linken Encoder jetzt
redundanten `case _GAMING:` zu streichen ergibt exakt dieselben 28654 Byte —
LTO faltet die identischen Zweige ohnehin zusammen. Der Zweig bleibt deshalb
stehen, das hält beide Encoder und beide Boards symmetrisch lesbar.

**Nicht betroffen:**
- **Kyria** — `ENCODER_ENABLE = no` in ihrer Keymap-`rules.mk`, kein
  `encoder_update_user()` in der keymap.c. Nichts zu tun.
- **Lotus58** (stillgelegt) — ihr Handler schaltet per `IS_LAYER_ON(_NUMB)`,
  und `_NUMB` ist ein Momentary-Layer, steht also korrekt in `layer_state`.
- **K3 Pro** — hat gar keinen Encoder.
- **GMMK Pro** — dort ist dasselbe Ternary schon drin, es kam von dort
  (Commit `bd8ff1ee28`, „Der Drehencoder drehte ins Leere" weiter oben). Diese
  Änderung hier ist das Nachziehen auf den beiden Sofles.

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
