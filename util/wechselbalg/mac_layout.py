#!/usr/bin/env python3
"""Was liefert eine Tastenkombination auf der macOS-Belegung -- und wo liegt
ein bestimmtes Zeichen?

Hintergrund: die AltGr-Ebene der deutschen Windows-Belegung liegt auf dem Mac
anders (@ ist AltGr+Q gegen Opt+L). users/wechselbalg/wechselbalg.c uebersetzt
das im Mac-Modus ueber eine Tabelle. Wer die Tabelle erweitert, sollte die
Mac-Seite *messen* statt sie zu raten -- dafuer ist dieses Skript da.

Es liest die echte, aktive Systembelegung ueber Carbons UCKeyTranslate aus,
also genau das, was der Mac beim Tippen tatsaechlich tut.

    python3 util/wechselbalg/mac_layout.py            # Zeichen -> Kombination
    python3 util/wechselbalg/mac_layout.py Opt+L      # Kombination -> Zeichen
    python3 util/wechselbalg/mac_layout.py --dump     # ganze Belegung

Nur macOS. Keine Abhaengigkeiten ausser der Standardbibliothek.

⚠️ Zwei Fallstricke, beide hier schon eingebaut:
  - Die TIS-Properties muessen die *exportierten* Konstanten sein
    (ctypes.c_void_p.in_dll). Selbstgebaute CFStrings fuehren zum Absturz.
  - UniCharCount ist UInt32, nicht unsigned long. Mit c_ulong korrumpiert
    UCKeyTranslate den Stack.
"""
import ctypes
import ctypes.util
import sys
from ctypes import byref, c_uint8, c_uint16, c_uint32, c_void_p, POINTER

carbon = ctypes.cdll.LoadLibrary(ctypes.util.find_library("Carbon"))
cf = ctypes.cdll.LoadLibrary(ctypes.util.find_library("CoreFoundation"))

cf.CFStringGetCStringPtr.restype = ctypes.c_char_p
cf.CFStringGetCStringPtr.argtypes = [c_void_p, c_uint32]
cf.CFDataGetBytePtr.restype = POINTER(c_uint8)
cf.CFDataGetBytePtr.argtypes = [c_void_p]
carbon.TISCopyCurrentKeyboardLayoutInputSource.restype = c_void_p
carbon.TISGetInputSourceProperty.restype = c_void_p
carbon.TISGetInputSourceProperty.argtypes = [c_void_p, c_void_p]
carbon.UCKeyTranslate.restype = ctypes.c_int32
carbon.UCKeyTranslate.argtypes = [
    POINTER(c_uint8), c_uint16, c_uint16, c_uint32, c_uint32, c_uint32,
    POINTER(c_uint32), c_uint32, POINTER(c_uint32), POINTER(c_uint16),
]
carbon.LMGetKbdType.restype = c_uint8

_prop_name = c_void_p.in_dll(carbon, "kTISPropertyLocalizedName")
_prop_data = c_void_p.in_dll(carbon, "kTISPropertyUnicodeKeyLayoutData")
cf.CFStringGetCString.restype = ctypes.c_bool
cf.CFStringGetCString.argtypes = [c_void_p, ctypes.c_char_p, ctypes.c_long, c_uint32]


def _cfstr(ref):
    """CFStringRef -> str. CFStringGetCStringPtr darf NULL liefern, dann kopieren."""
    if not ref:
        return "?"
    p = cf.CFStringGetCStringPtr(ref, 0x08000100)
    if p:
        return p.decode()
    buf = ctypes.create_string_buffer(512)
    if cf.CFStringGetCString(ref, buf, 512, 0x08000100):
        return buf.value.decode()
    return "?"


_src = carbon.TISCopyCurrentKeyboardLayoutInputSource()
LAYOUT_NAME = _cfstr(carbon.TISGetInputSourceProperty(_src, _prop_name))
KBD = cf.CFDataGetBytePtr(carbon.TISGetInputSourceProperty(_src, _prop_data))
KBTYPE = carbon.LMGetKbdType()

