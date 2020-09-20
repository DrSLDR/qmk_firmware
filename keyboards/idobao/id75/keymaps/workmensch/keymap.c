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
#define _QW 1
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
static uint8_t active_base_layer;
static uint8_t active_compose;

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
  , COMP_SW    // Switch compose mode
  , SKY_S1     // Skyrim, sell 1
  , SKY_MK     // Skyrim, make thing
  , SKY_MK2    // Skyrim, make thing but differently
  , HLP_AR1    // Helper arrow ->
  , HLP_AR2    // Helper arrow =>
};

// Unicode keycodes and map
enum unicode_names {
  SLAR,
  CLAR,
  SLAD,
  CLAD,
  SLOD,
  CLOD,
  SLEA,
  CLEA,
  SLJC,
  CLJC,
  SLSC,
  CLSC,
  SLHC,
  CLHC,
  SLCC,
  CLCC,
  SLGC,
  CLGC,
  SLUB,
  CLUB,
  STHO,
  CTHO
};

const uint32_t PROGMEM unicode_map[] = {
  [SLAR] = 0xe5,  // SMALL LETTER A WITH RING ABOVE    (å)
  [CLAR] = 0xc5,  // CAPITAL LETTER A WITH RING ABOVE  (Å)
  [SLAD] = 0xe4,  // SMALL LETTER A WITH DIAERESIS     (ä)
  [CLAD] = 0xc4,  // CAPITAL LETTER A WITH DIAERESIS   (Ä)
  [SLOD] = 0xf6,  // SMALL LETTER O WITH DIAERESIS     (ö)
  [CLOD] = 0xd6,  // CAPITAL LETTER O WITH DIAERESIS   (Ö)
  [SLEA] = 0xe9,  // SMALL LETTER E WITH ACUTE         (é)
  [CLEA] = 0xc9,  // CAPITAL LETTER E WITH ACUTE       (É)
  [SLJC] = 0x135, // SMALL LETTER J WITH CIRCUMFLEX    (ĵ)
  [CLJC] = 0x134, // CAPITAL LETTER J WITH CIRCUMFLEX  (Ĵ)
  [SLSC] = 0x15d, // SMALL LETTER S WITH CIRCUMFLEX    (ŝ)
  [CLSC] = 0x15c, // CAPITAL LETTER S WITH CIRCUMFLEX  (Ŝ)
  [SLHC] = 0x125, // SMALL LETTER H WITH CIRCUMFLEX    (ĥ)
  [CLHC] = 0x124, // CAPITAL LETTER H WITH CIRCUMFLEX  (Ĥ)
  [SLCC] = 0x109, // SMALL LETTER C WITH CIRCUMFLEX    (ĉ)
  [CLCC] = 0x108, // CAPITAL LETTER C WITH CIRCUMFLEX  (Ĉ)
  [SLGC] = 0x11d, // SMALL LETTER G WITH CIRCUMFLEX    (ĝ)
  [CLGC] = 0x11c, // CAPITAL LETTER G WITH CIRCUMFLEX  (Ĝ)
  [SLUB] = 0x16d, // SMALL LETTER U WITH BREVE         (ŭ)
  [CLUB] = 0x16c, // CAPITAL LETTER U WITH BREVE       (Ŭ)
  [STHO] = 0xfe,  // SMALL LETTER THORN                (þ)
  [CTHO] = 0xde   // CAPITAL LETTER THORN              (Þ)
};

// Combo things
enum combos {
  AO_ARING,
  ZE_ADIAE,
  EO_ODIAE
};

