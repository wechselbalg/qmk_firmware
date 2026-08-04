 /* Copyright 2021 Dane Evans
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
  #pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

///https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/


#ifdef CONVERT_TO_LIATRIS
    // Liatris-Build (RP2040): beide Haelften koennen Master sein, je nachdem
    // welche Seite gerade per USB verbunden ist.
    #define SPLIT_USB_DETECT

    // Haendigkeit MUSS unabhaengig vom Master-Status feststehen, sonst denken
    // ohne weitere Angabe beide Haelften "ich bin links" (QMK-Default) und die
    // rechte Haelfte wird mit der falschen (linken) Pin-Belegung gelesen ->
    // gespiegeltes Verhalten. Fix: Haendigkeit fest im EEPROM ablegen, dafuer
    // jede Haelfte einmalig mit -bl uf2-split-left / uf2-split-right flashen.
    #define EE_HANDS

    // Zusaetzlicher Draht (Elite-C-Pin B7 = GP12) ueber den vierten TRRS-Kanal
    // ermoeglicht echtes Full-Duplex-Serial statt Half-Duplex auf nur D2/GP1.
    // keyboard.json setzt "split.serial.pin" (D2) global -> SOFT_SERIAL_PIN
    // wuerde sonst mit unserem SERIAL_USART_TX_PIN kollidieren.
    #undef SOFT_SERIAL_PIN
    #define SERIAL_USART_FULL_DUPLEX
    #define SERIAL_USART_TX_PIN 1U  // D2
    #define SERIAL_USART_RX_PIN 12U // B7 (Elite-C-Pinname), zusaetzlicher Draht
    // Verkabelung ist gerade durchverbunden (nicht gekreuzt) -> Master muss
    // TX/RX intern tauschen, damit beide Seiten zueinander passen.
    #define SERIAL_USART_PIN_SWAP
    // WS2812 belegt auf RP2040 typischerweise PIO0 - Serial auf PIO1 ausweichen.
    #define SERIAL_PIO_USE_PIO1

    /*
    Ein Byte Statusflags fuer die Status-LED (Caps Word, Layer Lock, Jiggler,
    Mac-Modus). QMK synchronisiert layer_state und die RGB-Matrix-Config von
    sich aus, diese vier aber nicht -- sie entstehen alle auf dem Master.
    Siehe users/wechselbalg/status_led.c.
    */
    #define SPLIT_TRANSACTION_IDS_USER WB_SYNC_STATUS
#else
    // #define MASTER_LEFT
    #define MASTER_RIGHT
    // #define EE_HANDS

    // Serial comms between halves. Change if using I2C
    #define USE_SERIAL
    #define SOFT_SERIAL_PIN D2
#endif

/*
Die aktive Ebene wird auf BEIDEN Haelften gebraucht: die Peripherie rechnet
ihre LED-Farben selbst aus und muss dafuer wissen, welcher Layer an ist. Stand
frueher im RGBLIGHT-Block und war damit ans falsche Feature gekoppelt.
*/
#define SPLIT_LAYER_STATE_ENABLE

#define CUSTOM_FONT

#define CUSTOM_LAYER_READ //if you remove this it causes issues - needs better guarding

// Used for tapping in keymap
// (TAPPING_FORCE_HOLD entfernt: existiert seit QMK 2023-02 nicht mehr, war
//  also ein wirkungsloser Define -- und stand inhaltlich im Widerspruch zum
//  QUICK_TAP_TERM 80 aus users/wechselbalg/config.h, das es abgeloest hat.)
#define TAPPING_TOGGLE 3
#define TAP_CODE_DELAY 10

#define RGBLIGHT_SLEEP
//
#define RGBLIGHT_LAYERS

/* ws2812 RGB LED: Pin (D3) kommt aus keyboard.json des Boards */



