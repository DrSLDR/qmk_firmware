/* Copyright 2017 Benjamin Kesselring
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


// place overrides here
#define ONESHOT_TIMEOUT 3000
#undef DEBOUNCE
#define DEBOUNCE 5

// Define some LIGHTS and EFFECTS

#define RESET_LED_HSV HSV_RED
#define _WM_LED_HSV 5, 255, 255
#define _QW_LED_HSV 100, 255, 255
#define _OSM_LED_HSV HSV_GREEN

#define STARTUP_EFFECT RGBLIGHT_MODE_STATIC_LIGHT
#define FN_EFFECT RGBLIGHT_MODE_SNAKE + 5
#define CAPS_EFFECT RGBLIGHT_MODE_KNIGHT + 1

// Combo stuff
#define COMBO_COUNT 3
#define COMBO_TERM 100

// Skyrim macro stuff
#define SKY_INTERACT_KEY KC_R
#define SKY_READY_KEY KC_T
#define SKY_KEY_DELAY 25

// Enable NKRO by great force
#define FORCE_NKRO

/* USB Device descriptor parameter */
#undef VENDOR_ID
#undef PRODUCT_ID
#undef DEVICE_VER
#undef MANUFACTURER
#undef PRODUCT
#undef DESCRIPTION
#define VENDOR_ID       0x51dc
#define PRODUCT_ID      0x005e
#define DEVICE_VER      0x0001
#define MANUFACTURER    SLDR
#define PRODUCT         ID75 (idobo)
#define DESCRIPTION     15x5 matrix keyboard
