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
#define _QW 0
#define _WM 1
#define _FN 2
#define _OSM 3

// Modifier macros
#define _MOD_LCTL 0x1
#define _MOD_LSFT 0x2
#define _MOD_LALT 0x4
#define _MOD_LGUI 0x8
#define _MOD_RCTL 0x10
#define _MOD_RSFT 0x20
#define _MOD_RALT 0x40
#define _MOD_RGUI 0x80

// Variable declarations
static uint8_t prev_mode;
static bool caps;
static uint8_t topmost_active_layer;

// Caps effect control
void caps_effect_toggle(void);

// Compose effect control
void compose_cycle(void);

// Color triple decomposer
void decompose_triple(uint8_t, uint8_t, uint8_t, uint8_t*, uint8_t*, uint8_t*);

// Layer manager function
void move_layer(bool up);

// Custom keycodes
enum custom_keycodes {
    LY_UP = SAFE_RANGE
  , LY_DN
  , RESET
  , SKY_S1     // Skyrim, sell 1
  , SKY_MK     // Skyrim, make thing
  , SKY_MK2    // Skyrim, make thing but differently
  , HLP_AR1    // Helper arrow ->
  , HLP_AR2    // Helper arrow =>
};

// Combo things
enum combos {
  AO_ARING,       // Å, å
  AE_ADIAE,       // Ä, ä
  EO_ODIAE,       // Ö, ö
  EX_EACUT        // É, é
};

const uint16_t PROGMEM ao_combo[]  = {KC_A, KC_O, COMBO_END};       // Å, å
const uint16_t PROGMEM ae_combo[]  = {KC_A, KC_E, COMBO_END};       // Ä, ä
const uint16_t PROGMEM eo_combo[]  = {KC_E, KC_O, COMBO_END};       // Ö, ö
const uint16_t PROGMEM ex_combo[]  = {KC_E, KC_X, COMBO_END};       // É, é

combo_t key_combos[COMBO_COUNT] = {
  [AO_ARING] = COMBO_ACTION(ao_combo),      // Å, å
  [AE_ADIAE] = COMBO_ACTION(ae_combo),      // Ä, ä
  [EO_ODIAE] = COMBO_ACTION(eo_combo),      // Ö, ö
  [EX_EACUT] = COMBO_ACTION(ex_combo)       // É, é
};

// Helper macros
#define ALGUI A(KC_LGUI)
#define KCLCA C(KC_LALT)
#define ESCFN LT(_FN, KC_ESC)
#define KCOSL OSL(_OSM)
#define CLSFT C(KC_LSFT)
#define KCMEH LCA(KC_LSFT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

 /* QWERTY (Keeping QWERTY on the bottom, layering WM on top)
* .--------------------------------------------------------------------------------------------------------------------------------------.
 * | GRAVE  | 1      | 2      | 3      | 4      | 5      | INSERT | PSCR   | BREAK  | 6      | 7      | 8      | 9      | 0      | BACKSP |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | W      | E      | R      | T      | PREV   | PLAY   | NEXT   | Y      | U      | I      | O      | P      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | LCTRL  | A      | S      | D      | F      | G      | VOL-   | MUTE   | VOL+   | H      | J      | K      | L      | ;      | '      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | C      | V      | B      | ALGUI  | NOOP   | LCA    | N      | M      | ,      | .      | /      | RSHFT  |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | ESC/FN | LGUI   | OSL    | LALT   | FN     | SPACE  | CLSFT  | CAPS   | RALT   | ENTER  | FN     | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_QW] = LAYOUT_ortho_5x15( /* QWERTY */
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_INS,  KC_PSCR, KC_PAUS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MPRV, KC_MPLY, KC_MNXT, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, \
    KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_VOLD, KC_MUTE, KC_VOLU, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    ALGUI,   XXXXXXX, KCLCA,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    ESCFN,   KC_LGUI, KCOSL,   KC_LALT, MO(_FN), KC_SPC,  CLSFT,   KC_CAPS, KC_RALT, KC_ENT,  MO(_FN), KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
 ),

