/* Copyright 2018 Milton Griffin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope thyat it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Keyboard Layers
#define _WM 0
#define _FN 1

// Shorthand LED macros
#define LEDTRIPLE uint8_t, uint8_t, uint8_t
#define PLEDTRIPLE uint8_t*, uint8_t*, uint8_t*

#define PCURLED &ch, &cs, &cv
#define PPRELED &ph, &ps, &pv
#define PTMPLED &th, &ts, &tv

uint8_t ph, ps, pv, ch, cs, cv, th, ts, tv;
static void decode_symbol(LEDTRIPLE, PLEDTRIPLE);
static void read_current_led(PLEDTRIPLE);
static void set_led(PLEDTRIPLE);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | TILDE  | 1      | 2      | 3      | 4      | 5      | -      | `      | =      | 6      | 7      | 8      | 9      | 0      | BACKSP |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | W      | E      | R      | T      | [      | \      | ]      | Y      | U      | I      | O      | P      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | LCTRL  | A      | S      | D      | F      | G      | HOME   | DEL    | PG UP  | H      | J      | K      | L      | ;      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | C      | V      | B      | END    | UP     | PG DN  | N      | M      | ,      | .      | /      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | ESC    | FN     | LGUI   | LALT   | SPACE  | SPACE  | LEFT   | DOWN   | RIGHT  | SPACE  | SPACE  | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */


  [_WM] = LAYOUT_ortho_5x15( /* Workmensch */
    KC_TILDE, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_MINS, KC_GRV,  KC_EQL,  KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
    KC_TAB,   KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    KC_LBRC, KC_BSLS, KC_RBRC, KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSLS, \
    KC_LCTL,  KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    KC_HOME, KC_DEL,  KC_PGUP, KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    KC_ENT,  \
    KC_LSFT,  KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    KC_END,  KC_UP,   KC_PGDN, KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_ESC,   MO(_FN), KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  KC_LEFT, KC_DOWN, KC_RGHT, KC_SPC,  KC_SPC,  KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
 ),


/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F11    | RESET  | F12    | F6     | F7     | F8     | F9     | F10    | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        | RGB HD | RGB HI | P7     | P8     | P9     | -      | {      | }      | EQL    | ^EQL   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | PREV   | PLAY   | NEXT   | STOP   | RGB SD | RGB SI | P4     | P5     | P6     | +      | [      | ]      | QUOT   | ^QUOT  |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | VOL-   | MUTE   | VOL+   |        | RGB VD | RGB VI | P1     | P2     | P3     | PENT   |        |        | MINS   | ^MINS  |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | [TRNS] | RGB TG |        | RGB RMD| RGB MD |        | P0     | PAUSE  | PENT   | PENT   |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,         KC_F8,         KC_F9,   KC_F10,        KC_DEL,  \
    _______, _______, _______, _______, RGB_HUD, RGB_HUI, KC_P7,   KC_P8,   KC_P9,   KC_MINS, LSFT(KC_LBRC), LSFT(KC_RBRC), KC_EQL,  LSFT(KC_EQL),  _______, \
    KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, RGB_SAD, RGB_SAI, KC_P4,   KC_P5,   KC_P6,   KC_PLUS, KC_LBRC,       KC_RBRC,       KC_QUOT, LSFT(KC_QUOT), _______, \
    KC_VOLD, KC_MUTE, KC_VOLU, _______, RGB_VAD, RGB_VAI, KC_P1,   KC_P2,   KC_P3,   KC_PENT, _______,       _______,       KC_MINS, LSFT(KC_MINS), _______, \
    _______, KC_TRNS, RGB_TOG, _______, RGB_RMOD,RGB_MOD, _______, KC_P0,   KC_PAUS, KC_PENT, KC_PENT,       _______,       _______, _______,       _______  \
 ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record){
  switch (keycode) {
    case RESET:
    if (record->event.pressed){
      rgblight_sethsv(0, 255, 255);
    }
    else {
      // do fuck all
    }
	case MO(_FN):
	if (record->event.pressed){
	  th = rgblight_get_hue();
	  ts = rgblight_get_sat();
	  tv = rgblight_get_val();
      rgblight_sethsv(43, 255, 255);
    }
    else {
      rgblight_sethsv(th, ts, tv);
    }
  }
  return true;
}

void keyboard_post_init_user(){
  rgblight_sethsv(128, 255, 255);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

static void decode_symbol(uint8_t h,   uint8_t s,   uint8_t v,
                          uint8_t *hp, uint8_t *sp, uint8_t *vp){
  *hp = h; *sp = s; *vp = v;
}

static void read_current_led(uint8_t *hp, uint8_t *sp, uint8_t *vp){
  *hp = rgblight_get_hue();
  *sp = rgblight_get_sat();
  *vp = rgblight_get_val();
}

static void set_led(uint8_t *hp, uint8_t *sp, uint8_t *vp){
  rgblight_sethsv(*hp, *sp, *vp);
}
