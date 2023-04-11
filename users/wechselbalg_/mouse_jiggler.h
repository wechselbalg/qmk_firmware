/*
 * Copyright 2020 Richard Sutherland (rich@brickbots.com)
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

#include <stdbool.h>
#include <stdint.h>

#ifndef MOUSE_JIGGLER_SECONDS_MIN
#    define MOUSE_JIGGLER_SECONDS_MIN 240
#endif
#ifndef MOUSE_JIGGLER_SECONDS_MAX
#    define MOUSE_JIGGLER_SECONDS_MAX 299
#endif
#ifndef MOUSE_JIGGLER_KEYCODE
#    define MOUSE_JIGGLER_KEYCODE KC_F13
#endif

bool mouse_jiggler_keycode(uint16_t keycode);
bool mouse_jiggler_keycode_kb(uint16_t keycode);
bool mouse_jiggler_keycode_user(uint16_t keycode);

#ifdef MOUSE_JIGGLER_ALLOW_COUNT_REGRESSION
uint8_t mouse_jiggler_regress_count(uint16_t keycode);
#endif

void    update_mouse_jiggler(uint16_t);

void decay_mouse_jiggler(void);
