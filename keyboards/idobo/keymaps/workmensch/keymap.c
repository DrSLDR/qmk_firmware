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
#define _SE_FN 3
#define _OSM 4

// Variable declarations
static uint8_t prev_mode;
static bool caps;
static uint8_t active_base_layer;

// Caps effect control
void caps_effect_toggle(void);

// Layer manager function
void move_layer(bool up);

// Custom keycodes
enum custom_keycodes {
  LY_UP = SAFE_RANGE,
  LY_DN,
  SWE_COM,
  SWE_PER,
  SWE_SLS,
  SWE_BSL,
  SWE_SCL,
  SWE_TLD,
  SWE_2,
  SWE_4,
  SWE_6,
  SWE_7,
  SWE_8,
  SWE_9,
  SWE_0,
  SWE_LCB,
  SWE_RCB,
  SWE_LBC,
  SWE_RBC,
  SWE_EQL,
  SWE_PLS,
  SWE_QOT,
  SWE_DQT,
  SWE_MIN,
  SWE_USC,
  SWE_AA,
  SWE_AAS,
  SWE_AE,
  SWE_AES,
  SWE_OE,
  SWE_OES,
  SWE_ACU,
  SWE_EE,
  SWE_EES
};

// Keycode tapper - shorthand when doing SWE replacement
void reg_unreg_keycode(uint16_t keycode, bool pressed);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* WORKMAN
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | GRAVE  | 1      | 2      | 3      | 4      | 5      |        | OSL    |        | 6      | 7      | 8      | 9      | 0      | BACKSP |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | W      | E      | R      | T      | P7     | P8     | P9     | Y      | U      | I      | O      | P      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | LCTRL  | A      | S      | D      | F      | G      | P4     | P5     | P6     | H      | J      | K      | L      | ;      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | C      | V      | B      | P1     | P2     | P3     | N      | M      | ,      | .      | /      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | ESC    | LGUI   | FN     | LALT   | OSL    | SPACE  | NUMLCK | P0     | CMB TOG| SPACE  | OSL    | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_WM] = LAYOUT_ortho_5x15( /* Workmensch */
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,       KC_5,    _______, OSL(_OSM), _______, KC_6,    KC_7,       KC_8,    KC_9,    KC_0,    KC_BSPC, \
    KC_TAB,  KC_Q,    KC_D,    KC_R,    KC_W,       KC_B,    KC_P7,   KC_P8,     KC_P9,   KC_J,    KC_F,       KC_U,    KC_P,    KC_SCLN, KC_BSLS, \
    KC_LCTL, KC_A,    KC_S,    KC_H,    KC_T,       KC_G,    KC_P4,   KC_P5,     KC_P6,   KC_Y,    KC_N,       KC_E,    KC_O,    KC_I,    KC_ENT,  \
    KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,       KC_V,    KC_P1,   KC_P2,     KC_P3,   KC_K,    KC_L,       KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_ESC,  KC_LGUI, MO(_FN), KC_LALT, OSL(_OSM),  KC_SPC,  KC_NLCK, KC_P0,     CMB_TOG, KC_SPC,  OSL(_OSM),  KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
 ),

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F11    | RESET  | F12    | F6     | F7     | F8     | F9     | F10    | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        | 7      | 8      | 9      | -      | {      | }      | EQL    | PLUS   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | PREV   | PLAY   | NEXT   | STOP   |        | 4      | 5      | 6      | +      | [      | ]      | QUOT   | DQUO   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | VOL-   | MUTE   | VOL+   |        |        | 1      | 2      | 3      | PENT   |        |        | MINS   | UNDS   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        | [TRNS] |        | LY DOWN|        | RGB TG | 0      | PAUSE  |        | LY UP  | HOME   | PG UP  | PG DN  | END    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
    _______, _______, _______, _______, _______, _______, KC_7,    KC_8,    KC_9,    KC_MINS, KC_LCBR, KC_RCBR, KC_EQL,  KC_PLUS, _______, \
    _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, _______, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_LBRC, KC_RBRC, KC_QUOT, KC_DQUO, _______, \
    _______, KC_VOLD, KC_MUTE, KC_VOLU, _______, _______, KC_1,    KC_2,    KC_3,    KC_PENT, _______, _______, KC_MINS, KC_UNDS, _______, \
    _______, _______, KC_TRNS, _______, LY_DN,   _______, RGB_TOG, KC_0,    KC_PAUS, _______, LY_UP,   KC_HOME, KC_PGUP, KC_PGDN, KC_END \
 ),

 /* SWEDEN WORKMAN MONSTROSITY
  * .--------------------------------------------------------------------------------------------------------------------------------------.
  * | SE ~   |        | SE 2   |        | SE 4   |        |        |        |        | SE 6   | SE 7   | SE 8   | SE 9   | SE 0   |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
  * |        |        |        |        |        |        |        |        |        |        |        |        |        | SE ;   | SE \   |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
  * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
  * |        |        |        |        |        |        |        |        |        |        |        | SE ,   | SE .   | SE /   |        |
  * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
  * |        |        | FN     |        |        |        |        |        |        |        |        |        |        |        |        |
  * '--------------------------------------------------------------------------------------------------------------------------------------'
  */

   [_SE] = LAYOUT_ortho_5x15( /* SWEWORKNOPE */
     SWE_TLD, _______, SWE_2,      _______, SWE_4,   _______, _______, _______, _______, SWE_6,   SWE_7,   SWE_8,   SWE_9,   SWE_0,   _______, \
     _______, _______, _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, SWE_SCL, SWE_BSL, \
     _______, _______, _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
     _______, _______, _______,    _______, _______, _______, _______, _______, _______, _______, _______, SWE_COM, SWE_PER, SWE_SLS, _______, \
     _______, _______, MO(_SE_FN), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
 ),

  /* SPECIFIC SWEDISH FUNCTION OVERRIDES
   * .--------------------------------------------------------------------------------------------------------------------------------------.
   * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
   * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
   * |        |        |        |        |        |        |        |        |        |        | SE {   | SE }   | SE =   | SE +   |        |
   * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
   * |        |        |        |        |        |        |        |        |        |        | SE [   | SE ]   | SE '   | SE "   |        |
   * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
   * |        |        |        |        |        |        |        |        |        |        |        |        | SE -   | SE _   |        |
   * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
   * |        |        | [TRNS] |        |        |        |        |        |        |        |        |        |        |        |        |
   * '--------------------------------------------------------------------------------------------------------------------------------------'
   */

   [_SE_FN] = LAYOUT_ortho_5x15( /* SWEDISH FUNCTION */
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, SWE_LCB, SWE_RCB, SWE_EQL, SWE_PLS, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, SWE_LBC, SWE_RBC, SWE_QOT, SWE_DQT, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, SWE_MIN, SWE_USC, _______, \
      _______, _______, KC_TRNS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
 ),

   /* SUPER ONE-SHOT STUFF
    * .--------------------------------------------------------------------------------------------------------------------------------------.
    * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
    * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
    * |        | Å      |        |        |        |        |        |        |        | É      | Å      | Ä      | Ö      |        |        |
    * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
    * |        | å      |        |        |        |        |        |        |        | é      | å      | ä      | ö      |´       |        |
    * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
    * | CAPS   |        |        |        |        |        |        |        |        |        |        |        |        |        | CAPS   |
    * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
    * |        |        |        |        | NOOP   |        |        |        |        |        | NOOP   |        |        |        |        |
    * '--------------------------------------------------------------------------------------------------------------------------------------'
    */

    [_OSM] = LAYOUT_ortho_5x15( /* ONE SHOT LAYER */
       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
       _______, SWE_AAS, _______, _______, _______, _______, _______, _______, _______, SWE_EES, SWE_AAS, SWE_AES, SWE_OES, _______, _______, \
       _______, SWE_AA,  _______, _______, _______, _______, _______, _______, _______, SWE_EE,  SWE_AA,  SWE_AE,  SWE_OE,  SWE_ACU,  _______, \
       KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_CAPS, \
       _______, _______, _______, _______, XXXXXXX, _______, _______, _______, _______, _______, XXXXXXX, _______, _______, _______, _______  \
 ),
};

