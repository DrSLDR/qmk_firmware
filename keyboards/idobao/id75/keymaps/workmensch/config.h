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
#define _WM_LED_HSV 20, 255, 255
#define _SE_LED_HSV HSV_BLUE
#define _OSM_LED_HSV HSV_GREEN

#define COMP_LNX_LED_HSV HSV_RED
#define COMP_WINC_LED_HSV 139, 232, 255

#define _FN_LED_SAT_DEN 2

#define STARTUP_EFFECT RGBLIGHT_MODE_STATIC_LIGHT
#define FN_EFFECT RGBLIGHT_MODE_SNAKE + 5
#define CAPS_EFFECT RGBLIGHT_MODE_BREATHING + 3

// Skyrim macro stuff
#define SKY_INTERACT_KEY KC_R
#define SKY_READY_KEY KC_T
#define SKY_KEY_DELAY 25

// Compose flash delay
#define COMPOSE_FLASH_DELAY 100

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
