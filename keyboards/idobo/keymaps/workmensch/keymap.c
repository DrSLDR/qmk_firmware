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
#define _SE 1
#define _FN 2

// Shorthand LED macros
#define LEDTRIPLE uint8_t, uint8_t, uint8_t
#define PLEDTRIPLE uint8_t*, uint8_t*, uint8_t*

#define PCURLED &ch, &cs, &cv
#define PPRELED &ph, &ps, &pv

// Variable declarations
static uint8_t ph, ps, pv, ch, cs, cv;
static bool caps;
static uint8_t active_base_layer;

// Low level LED control
void read_current_led(PLEDTRIPLE);
void set_led(PLEDTRIPLE);
bool led_equal(LEDTRIPLE, PLEDTRIPLE);

// Higher level LED utility functions
void set_temporary_led(LEDTRIPLE);
void reset_temporary_led(void);
void set_permanent_led(LEDTRIPLE);
void caps_effect_toggle(void);
void set_temporary_fn_led(void);

// Layer manager function
void move_layer(bool up);

// Custom keycodes
enum custom_keycodes {
  LY_UP = SAFE_RANGE,
  LY_DN,
};

// Combo keycodes
enum combos {
  AA_SWE,
  AE_SWE,
  OE_SWE,
  EE_SWE
};

// Combo definitions
const uint16_t PROGMEM aa_combo[] = {KC_A, KC_O, COMBO_END};
const uint16_t PROGMEM ae_combo[] = {KC_A, KC_E, COMBO_END};
const uint16_t PROGMEM oe_combo[] = {KC_O, KC_E, COMBO_END};
const uint16_t PROGMEM ee_combo[] = {KC_E, KC_SPC, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  [AA_SWE] = COMBO(aa_combo, KC_LBRC),
  [AE_SWE] = COMBO(ae_combo, KC_QUOT),
  [OE_SWE] = COMBO(oe_combo, KC_SCLN),
  [EE_SWE] = COMBO(ee_combo, KC_EQL)
};

// A tap dance state struct
typedef struct {
  int state;
} tap;

// A tap dance type enum
enum {
  SINGLE_TAP = 1,
  DOUBLE_TAP = 2
};

// General tap dance prototypes
int cur_dance (qk_tap_dance_state_t *state);

// Specific handler prototypes for the Shift tap dance
void lshft_finished(qk_tap_dance_state_t *state, void *user_data);
void lshft_reset(qk_tap_dance_state_t *state, void *user_data);
void rshft_finished(qk_tap_dance_state_t *state, void *user_data);
void rshft_reset(qk_tap_dance_state_t *state, void *user_data);

// Specific shift tap dance state objects
static tap lshfttap_state = {
  .state = 0
};
static tap rshfttap_state = {
  .state = 0
};

// Tap dance definitions
enum {
  TD_LSH_CAPS = 0,
  TD_RSH_CAPS = 1
};

qk_tap_dance_action_t tap_dance_actions[] = {
  // Tap once for shift, twice for Caps Lock
  [TD_LSH_CAPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lshft_finished, lshft_reset),
  [TD_RSH_CAPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rshft_finished, rshft_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* WORKMAN
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | GRAVE  | 1      | 2      | 3      | 4      | 5      |        |        |        | 6      | 7      | 8      | 9      | 0      | BACKSP |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | W      | E      | R      | T      |        |        |        | Y      | U      | I      | O      | P      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | LCTRL  | A      | S      | D      | F      | G      |        |        |        | H      | J      | K      | L      | ;      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | C      | V      | B      |        |        |        | N      | M      | ,      | .      | /      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | ESC    | LGUI   | FN     | LALT   | SPACE  | SPACE  |        |        | CMB TOG| SPACE  | SPACE  | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_WM] = LAYOUT_ortho_5x15( /* Workmensch */
    KC_GRV,           KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______, _______, _______, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
    KC_TAB,           KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    _______, _______, _______, KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSLS, \
    KC_LCTL,          KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    _______, _______, _______, KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    KC_ENT,  \
    TD(TD_LSH_CAPS),  KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    _______, _______, _______, KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, TD(TD_RSH_CAPS), \
    KC_ESC,           KC_LGUI, MO(_FN), KC_LALT, KC_SPC,  KC_SPC,  _______, _______, CMB_TOG, KC_SPC,  KC_SPC,  KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
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
 * |        |        | [TRNS] |        | LY DOWN|        | RGB TG | P0     | PAUSE  | PENT   | LY UP  | HOME   | PG UP  | PG DN  | END    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
    _______, _______, _______, _______, _______, _______, KC_P7,   KC_P8,   KC_P9,   KC_MINS, KC_LCBR, KC_RCBR, KC_EQL,  KC_PLUS, _______, \
    _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, _______, KC_P4,   KC_P5,   KC_P6,   KC_PLUS, KC_LBRC, KC_RBRC, KC_QUOT, KC_DQUO, _______, \
    _______, KC_VOLD, KC_MUTE, KC_VOLU, _______, _______, KC_P1,   KC_P2,   KC_P3,   KC_PENT, _______, _______, KC_MINS, KC_UNDS, _______, \
    _______, _______, KC_TRNS, _______, LY_DN,   _______, RGB_TOG, KC_P0,   KC_PAUS, KC_PENT, LY_UP,   KC_HOME, KC_PGUP, KC_PGDN, KC_END \
 ),

 /* SWEDEN WORKMAN MONSTROSITY
  * .--------------------------------------------------------------------------------------------------------------------------------------.
  * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
  * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
  * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
  * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
  * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
  * '--------------------------------------------------------------------------------------------------------------------------------------'
  */

   [_SE] = LAYOUT_ortho_5x15( /* SWEWORKNOPE */
     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
  ),
};