// STANDARD FUNCTIONS /////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record){
  switch (keycode) {
    case RESET:
      if (record->event.pressed){
        rgblight_mode(STARTUP_EFFECT);
        rgblight_sethsv(RESET_LED_HSV);
        wait_ms(50);
      }
      else {
        // do fuck all
      }
      break;
    case MO(_SE_FN):
      if (record->event.pressed){
        layer_on(_FN);
      }
      else {
        layer_off(_FN);
      }
    case MO(_FN):
      if (record->event.pressed){
        rgblight_mode(FN_EFFECT);
      }
      else {
        rgblight_mode(prev_mode);
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
    case KC_CAPS:
      if (record->event.pressed){
        caps_effect_toggle();
      }
      break;

    // Special edition Swedish keycodes
    case SWE_COM:
      if (keyboard_report->mods &(0x2|0x20)) {
        uint8_t temp = keyboard_report->mods & (2|32);
        unregister_mods(temp);
        reg_unreg_keycode(KC_NUBS, record->event.pressed);
        register_mods(temp);
      }
      else {
        reg_unreg_keycode(KC_COMM, record->event.pressed);
      }
      return false;
    case SWE_PER:
      if (keyboard_report->mods &(0x2|0x20)) {
        reg_unreg_keycode(KC_NUBS, record->event.pressed);
      }
      else {
        reg_unreg_keycode(KC_DOT, record->event.pressed);
      }
      return false;
      case SWE_SLS:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_MINS, record->event.pressed);
        }
        else {
          register_mods(0x2);
          reg_unreg_keycode(KC_7, record->event.pressed);
          unregister_mods(0x2);
        }
        return false;
      case SWE_BSL:
        if (keyboard_report->mods &(0x2|0x20)) {
          uint8_t temp = keyboard_report->mods & (2|32);
          unregister_mods(temp);
          register_mods(0x40); // ALTGR
          reg_unreg_keycode(KC_NUBS, record->event.pressed);
          unregister_mods(0x40);
          register_mods(temp);
        }
        else {
          register_mods(0x40); // ALTGR
          reg_unreg_keycode(KC_MINS, record->event.pressed);
          unregister_mods(0x40);
        }
        return false;
      case SWE_SCL:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_DOT, record->event.pressed);
        }
        else {
          register_mods(0x2); // LSFT
          reg_unreg_keycode(KC_COMM, record->event.pressed);
          unregister_mods(0x2);
        }
        return false;
      case SWE_TLD:
        if (keyboard_report->mods &(0x2|0x20)) {
          uint8_t temp = keyboard_report->mods & (2|32);
          unregister_mods(temp);
          register_mods(0x40); // ALTGR
          reg_unreg_keycode(KC_RBRC, record->event.pressed);
          unregister_mods(0x40);
          register_mods(temp);
        }
        else {
          register_mods(0x2); // LSFT
          reg_unreg_keycode(KC_EQL, record->event.pressed);
          unregister_mods(0x2);
        }
        return false;
      case SWE_2:
        if (keyboard_report->mods &(0x2|0x20)) {
          uint8_t temp = keyboard_report->mods & (2|32);
          unregister_mods(temp);
          register_mods(0x40); // ALTGR
          reg_unreg_keycode(KC_2, record->event.pressed);
          unregister_mods(0x40);
          register_mods(temp);
        }
        else {
          reg_unreg_keycode(KC_2, record->event.pressed);
        }
        return false;
      case SWE_4:
        if (keyboard_report->mods &(0x2|0x20)) {
          uint8_t temp = keyboard_report->mods & (2|32);
          unregister_mods(temp);
          register_mods(0x40); // ALTGR
          reg_unreg_keycode(KC_4, record->event.pressed);
          unregister_mods(0x40);
          register_mods(temp);
        }
        else {
          reg_unreg_keycode(KC_4, record->event.pressed);
        }
        return false;
      case SWE_6:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_RBRC, record->event.pressed);
        }
        else {
          reg_unreg_keycode(KC_6, record->event.pressed);
        }
        return false;
      case SWE_7:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_6, record->event.pressed);
        }
        else {
          reg_unreg_keycode(KC_7, record->event.pressed);
        }
        return false;
      case SWE_8:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_NUHS, record->event.pressed);
        }
        else {
          reg_unreg_keycode(KC_8, record->event.pressed);
        }
        return false;
      case SWE_9:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_8, record->event.pressed);
        }
        else {
          reg_unreg_keycode(KC_9, record->event.pressed);
        }
        return false;
      case SWE_0:
        if (keyboard_report->mods &(0x2|0x20)) {
          reg_unreg_keycode(KC_9, record->event.pressed);
        }
        else {
          reg_unreg_keycode(KC_0, record->event.pressed);
        }
        return false;
      case SWE_LBC:
        register_mods(0x40); // ALTGR
        reg_unreg_keycode(KC_8, record->event.pressed);
        unregister_mods(0x40);
        return false;
      case SWE_RBC:
        register_mods(0x40); // ALTGR
        reg_unreg_keycode(KC_9, record->event.pressed);
        unregister_mods(0x40);
        return false;
      case SWE_LCB:
        register_mods(0x40); // ALTGR
        reg_unreg_keycode(KC_7, record->event.pressed);
        unregister_mods(0x40);
        return false;
      case SWE_RCB:
        register_mods(0x40); // ALTGR
        reg_unreg_keycode(KC_0, record->event.pressed);
        unregister_mods(0x40);
        return false;
      case SWE_EQL:
        register_mods(0x2); // LSHFT
        reg_unreg_keycode(KC_0, record->event.pressed);
        unregister_mods(0x2);
        return false;
      case SWE_PLS:
        reg_unreg_keycode(KC_MINS, record->event.pressed);
        return false;
      case SWE_QOT:
        reg_unreg_keycode(KC_NUHS, record->event.pressed);
        return false;
      case SWE_DQT:
        register_mods(0x2); // LSHFT
        reg_unreg_keycode(KC_2, record->event.pressed);
        unregister_mods(0x2);
        return false;
      case SWE_MIN:
        reg_unreg_keycode(KC_SLSH, record->event.pressed);
        return false;
      case SWE_USC:
        register_mods(0x2); // LSHFT
        reg_unreg_keycode(KC_SLSH, record->event.pressed);
        unregister_mods(0x2);
        return false;
      case SWE_AA:
        reg_unreg_keycode(KC_LBRC, record->event.pressed);
        break;
      case SWE_AAS:
        register_mods(0x2); // LSHFT
        reg_unreg_keycode(KC_LBRC, record->event.pressed);
        unregister_mods(0x2);
        break;
      case SWE_AE:
        reg_unreg_keycode(KC_QUOT, record->event.pressed);
        break;
      case SWE_AES:
        register_mods(0x2); // LSHFT
        reg_unreg_keycode(KC_QUOT, record->event.pressed);
        unregister_mods(0x2);
        break;
      case SWE_OE:
        reg_unreg_keycode(KC_SCLN, record->event.pressed);
        break;
      case SWE_OES:
        register_mods(0x2); // LSHFT
        reg_unreg_keycode(KC_SCLN, record->event.pressed);
        unregister_mods(0x2);
        break;
      case SWE_ACU:
        reg_unreg_keycode(KC_EQL, record->event.pressed);
        break;
      case SWE_EE:
        SEND_STRING("é");
        break;
      case SWE_EES:
        SEND_STRING("É");
        break;
  }
  return true;
}

