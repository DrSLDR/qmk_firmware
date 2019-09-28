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
#define TAPPING_TERM 150
#define COMBO_TERM 150
#define COMBO_COUNT 4

// Define some LIGHTS and EFFECTS

#define RESET_LED_HSV HSV_RED
#define _WM_LED_HSV HSV_ORANGE
#define _SE_LED_HSV HSV_BLUE

#define _FN_LED_SAT_DEN 2

#define STARTUP_EFFECT RGBLIGHT_MODE_STATIC_LIGHT
#define CAPS_EFFECT RGBLIGHT_MODE_SNAKE + 5

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