/* WORKMAN (Partially transparent layer to handle Workman remappings; () means unchanged key, [] means to be changed key)
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        | (Q)    | [D]    | [R]    | [W]    | [B]    |        |        |        | [J]    | [F]    | [U]    | [P]    | [;]    |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * |        | (A)    | (S)    | [H]    | [T]    | (G)    |        |        |        | [Y]    | [N]    | [E]    | [O]    | [I]    |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |        | (Z)    | (X)    | [M]    | [C]    | [V]    |        |        |        | [K]    | [L]    |        |        |        |        |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_WM] = LAYOUT_ortho_5x15( /* Workman overlayer */
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, KC_D,    KC_R,    KC_W,    KC_B,    _______, _______, _______, KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, _______, \
    _______, _______, _______, KC_H,    KC_T,    _______, _______, _______, _______, KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    _______, \
    _______, _______, _______, KC_M,    KC_C,    KC_V,    _______, _______, _______, KC_K,    KC_L,    _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
 ),

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F11    | BOOT   | F12    | F6     | F7     | F8     | F9     | F10    | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | SKY S1 | SKY MK | SKY MK2|        |        |        | STOP   |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        | [      | ]      | MINS   | EQL    |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        | RGB TOG|        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        | [TRNS] |        | L DOWN | BUILD  | L UP   |        | [TRNS] | HOME   | PG UP  | PG DN  | END    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
    _______, SKY_S1,  SKY_MK,  SKY_MK2, _______, _______, _______, KC_MSTP, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  _______, \
    _______, _______, _______, _______, _______, _______, _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, KC_TRNS, _______, LY_DN,   QK_MAKE, LY_UP,   _______, KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END   \
 ),

/* SUPER ONE-SHOT STUFF
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | CAPS   |        |        |        |        |        |        |        |        |        |        |        |        |        | CAPS   |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        | NOOP   |        |        |        |        |        | NOOP   |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_OSM] = LAYOUT_ortho_5x15( /* ONE SHOT LAYER */
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_CAPS, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
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
        reset_keyboard();
      }
      else {
        // do fuck all
      }
      break;
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

    // MACROS AND OTHER HELPFUL STUFF
    case SKY_S1:
      if(record->event.pressed){
        tap_code(KC_ENT);
        wait_ms(SKY_KEY_DELAY);
        tap_code(KC_HOME);
        wait_ms(SKY_KEY_DELAY);
        tap_code(KC_ENT);
      }
      break;
    case SKY_MK:
      if(record->event.pressed){
        tap_code(SKY_INTERACT_KEY);
        wait_ms(SKY_KEY_DELAY);
        tap_code(KC_Y);
      }
      break;
    case SKY_MK2:
      if(record->event.pressed){
        tap_code(SKY_READY_KEY);
        wait_ms(SKY_KEY_DELAY);
        tap_code(KC_Y);
      }
      break;
    case HLP_AR1:
      if(record->event.pressed){
        SEND_STRING("->");
      }
      break;
    case HLP_AR2:
      if(record->event.pressed){
        SEND_STRING("=>");
      }
      break;
  }
  return true;
}

// COMBO HANDLER //////////////////////////////////////////////////////////////

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case AO_ARING:
      if (pressed) {
        tap_code16(RALT(KC_W));         // Å, å
      }
      break;
    case AE_ADIAE:
      if (pressed) {
        tap_code16(RALT(KC_A));         // Ä, ä
      }
      break;
    case EO_ODIAE:
      if (pressed) {
        tap_code16(RALT(KC_O));         // Ö, ö
      }
      break;
    case EX_EACUT:
      if (pressed) {
        tap_code16(RALT(KC_G));         // É, é
      }
      break;
  }
}

// INIT FUNCTION //////////////////////////////////////////////////////////////

void keyboard_post_init_user(){
  caps = false;
  topmost_active_layer = _WM;

  layer_on(_QW);
  layer_on(_WM);

  rgblight_sethsv(_WM_LED_HSV);
  rgblight_mode_noeeprom(STARTUP_EFFECT);
}

// LAYER CONTROL FUNCTION /////////////////////////////////////////////////////

void move_layer(bool up) {
  switch (topmost_active_layer) {
    case _WM:
      if (! up) {
        layer_off(topmost_active_layer);
        topmost_active_layer = _QW;
        rgblight_sethsv(_QW_LED_HSV);
      }
      break;
    case _QW:
      if (up) {
        topmost_active_layer = _WM;
        layer_on(topmost_active_layer);
        rgblight_sethsv(_WM_LED_HSV);
      break;
    }
  return;
  }
}

// COLOR DECOMPOSE FUNCTION ////////////////////////////////////////////////////

void decompose_triple(uint8_t h, uint8_t s, uint8_t v,
                      uint8_t *hp, uint8_t *sp, uint8_t *vp){
  *hp = h;
  *sp = s;
  *vp = v;
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
    rgblight_sethsv(_WM_LED_HSV);
  }
}