void keyboard_post_init_user(){
  caps = false;
  active_base_layer = _WM;

  rgblight_sethsv(_WM_LED_HSV);
  rgblight_mode_noeeprom(STARTUP_EFFECT);
}

// LAYER CONTROL FUNCTION /////////////////////////////////////////////////////

void move_layer(bool up) {
  switch (active_base_layer) {
    case _WM:
      if (up) {
        active_base_layer = _SE;
        layer_on(active_base_layer);
        rgblight_sethsv(_SE_LED_HSV);
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
        rgblight_sethsv(_WM_LED_HSV);
      }
      break;
  }
}

// KEY REPLACEMENT SHORTHAND FUNCTION /////////////////////////////////////////

void reg_unreg_keycode(uint16_t keycode, bool pressed) {
  if (pressed) {
    register_code(keycode);
  }
  else {
    unregister_code(keycode);
  }
}

// CAPS CONTROL FUNCTION //////////////////////////////////////////////////////

void caps_effect_toggle(){
  if (caps) {
    // Caps is on, turn off.
    caps = false;
    rgblight_mode(STARTUP_EFFECT);
    prev_mode = STARTUP_EFFECT;
  }
  else {
    // Caps is off, turn on.
    caps = true;
    rgblight_mode(CAPS_EFFECT);
    prev_mode = CAPS_EFFECT;
  }
}

// LED CONTROL FOR ONE SHOT LAYER //////////////////////////////////////////////

void oneshot_layer_changed_user(uint8_t layer) {
  if (layer == _OSM) {
    rgblight_sethsv(_OSM_LED_HSV);
  }
  else {
    switch (active_base_layer) {
      case _WM:
        rgblight_sethsv(_WM_LED_HSV);
        break;
      case _SE:
        rgblight_sethsv(_SE_LED_HSV);
        break;
    }
  }
}