#ifdef RGBLIGHT_ENABLE
    //#define RGBLIGHT_ANIMATIONS
	//#define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
	#define RGBLIGHT_EFFECT_RAINBOW_MOOD
	//#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
	//#define RGBLIGHT_EFFECT_SNAKE
	//#define RGBLIGHT_EFFECT_KNIGHT
	//#define RGBLIGHT_EFFECT_CHRISTMAS
	//#define RGBLIGHT_EFFECT_STATIC_GRADIENT
	//#define RGBLIGHT_EFFECT_RGB_TEST
	//#define RGBLIGHT_EFFECT_ALTERNATING
	//#define RGBLIGHT_EFFECT_TWINKLE

    #define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT
    #define RGBLIGHT_DEFAULT_HUE 43
    #define RGBLIGHT_DEFAULT_SAT 5
    #define RGBLIGHT_DEFAULT_VAL 40



    #define RGBLIGHT_LED_COUNT 58
	#define RGBLED_SPLIT { 29, 29 }

	//#define RGBLED_NUM 30
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 8
    #define RGBLIGHT_SAT_STEP 10
    #define RGBLIGHT_VAL_STEP 10
#endif

#ifdef RGB_MATRIX_ENABLE
/*
Nur der Liatris-Build (schwarzes Board). Die Farben kommen komplett aus
users/wechselbalg/rgb_language.c, das in rgb_matrix_indicators_advanced_user()
JEDE LED selbst setzt.

Deshalb sind die Animationen hier abgeschaltet (Michael, 2026-08-04): sie
waeren ohnehin unsichtbar, weil wir jeden Frame drueberschreiben, und
RM_NEXT/RM_PREV wuerden sonst zwischen Modi umschalten, die man nicht sieht.
Uebrig bleibt RGB_MATRIX_SOLID_COLOR, das QMK immer mitbaut -- dessen
Hue/Sat faerben bei uns das **Grundleuchten**, und Val dimmt das ganze Board
inklusive aller Indikatoren. Damit bleiben alle RGB-Tasten auf _ADJUST
sinnvoll, ausser den beiden Modus-Tasten.

RGB_MATRIX_KEYPRESSES und RGB_MATRIX_FRAMEBUFFER_EFFECTS sind mit den
Animationen weggefallen -- beides existiert nur fuer reaktive bzw.
Framebuffer-Effekte.

Kein RGB_MATRIX_TIMEOUT und kein RGB_MATRIX_SLEEP: die Indikatoren sollen
immer laufen. (Die frueher hier stehenden RGB_DISABLE_AFTER_TIMEOUT /
RGB_DISABLE_WHEN_USB_SUSPENDED waren tote Namen -- RGB Matrix kennt sie nicht,
sie haben nie etwas bewirkt.)
*/
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120

// Grundleuchten: warmes Cremeweiss, entspricht KMKs BASE_COLOR (255,200,120).
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#    define RGB_MATRIX_DEFAULT_HUE 25
#    define RGB_MATRIX_DEFAULT_SAT 135
#    define RGB_MATRIX_DEFAULT_VAL 40

#    define RGB_MATRIX_HUE_STEP 8
#    define RGB_MATRIX_SAT_STEP 8
#    define RGB_MATRIX_VAL_STEP 8

// Alle Animationen aus keyboard.json wieder abschalten, siehe oben.
#    undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#    undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#    undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#    undef ENABLE_RGB_MATRIX_BAND_SAT
#    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#    undef ENABLE_RGB_MATRIX_BAND_VAL
#    undef ENABLE_RGB_MATRIX_BREATHING
#    undef ENABLE_RGB_MATRIX_CYCLE_ALL
#    undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#    undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#    undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#    undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#    undef ENABLE_RGB_MATRIX_DUAL_BEACON
#    undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#    undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#    undef ENABLE_RGB_MATRIX_HUE_BREATHING
#    undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#    undef ENABLE_RGB_MATRIX_HUE_WAVE
#    undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#    undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#    undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#    undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#    undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#    undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#    undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#    undef ENABLE_RGB_MATRIX_RAINDROPS
#endif
