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
#define _SE 2

// Shorthand LED macros
#define LEDTRIPLE uint8_t, uint8_t, uint8_t
#define PLEDTRIPLE uint8_t*, uint8_t*, uint8_t*

#define PCURLED &ch, &cs, &cv
#define PPRELED &ph, &ps, &pv

uint8_t ph, ps, pv, ch, cs, cv;
static void read_current_led(PLEDTRIPLE);
static void set_led(PLEDTRIPLE);
static bool led_equal(LEDTRIPLE, PLEDTRIPLE);

static void set_temporary_led(LEDTRIPLE);
static void reset_temporary_led(void);
static void set_permanent_led(LEDTRIPLE);

// Tap dance definitions
enum {
  TD_LSH_CAPS = 0,
  TD_RSH_CAPS = 1
};

qk_tap_dance_action_t tap_dance_actions[] = {
  // Tap once for shift, twice for Caps Lock
  [TD_LSH_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
  [TD_RSH_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, KC_CAPS)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* WORKMAN
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | GRAVE  | 1      | 2      | 3      | 4      | 5      | -      | `      | =      | 6      | 7      | 8      | 9      | 0      | BACKSP |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | W      | E      | R      | T      |        |        |        | Y      | U      | I      | O      | P      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | LCTRL  | A      | S      | D      | F      | G      |        |        |        | H      | J      | K      | L      | ;      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | C      | V      | B      |        |        |        | N      | M      | ,      | .      | /      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | ESC    | FN     | LGUI   | LALT   | SPACE  | SPACE  |        |        |        | SPACE  | SPACE  | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_WM] = LAYOUT_ortho_5x15( /* Workmensch */
    KC_GRV,           KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_MINS, KC_GRV,  KC_EQL,  KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
    KC_TAB,           KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    _______, _______, _______, KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSLS, \
    KC_LCTL,          KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    _______, _______, _______, KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    KC_ENT,  \
    TD(TD_LSH_CAPS),  KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    _______, _______, _______, KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, TD(TD_RSH_CAPS), \
    KC_ESC,           MO(_FN), KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  _______, _______, _______, KC_SPC,  KC_SPC,  KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
 ),

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F11    | RESET  | F12    | F6     | F7     | F8     | F9     | F10    | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        | P7     | P8     | P9     | -      | {      | }      | EQL    | PLUS   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | PREV   | PLAY   | NEXT   | STOP   |        | P4     | P5     | P6     | +      | [      | ]      | QUOT   | DQUO   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | VOL-   | MUTE   | VOL+   |        |        | P1     | P2     | P3     | PENT   |        |        | MINS   | UNDS   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | [TRNS] | RGB TG |        |        |        |        | P0     | PAUSE  | PENT   | PENT   | HOME   | PG UP  | PG DN  | END    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
    _______, _______, _______, _______, _______, _______, KC_P7,   KC_P8,   KC_P9,   KC_MINS, KC_LCBR, KC_RCBR, KC_EQL,  KC_PLUS, _______, \
    _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, _______, KC_P4,   KC_P5,   KC_P6,   KC_PLUS, KC_LBRC, KC_RBRC, KC_QUOT, KC_DQUO, _______, \
    _______, KC_VOLD, KC_MUTE, KC_VOLU, _______, _______, KC_P1,   KC_P2,   KC_P3,   KC_PENT, _______, _______, KC_MINS, KC_UNDS, _______, \
    _______, KC_TRNS, RGB_TOG, _______, _______, _______, _______, KC_P0,   KC_PAUS, KC_PENT, KC_PENT, KC_HOME, KC_PGUP, KC_PGDN, KC_END \
 ),

 /* SWEDEN WORKMAN MONSTROSITY
  * .--------------------------------------------------------------------------------------------------------------------------------------.
  * | TILDE  | 1      | 2      | 3      | 4      | 5      | -      | `      | =      | 6      | 7      | 8      | 9      | 0      | BACKSP |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
  * | TAB    | Q      | W      | E      | R      | T      |        |        |        | Y      | U      | I      | O      | P      | \      |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
  * | LCTRL  | A      | S      | D      | F      | G      |        |        |        | H      | J      | K      | L      | ;      | ENTER  |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
  * | LSHIFT | Z      | X      | C      | V      | B      |        |        |        | N      | M      | ,      | .      | /      | RSHIFT |
  * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
  * | ESC    | FN     | LGUI   | LALT   | SPACE  | SPACE  |        |        |        | SPACE  | SPACE  | LEFT   | UP     | DOWN   | RIGHT  |
  * '--------------------------------------------------------------------------------------------------------------------------------------'
  */

   [_SE] = LAYOUT_ortho_5x15( /* SWEWORKNOPE */
     KC_TILDE,         KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_MINS, KC_GRV,  KC_EQL,  KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
     KC_TAB,           KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    _______, _______, _______, KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSLS, \
     KC_LCTL,          KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    _______, _______, _______, KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    KC_ENT,  \
     TD(TD_LSH_CAPS),  KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    _______, _______, _______, KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, TD(TD_RSH_CAPS), \
     KC_ESC,           MO(_FN), KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  _______, _______, _______, KC_SPC,  KC_SPC,  KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record){
  switch (keycode) {
    case RESET:
      if (record->event.pressed){
        rgblight_sethsv(HSV_RED);
      }
      else {
        // do fuck all
      }
      break;
    case MO(_FN):
      if (record->event.pressed){
        set_temporary_led(HSV_WHITE);
      }
      else {
        reset_temporary_led();
      }
      break;
  }
  return true;
}

void keyboard_post_init_user(){
  ph = 0; ps = 0; pv = 0;
  ch = 0; cs = 0; cv = 0;

  set_permanent_led(HSV_ORANGE);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

static void set_temporary_led(uint8_t h, uint8_t s, uint8_t v){
  if (led_equal(h, s, v, PCURLED)) {
    // Suggested color is the same as the recorded current, go no further
    return;
  }
  read_current_led(PPRELED);
  ch = h; cs = s; cv = v;
  set_led(PCURLED);
}

static void reset_temporary_led(){
  if (0 == rgblight_get_mode()){
    // Leds are turned off. Do nothing
    return;
  }
  set_led(PPRELED);
  ch = ph; cs = ps; cv = pv;
}

static void set_permanent_led(uint8_t h, uint8_t s, uint8_t v){
  ph = h; ps = s; pv = v;
  set_led(PPRELED);
}

static void read_current_led(uint8_t *hp, uint8_t *sp, uint8_t *vp){
  *hp = rgblight_get_hue();
  *sp = rgblight_get_sat();
  *vp = rgblight_get_val();
}

static void set_led(uint8_t *hp, uint8_t *sp, uint8_t *vp){
  rgblight_sethsv(*hp, *sp, *vp);
}

static bool led_equal(uint8_t h,   uint8_t s,   uint8_t v,
                      uint8_t *hp, uint8_t *sp, uint8_t *vp){
  return (h == *hp && s == *sp && v == *vp);
}