# macOS-Virtual-Keycode -> deutsche Tastenbeschriftung.
# QMK sendet HID-Usages; die Zuordnung HID -> Virtual Keycode ist positionell.
KEYS = {
    18: "1", 19: "2", 20: "3", 21: "4", 23: "5", 22: "6", 26: "7", 28: "8",
    25: "9", 29: "0", 27: "ß", 24: "´",
    12: "Q", 13: "W", 14: "E", 15: "R", 17: "T", 16: "Z", 32: "U", 34: "I",
    31: "O", 35: "P", 33: "Ü", 30: "+",
    0: "A", 1: "S", 2: "D", 3: "F", 5: "G", 4: "H", 38: "J", 40: "K",
    37: "L", 41: "Ö", 39: "Ä", 42: "#",
    6: "Y", 7: "X", 8: "C", 9: "V", 11: "B", 45: "N", 46: "M",
    43: ",", 47: ".", 44: "-",
    50: "<", 10: "^", 49: "Space",
}
MODS = [(0, ""), (2, "S"), (8, "Opt"), (10, "S+Opt")]
BY_NAME = {v.upper(): k for k, v in KEYS.items()}


def xlate(vk, mods, dead_in=0):
    """(Zeichenkette, deadKeyState). Leere Kette + dead != 0 heisst Dead Key."""
    dead = c_uint32(dead_in)
    length = c_uint32(0)
    buf = (c_uint16 * 8)()
    if carbon.UCKeyTranslate(KBD, vk, 0, mods, KBTYPE, 0, byref(dead), 8,
                             byref(length), buf) != 0:
        return "", 0
    return "".join(chr(buf[i]) for i in range(length.value)), dead.value


def resolve(vk, mods):
    """(Zeichen, ist_dead_key) -- Dead Keys werden mit Space aufgeloest."""
    s, dead = xlate(vk, mods)
    if s:
        return s, False
    if dead:
        s2, _ = xlate(49, 0, dead)  # kVK_Space
        return s2, True
    return "", False


def build_table():
    """Zeichen -> Liste von (Kombination, ist_dead_key)."""
    table = {}
    for vk, keyname in KEYS.items():
        for mval, mname in MODS:
            s, is_dead = resolve(vk, mval)
            combo = f"{mname}+{keyname}" if mname else keyname
            for ch in s:
                table.setdefault(ch, []).append((combo, is_dead))
    return table


def main():
    print(f"Aktive Belegung: {LAYOUT_NAME}\n")
    args = sys.argv[1:]

    if args and args[0] == "--dump":
        for vk, keyname in KEYS.items():
            row = []
            for mval, mname in MODS:
                s, dead = resolve(vk, mval)
                row.append(f"{mname or 'blank'}={s or '-'}{'*' if dead else ''}")
            print(f"{keyname:<7} " + "  ".join(row))
        print("\n* = Dead Key (hier mit Space aufgeloest)")
        return

    if args:  # Kombination -> Zeichen, z.B. "Opt+L" oder "S+Opt+7"
        for spec in args:
            parts = spec.split("+")
            key, mods = parts[-1].upper(), "+".join(parts[:-1])
            vk = BY_NAME.get(key)
            if vk is None:
                print(f"{spec}: unbekannte Taste '{key}'")
                continue
            mval = dict((m, v) for v, m in MODS).get(mods)
            if mval is None:
                print(f"{spec}: unbekannte Modifier '{mods}' (leer, S, Opt, S+Opt)")
                continue
            s, dead = resolve(vk, mval)
            print(f"{spec:<12} -> {s or '(nichts)'}{'  (Dead Key)' if dead else ''}")
        return

    # Vorgabe: die Zeichen, die dieses Layout benutzt, rueckwaerts nachschlagen
    table = build_table()
    wanted = "@[]{}\\|~¹²³‹›¢‘’„“”«»…·–—¡¿£¤¥ſ€°§"
    print(f"{'Zeichen':<9} macOS-Kombination(en)")
    print("-" * 52)
    for ch in wanted:
        hits = table.get(ch)
        if not hits:
            print(f"{ch:<9} -- NICHT ERREICHBAR --")
        else:
            shown = ", ".join(c + ("  (Dead Key)" if d else "") for c, d in hits[:3])
            print(f"{ch:<9} {shown}")


if __name__ == "__main__":
    main()
