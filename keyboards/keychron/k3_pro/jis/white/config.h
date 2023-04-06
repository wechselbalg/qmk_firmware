/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

#ifdef LED_MATRIX_ENABLE
/* LED matrix driver configuration */
#    define DRIVER_COUNT 1
#    define DRIVER_ADDR_1 0b1110100
#    define LED_MATRIX_LED_COUNT 87

/* Set to infinit, which is use in USB mode by default */
#    define LED_MATRIX_TIMEOUT LED_MATRIX_TIMEOUT_INFINITE
/* Allow shutdown of led driver to save power */
#    define LED_MATRIX_DRIVER_SHUTDOWN_ENABLE
/* Turn off backlight on low brightness to save power */
#    define LED_MATRIX_BRIGHTNESS_TURN_OFF_VAL 48

/* Caps lock indicating led */
#    define DIM_CAPS_LOCK
#    define CAPS_LOCK_INDEX 46
#    define LOW_BAT_IND_INDEX 79

/* LED Matrix Animation modes. Explicitly enabled
 * For full list of effects, see:
 * https://docs.qmk.fm/#/feature_led_matrix?id=led-matrix-effects
 */
#    define LED_MATRIX_KEYPRESSES

#    define ENABLE_LED_MATRIX_NONE
#    define ENABLE_LED_MATRIX_SOLID
// #    define ENABLE_LED_MATRIX_ALPHAS_MODS
#    define ENABLE_LED_MATRIX_BREATHING
// #    define ENABLE_LED_MATRIX_BAND
#    define ENABLE_LED_MATRIX_BAND_PINWHEEL
#    define ENABLE_LED_MATRIX_BAND_SPIRAL
#    define ENABLE_LED_MATRIX_CYCLE_LEFT_RIGHT
#    define ENABLE_LED_MATRIX_CYCLE_UP_DOWN
#    define ENABLE_LED_MATRIX_CYCLE_OUT_IN
#    define ENABLE_LED_MATRIX_DUAL_BEACON

#    define ENABLE_LED_MATRIX_SOLID_REACTIVE_SIMPLE
// #    define ENABLE_LED_MATRIX_SOLID_REACTIVE_WIDE
#    define ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTIWIDE
// #    define ENABLE_LED_MATRIX_SOLID_REACTIVE_CROSS
// #    define ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTICROSS
// #    define ENABLE_LED_MATRIX_SOLID_REACTIVE_NEXUS
#    define ENABLE_LED_MATRIX_SOLID_REACTIVE_MULTINEXUS
#    define ENABLE_LED_MATRIX_SOLID_SPLASH
// #    define ENABLE_LED_MATRIX_SOLID_MULTISPLASH

#    define ENABLE_LED_MATRIX_WAVE_LEFT_RIGHT
#    define ENABLE_LED_MATRIX_WAVE_UP_DOWN
#    define ENABLE_LED_MATRIX_EFFECT_MAX

/* Use first 6 channels of LED driver */
#    define PHASE_CHANNEL MSKPHASE_6CHANNEL

/* Set LED driver current */
#    define CKLED2001_CURRENT_TUNE \
        { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }

#endif
