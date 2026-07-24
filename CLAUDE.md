# Wechselbalgs QMK-Fork — Projektkontext

Persönlicher Fork von QMK zur Pflege eigener Tastaturlayouts. Diese Datei ist der
zentrale, mit dem Repo mitreisende Statusspeicher, damit die Arbeit an jedem Rechner
fortgesetzt werden kann. **Nach nennenswerten Änderungen den Abschnitt „Aktueller
Stand" aktualisieren.**

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
| Sofle Choc | `sofle_choc` | AVR (voll, ~96 %) |
| splitkb Kyria | `splitkb/kyria/rev1` | AVR (voll, ~92 %) |
| GMMK Pro ISO | `gmmk/pro/rev1/iso` | STM32 |
| Lotus58 | `tweetydabird/lotus58` | AVR (voll, ~95 %) |
| Keychron K3 Pro ISO RGB | `keychron/k3_pro/iso/rgb` | STM32 |

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

## Aktueller Stand (Stand: 2026-07-24)

**Fertig:**
- Fork aufgeräumt (nur noch master/develop/mike), auf aktuellen QMK-Stand gemergt.
- Alle 6 Boards kompilieren.
- Layer Lock + Caps Word auf QMK-Core umgestellt; altes `features/layer_lock.c`
  und Duplikat `users/wechselbalg_` entfernt.
- `_MAC`-Overlay-Layer für alle Wrapper-Keymaps hinzugefügt.
- Lotus58 vom eigenen Port auf das offizielle `tweetydabird/lotus58` migriert.

**Offen / vor dem Flashen prüfen:**
- **K3 Pro**: MINE-Ebene enthält Annahmen (neu belegte `+`-Taste, NUBS = `MO__NUM`);
  `_TEST`-Layer ist von keiner Taste erreichbar. Vor Nutzung sichten.
- **Lotus58-Keymap** ist noch eigenständig (nicht auf das Wrapper-System umgestellt)
  → hat daher noch **keinen** `_MAC`-Layer. Nächster natürlicher Schritt.
- K3-Pro-Varianten ansi/jis/white bleiben ungebaut (alte LED-Tabellen-Makros).
- Idee: Mac-Variante des `_NAV`-Layers (Wort-Sprünge Opt+Pfeil statt Ctrl+Pfeil).