// STANDARD FUNCTIONS /////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record){
  switch (keycode) {
    case RESET:
      if (record->event.pressed){
        rgblight_sethsv(RESET_LED_HSV);
      }
      else {
        // do fuck all
      }
      break;
    case MO(_FN):
      if (record->event.pressed){
        set_temporary_fn_led();
      }
      else {
        reset_temporary_led();
      }
      break;
    case LY_UP:
      if (record->event.pressed){
        move_layer(true);
        return false;
      }
      break;
    case LY_DN:
      if (record->event.pressed){
        move_layer(false);
        return false;
      }
      break;
  }
  return true;
}

void keyboard_post_init_user(){
  ph = 0; ps = 0; pv = 0;
  ch = 0; cs = 0; cv = 0;
  caps = false;
  active_base_layer = _WM;

  set_permanent_led(_WM_LED_HSV);
  rgblight_mode_noeeprom(STARTUP_EFFECT);
  combo_disable();
}

// LAYER CONTROL FUNCTION /////////////////////////////////////////////////////

void move_layer(bool up) {
  switch (active_base_layer) {
    case _WM:
      if (up) {
        active_base_layer = _SE;
        layer_on(active_base_layer);
        set_permanent_led(_SE_LED_HSV);
      }
      break;
    case _SE:
      if (up) {
        // There is no higher layer
        return;
      }
      else {
        layer_off(active_base_layer);
        active_base_layer = _WM;
        set_permanent_led(_WM_LED_HSV);
      }
      break;
  }
}

// LED CONTROL FUNCTIONS //////////////////////////////////////////////////////

void set_temporary_led(uint8_t h, uint8_t s, uint8_t v){
  if (led_equal(h, s, v, PCURLED)) {
    // Suggested color is the same as the recorded current, go no further
    return;
  }
  read_current_led(PPRELED);
  ch = h; cs = s; cv = v;
  set_led(PCURLED);
}

void reset_temporary_led(){
  if (0 == rgblight_get_mode()){
    // Leds are turned off. Do nothing
    return;
  }
  set_led(PPRELED);
  ch = ph; cs = ps; cv = pv;
}

void set_permanent_led(uint8_t h, uint8_t s, uint8_t v){
  ph = h; ps = s; pv = v;
  set_led(PPRELED);
}

void read_current_led(uint8_t *hp, uint8_t *sp, uint8_t *vp){
  *hp = rgblight_get_hue();
  *sp = rgblight_get_sat();
  *vp = rgblight_get_val();
}

void set_led(uint8_t *hp, uint8_t *sp, uint8_t *vp){
  rgblight_sethsv(*hp, *sp, *vp);
}

bool led_equal(uint8_t h,   uint8_t s,   uint8_t v,
                      uint8_t *hp, uint8_t *sp, uint8_t *vp){
  return (h == *hp && s == *sp && v == *vp);
}

void caps_effect_toggle(){
  if (caps) {
    // Caps is on, turn off.
    caps = false;
    rgblight_mode(STARTUP_EFFECT);
  }
  else {
    // Caps is off, turn on.
    caps = true;
    rgblight_mode(CAPS_EFFECT);
  }
}

void set_temporary_fn_led(){
  uint8_t sat = ps / _FN_LED_SAT_DEN;
  set_temporary_led(ph, sat, pv);
}

// TAP DANCE CONTROL FUNCTIONS ////////////////////////////////////////////////

int cur_dance(qk_tap_dance_state_t *state){
  if (state->count == 1) {
    return SINGLE_TAP;
  }
  else if (state->count == 2) {
    return DOUBLE_TAP;
  }
  else {
    return 7; // The most powerful number
  }
}

void lshft_finished(qk_tap_dance_state_t *state, void *user_data) {
  lshfttap_state.state = cur_dance(state);
  switch (lshfttap_state.state) {
    case SINGLE_TAP: register_code(KC_LSFT); break;
    case DOUBLE_TAP: register_code(KC_CAPS); break;
  }
}

void lshft_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (lshfttap_state.state) {
    case SINGLE_TAP: unregister_code(KC_LSFT); break;
    case DOUBLE_TAP: unregister_code(KC_CAPS); caps_effect_toggle(); break;
  }
  lshfttap_state.state = 0;
}

void rshft_finished(qk_tap_dance_state_t *state, void *user_data) {
  rshfttap_state.state = cur_dance(state);
  switch (rshfttap_state.state) {
    case SINGLE_TAP: register_code(KC_RSFT); break;
    case DOUBLE_TAP: register_code(KC_CAPS); break;
  }
}

void rshft_reset(qk_tap_dance_state_t *state, void *user_data) {
  switch (rshfttap_state.state) {
    case SINGLE_TAP: unregister_code(KC_RSFT); break;
    case DOUBLE_TAP: unregister_code(KC_CAPS); caps_effect_toggle(); break;
  }
  rshfttap_state.state = 0;
}
