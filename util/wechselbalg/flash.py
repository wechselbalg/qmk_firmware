#!/usr/bin/env python3
"""Flash helper for wechselbalg's QMK split keyboards.

Compiles and flashes each half in turn, using `qmk flash`'s built-in
bootloader-wait logic (so it works whether the device shows up as a UF2
drive, a Caterina serial port, or a DFU device). Runs on macOS, Linux and
Windows -- it only shells out to `qmk`/`make`, no OS-specific tooling.

There is no way to trigger the bootloader itself from this script (no
generic QMK mechanism for that) -- you still have to press the physical
boot button (or the QK_BOOT key combo) on the half being flashed when
prompted. Everything after that (waiting for the device, copying/flashing,
moving on to the next half) is automatic.

Usage:
    python3 util/wechselbalg/flash.py --list
    python3 util/wechselbalg/flash.py sofle_choc_black
    python3 util/wechselbalg/flash.py sofle_choc_white --side right
"""
import argparse
import os
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
AVRDUDE_CONF_WORKAROUND = Path(__file__).resolve().parent / "avrdude_minimal_atmega32u4.conf"

# To add a new keyboard: add an entry here. "sides" is None for boards that
# are a single PCB (no split halves); otherwise a dict with "left"/"right"
# entries. "bootloader" is the -bl value passed to `qmk flash` (None = use
# the board's default bootloader target).
BOARDS = {
    "sofle_choc_white": {
        "label": "Sofle Choc (weiss, Atmel/AVR: rechts Elite-C, links Pro Micro)",
        "kb": "sofle_choc",
        "km": "wechselbalg",
        "convert_to": None,
        "sides": {
            "left": {
                "bootloader": None,  # default target = Caterina/avr109
                "prompt": (
                    "Boot-Taster der LINKEN Haelfte (Pro Micro) GEDRUECKT HALTEN,\n"
                    "  dabei USB einstecken. Bootloader bleibt nur ~8 Sekunden aktiv!"
                ),
                "avrdude_conf_workaround": True,
            },
            "right": {
                "bootloader": "dfu",
                "prompt": (
                    "Boot-Taster der RECHTEN Haelfte (Elite-C) GEDRUECKT HALTEN,\n"
                    "  dabei USB einstecken."
                ),
                "avrdude_conf_workaround": False,
            },
        },
    },
    "sofle_choc_black": {
        "label": "Sofle Choc (schwarz, 2x splitkb Liatris/RP2040)",
        "kb": "sofle_choc",
        "km": "wechselbalg",
        "convert_to": "liatris",
        "sides": {
            "left": {
                "bootloader": "uf2-split-left",
                "prompt": "BOOTSEL an der LINKEN Haelfte gedrueckt halten, dabei USB einstecken.",
            },
            "right": {
                "bootloader": "uf2-split-right",
                "prompt": "BOOTSEL an der RECHTEN Haelfte gedrueckt halten, dabei USB einstecken.",
            },
        },
    },
    "k3_pro": {
        "label": "Keychron K3 Pro ISO RGB (kein Split, ein Board)",
        "kb": "keychron/k3_pro/iso/rgb",
        "km": "wechselbalg",
        "convert_to": None,
        "sides": None,  # kein Split -> nur ein Flash-Durchlauf
        "prompt": (
            "USB abziehen. Dann Esc GEDRUECKT HALTEN und dabei wieder einstecken.\n"
            "  Das ist Bootmagic: BOOTMAGIC_ENABLE steht in keyboards/keychron/k3_pro/rules.mk,\n"
            "  und Matrix [0,0] ist auf diesem Board die Esc-Taste. Das Geraet meldet sich\n"
            "  danach als 'STM32 BOOTLOADER' (DFU, 0483:df11).\n"
            "  ACHTUNG: Bootmagic setzt dabei das EEPROM zurueck -- das ist gewollt und\n"
            "  zugleich der Rettungsweg, falls das Board je auf einem toten Layer landet.\n"
            "  Alternativen: QK_BOOT liegt im _ADJUST-Layer (End halten) auf Esc, Backspace\n"
            "  und B -- oder der Reset-Taster auf der Platine (Gehaeuse oeffnen)."
        ),
    },
    # Weitere Boards hier ergaenzen, z.B.:
    # "kyria": {
    #     "label": "splitkb Kyria",
    #     "kb": "splitkb/kyria/rev1",
    #     "km": "wechselbalg",
    #     "convert_to": None,
    #     "sides": {
    #         "left":  {"bootloader": None, "prompt": "..."},
    #         "right": {"bootloader": None, "prompt": "..."},
    #     },
    # },
    # "gmmk_pro": {
    #     "label": "Glorious GMMK Pro (kein Split, ein Board)",
    #     "kb": "gmmk/pro/rev1/iso",
    #     "km": "wechselbalg",
    #     "convert_to": None,
    #     "sides": None,  # kein Split -> nur ein Flash-Durchlauf
    # },
}


def run_flash(kb, km, convert_to, bootloader, extra_env):
    cmd = ["qmk", "flash", "-kb", kb, "-km", km]
    if convert_to:
        cmd += ["-e", f"CONVERT_TO={convert_to}"]
    if bootloader:
        cmd += ["-bl", bootloader]
    env = os.environ.copy()
    env.update(extra_env)
    print(f"\n$ {' '.join(cmd)}\n")
    subprocess.run(cmd, cwd=REPO_ROOT, env=env, check=True)


def flash_side(board, side_key, side):
    print(f"\n=== {board['label']} — Seite: {side_key} ===")
    print(side["prompt"])
    input("Enter druecken, sobald bereit (der Befehl wartet automatisch auf das Geraet)...")

    extra_env = {}
    if side.get("avrdude_conf_workaround") and AVRDUDE_CONF_WORKAROUND.exists():
        extra_env["AVRDUDE_PROGRAMMER"] = f'avrdude -C "{AVRDUDE_CONF_WORKAROUND}"'

    run_flash(board["kb"], board["km"], board.get("convert_to"), side.get("bootloader"), extra_env)
    print(f"--- {side_key} fertig ---")


def flash_board(board_key, only_side=None):
    board = BOARDS[board_key]
    sides = board.get("sides")

    if sides is None:
        flash_side(board, "single", {
            "bootloader": None,
            "prompt": board.get("prompt", "Board in den Bootloader-Modus bringen, dann USB einstecken."),
        })
        return

    if only_side:
        flash_side(board, only_side, sides[only_side])
        return

    for side_key, side in sides.items():
        flash_side(board, side_key, side)
    print(f"\nFertig: {board['label']} komplett geflasht.")


def list_boards():
    print("Verfuegbare Boards:")
    for key, b in BOARDS.items():
        print(f"  {key:20s} {b['label']}")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("board", nargs="?", choices=list(BOARDS.keys()), help="Welches Board geflasht werden soll")
    parser.add_argument("--side", choices=["left", "right"], help="Nur eine Haelfte flashen")
    parser.add_argument("--list", action="store_true", help="Verfuegbare Boards auflisten und beenden")
    args = parser.parse_args()

    if args.list:
        list_boards()
        sys.exit(0)

    if not args.board:
        list_boards()
        parser.print_usage()
        sys.exit(1)

    board = BOARDS[args.board]
    if args.side and not board.get("sides"):
        print(f"{args.board} ist nicht gesplittet, --side wird ignoriert.")
        flash_board(args.board)
    else:
        flash_board(args.board, only_side=args.side)


if __name__ == "__main__":
    main()