const uint16_t PROGMEM ao_combo[] = {KC_A, KC_O, COMBO_END};
const uint16_t PROGMEM ze_combo[] = {KC_Z, KC_E, COMBO_END};
const uint16_t PROGMEM eo_combo[] = {KC_E, KC_O, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  [AO_ARING] = COMBO_ACTION(ao_combo),
  [ZE_ADIAE] = COMBO_ACTION(ze_combo),
  [EO_ODIAE] = COMBO_ACTION(eo_combo)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* WORKMAN
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | GRAVE  | 1      | 2      | 3      | 4      | 5      | INSERT | PSCR   | BREAK  | 6      | 7      | 8      | 9      | 0      | BACKSP |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | Q      | D      | R      | W      | B      | PREV   | PLAY   | NEXT   | J      | F      | U      | P      | ;      | \      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | LCTRL  | A      | S      | H      | T      | G      | VOL-   | MUTE   | VOL+   | Y      | N      | E      | O      | I      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | Z      | X      | M      | C      | V      | ALGUI  | NOOP   | LCA    | K      | L      | ,      | .      | /      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | ESC/FN | LGUI   | FN     | LALT   | OSL    | SPACE  | CLSFT  | CAPS   | MEH    | SPACE  | OSL    | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_WM] = LAYOUT_ortho_5x15( /* Workmensch */
    KC_GRV,          KC_1,    KC_2,    KC_3,    KC_4,      KC_5,   KC_INS,     KC_PSCR, KC_PAUS,      KC_6,   KC_7,      KC_8,    KC_9,   KC_0,    KC_BSPC, \
    KC_TAB,          KC_Q,    KC_D,    KC_R,    KC_W,      KC_B,   KC_MPRV,    KC_MPLY, KC_MNXT,      KC_J,   KC_F,      KC_U,    KC_P,   KC_SCLN, KC_BSLS, \
    KC_LCTL,         KC_A,    KC_S,    KC_H,    KC_T,      KC_G,   KC_VOLD,    KC_MUTE, KC_VOLU,      KC_Y,   KC_N,      KC_E,    KC_O,   KC_I,    KC_ENT,  \
    KC_LSFT,         KC_Z,    KC_X,    KC_M,    KC_C,      KC_V,   A(KC_LGUI), XXXXXXX, C(KC_LALT),   KC_K,   KC_L,      KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, \
    LT(_FN, KC_ESC), KC_LGUI, MO(_FN), KC_LALT, OSL(_OSM), KC_SPC, C(KC_LSFT), KC_CAPS, LCA(KC_LSFT), KC_SPC, OSL(_OSM), KC_LEFT, KC_UP,  KC_DOWN, KC_RGHT  \
 ),

 /* QWERTY (Because that's needed sometimes)
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        | Q      | W      | E      | R      | T      |        |        |        | Y      | U      | I      | O      | P      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * |        | A      | S      | D      | F      | G      |        |        |        | H      | J      | K      | L      | ;      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |        | Z      | X      | C      | V      | B      |        |        |        | N      | M      |        |        |        |        |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_QW] = LAYOUT_ortho_5x15( /* QWERTY */
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    _______, _______, _______, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______, \
    _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    _______, _______, _______, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______, \
    _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______, _______, _______, KC_N,    KC_M,    _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
 ),

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F11    | RESET  | F12    | F6     | F7     | F8     | F9     | F10    | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | SKY S1 | SKY MK | SKY MK2|        |        |        | STOP   |        |        | {      | }      | EQL    | PLUS   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        | [      | ]      | QUOT   | DQUO   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        | RGB TOG|        |        |        |        | MINS   | UNDS   |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        | [TRNS] |        | LY DOWN|        |        | COMP SW|        |        | LY UP  | HOME   | PG UP  | PG DN  | END    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
    _______, SKY_S1,  SKY_MK,  SKY_MK2, _______, _______, _______, KC_MSTP, _______, _______, KC_LCBR, KC_RCBR, KC_EQL,  KC_PLUS, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, KC_QUOT, KC_DQUO, _______, \
    _______, _______, _______, _______, _______, _______, _______, RGB_TOG, _______, _______, _______, _______, KC_MINS, KC_UNDS, _______, \
    _______, _______, KC_TRNS, _______, LY_DN,   _______, _______, COMP_SW, _______, _______, LY_UP,   KC_HOME, KC_PGUP, KC_PGDN, KC_END \
 ),

/* SUPER ONE-SHOT STUFF
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        |        |        | Ĵ      |        | Ŭ      |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | Å      | Ŝ      | Ĥ      | Þ      | Ĝ      |        |        |        |        | Å      | Ä      | Ö      | É      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | CAPS   |        |        |        | Ĉ      |        |        |        |        |        |        |        | ->     | =>     | CAPS   |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        | NOOP   |        |        |        |        |        | NOOP   |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_OSM] = LAYOUT_ortho_5x15( /* ONE SHOT LAYER */
    _______, _______,        _______,        _______,        _______,        _______,        _______, _______, _______, _______,        _______,        _______,        _______,        _______,        _______, \
    _______, _______,        _______,        _______,        _______,        _______,        _______, _______, _______, XP(SLJC, CLJC), _______,        XP(SLUB, CLUB), _______,        _______,        _______, \
    _______, XP(SLAR, CLAR), XP(SLSC, CLSC), XP(SLHC, CLHC), XP(STHO, CTHO), XP(SLGC, CLGC), _______, _______, _______, _______,        XP(SLAR, CLAR), XP(SLAD, CLAD), XP(SLOD, CLOD), XP(SLEA, CLEA), _______, \
    KC_CAPS, _______,        _______,        _______,        XP(SLCC, CLCC), _______,        _______, _______, _______, _______,        _______,        _______,        HLP_AR1,        HLP_AR2,        KC_CAPS, \
    _______, _______,        _______,        _______,        XXXXXXX,        _______,        _______, _______, _______, _______,        XXXXXXX,        _______,        _______,        _______,        _______  \
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
    case COMP_SW:
      if (record->event.pressed){
        compose_cycle();
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
        tap_code16(RALT(KC_A));
      }
      break;
    case ZE_ADIAE:
      if (pressed) {
        tap_code16(RALT(KC_QUOT));
      }
      break;
    case EO_ODIAE:
      if (pressed) {
        tap_code16(RALT(KC_O));
      }
      break;
  }
}

// INIT FUNCTION //////////////////////////////////////////////////////////////

void keyboard_post_init_user(){
  caps = false;
  active_base_layer = _WM;
  active_compose = UC_LNX;

  rgblight_sethsv(_WM_LED_HSV);
  rgblight_mode_noeeprom(STARTUP_EFFECT);
  set_unicode_input_mode(active_compose);
}

// LAYER CONTROL FUNCTION /////////////////////////////////////////////////////

void move_layer(bool up) {
  switch (active_base_layer) {
    case _WM:
      if (up) {
        active_base_layer = _QW;
        layer_on(active_base_layer);
        rgblight_sethsv(_QW_LED_HSV);
      }
      break;
    case _QW:
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
  return;
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

// COMPOSE METHOD CYCLE ////////////////////////////////////////////////////////

void compose_cycle(){
  switch (active_compose){
    case UC_LNX:
      active_compose = UC_WINC;
      SEND_STRING("WIN");
      break;
    case UC_WINC:
      active_compose = UC_LNX;
      SEND_STRING("LNX");
      break;
  }
  set_unicode_input_mode(active_compose);
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
