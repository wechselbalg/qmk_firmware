#include "wechselbalg.h"

// Deutsche Caps-Word-Regeln (Core-Feature CAPS_WORD_ENABLE), gemeinsam fuer alle Boards
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case DE___AE:
        case DE___OE:
        case DE___UE:
        case DE_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case N3_NUM1 ... N3_NUM0:
        case DE_SS:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}
