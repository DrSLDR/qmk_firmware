/*
Copyright 2017 Danny Nguyen <danny@keeb.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Evil split kekbord workaround */

#define SPLIT_USB_DETECT

/* Use I2C or Serial, not both */

#define USE_SERIAL
//#define USE_I2C

/* Select hand configuration */

#define MASTER_LEFT
//#define MASTER_RIGHT
//#define EE_HANDS

#define FORCE_NKRO // force NKRO on by default

/* Combo funstuff */

#define COMBO_COUNT 14
#define COMBO_TERM 50

/* USB Device descriptor parameter */
#undef VENDOR_ID
#undef PRODUCT_ID
#undef DEVICE_VER
#undef MANUFACTURER
#undef PRODUCT
#undef DESCRIPTION
#define VENDOR_ID       0x51dc
#define PRODUCT_ID      0x005e
#define DEVICE_VER      0x0002
#define MANUFACTURER    SLDR
#define PRODUCT         Iris rev2.7
#define DESCRIPTION     Keeb.io Iris Split Keyboard
