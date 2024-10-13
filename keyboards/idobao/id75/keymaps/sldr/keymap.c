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
  AO_ARING,       // Å, å
  AE_ADIAE,       // Ä, ä
  EO_ODIAE,       // Ö, ö
  EU_EACUT,       // É, é
  PAR_SECT,       // °, §
  DEG_DEGR,       // °, § - shortcut to the degree sign
  LAM_LAMB,       // Λ, λ
  DEL_DELT,       // Δ, δ
  LEM_INFT,       // ™, ∞ - lemniscate
  TM_TRADM,       // ™, ∞ - shortcut to the trademark sign
  EUR_EURO,       // €
  THO_THRN,       // Þ, þ
  LAR_LARR,       // ←
  RAW_RARR        // →
};

const uint16_t PROGMEM ao_combo[]  = {KC_A, KC_O, COMBO_END};       // Å, å
const uint16_t PROGMEM ae_combo[]  = {KC_A, KC_E, COMBO_END};       // Ä, ä
const uint16_t PROGMEM eo_combo[]  = {KC_E, KC_O, COMBO_END};       // Ö, ö
const uint16_t PROGMEM eu_combo[]  = {KC_E, KC_U, COMBO_END};       // É, é
const uint16_t PROGMEM par_combo[] = {KC_P, KC_A, KC_R, COMBO_END}; // °, §
const uint16_t PROGMEM deg_combo[] = {KC_D, KC_E, KC_G, COMBO_END}; // °, §
const uint16_t PROGMEM lam_combo[] = {KC_L, KC_A, KC_M, COMBO_END}; // Λ, λ
const uint16_t PROGMEM del_combo[] = {KC_D, KC_E, KC_L, COMBO_END}; // Δ, δ
const uint16_t PROGMEM lem_combo[] = {KC_L, KC_E, KC_M, COMBO_END}; // ™, ∞
const uint16_t PROGMEM tm_combo[]  = {KC_T, KC_M, COMBO_END};       // ™, ∞
const uint16_t PROGMEM eur_combo[] = {KC_E, KC_U, KC_R, COMBO_END}; // €
const uint16_t PROGMEM tho_combo[] = {KC_T, KC_H, KC_O, COMBO_END}; // Þ, þ
const uint16_t PROGMEM lar_combo[] = {KC_L, KC_A, KC_R, COMBO_END}; // ←
const uint16_t PROGMEM raw_combo[] = {KC_R, KC_A, KC_W, COMBO_END}; // →

combo_t key_combos[COMBO_COUNT] = {
  [AO_ARING] = COMBO_ACTION(ao_combo),      // Å, å
  [AE_ADIAE] = COMBO_ACTION(ae_combo),      // Ä, ä
  [EO_ODIAE] = COMBO_ACTION(eo_combo),      // Ö, ö
  [EU_EACUT] = COMBO_ACTION(eu_combo),      // É, é
  [PAR_SECT] = COMBO_ACTION(par_combo),     // °, §
  [DEG_DEGR] = COMBO_ACTION(deg_combo),     // °, §
  [LAM_LAMB] = COMBO_ACTION(lam_combo),     // Λ, λ
  [DEL_DELT] = COMBO_ACTION(del_combo),     // Δ, δ
  [LEM_INFT] = COMBO_ACTION(lem_combo),     // ™, ∞
  [TM_TRADM] = COMBO_ACTION(tm_combo),      // ™, ∞
  [EUR_EURO] = COMBO_ACTION(eur_combo),     // €
  [THO_THRN] = COMBO_ACTION(tho_combo),     // Þ, þ
  [LAR_LARR] = COMBO_ACTION(lar_combo),     // ←
  [RAW_RARR] = COMBO_ACTION(raw_combo)      // →
};

// Helper macros
#define ALGUI A(KC_LGUI)
#define LCA C(KC_LALT)
#define ESCFN LT(_FN, KC_ESC)
#define KCOSL OSL(_OSM)
#define CLSFT C(KC_LSFT)
#define MEH LCA(KC_LSFT)

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
 * | ESC/FN | LGUI   | FN     | LALT   | OSL    | SPACE  | CLSFT  | CAPS   | MEH    | ENTER  | RALT   | LEFT   | UP     | DOWN   | RIGHT  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

  [_QW] = LAYOUT_ortho_5x15( /* QWERTY */
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_INS,  KC_PSCR, KC_PAUS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MPRV, KC_MPLY, KC_MNXT, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, \
    KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_VOLD, KC_MUTE, KC_VOLU, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
    LC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    ALGUI,   XXXXXXX, LCA,     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    ESCFN,   KC_LGUI, MO(_FN), KC_LALT, KCOSL,   KC_SPC,  CLSFT,   KC_CAPS, MEH,     KC_ENT,  KC_RALT, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT  \
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
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
 ),

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F11    | RESET  | F12    | F6     | F7     | F8     | F9     | F10    | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | SKY S1 | SKY MK | SKY MK2|        |        |        | STOP   |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        | COMB T |        |        | [      | ]      | MINS   | EQL    |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        |        |        |        |        |        | RGB TOG|        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        |        | [TRNS] |        | LY DOWN|        |        | COMP SW|        |        | LY UP  | HOME   | PG UP  | PG DN  | END    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = LAYOUT_ortho_5x15( /* FUNCTION */
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,  RESET,   KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
    _______, SKY_S1,  SKY_MK,  SKY_MK2, _______, _______, _______, KC_MSTP, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, CMB_TOG, _______, _______, KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  _______, \
    _______, _______, _______, _______, _______, _______, _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, KC_TRNS, _______, LY_DN,   _______, _______, COMP_SW, _______, _______, LY_UP,   KC_HOME, KC_PGUP, KC_PGDN, KC_END \
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
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
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
        tap_code16(RALT(KC_A));         // Å, å
      }
      break;
    case AE_ADIAE:
      if (pressed) {
        tap_code16(RALT(KC_QUOT));      // Ä, ä
      }
      break;
    case EO_ODIAE:
      if (pressed) {
        tap_code16(RALT(KC_O));         // Ö, ö
      }
      break;
    case EU_EACUT:
      if (pressed) {
        tap_code16(RALT(KC_Y));         // É, é
      }
      break;
    case PAR_SECT:
      if (pressed) {
        tap_code16(RALT(KC_2));         // °, §
      }
      break;
    case DEG_DEGR:
      if (pressed) {
        tap_code16(SAGR(KC_2));         // °, §
      }
      break;
    case LAM_LAMB:
      if (pressed) {
        tap_code16(RALT(KC_4));         // Λ, λ
      }
      break;
    case DEL_DELT:
      if (pressed) {
        tap_code16(RALT(KC_5));         // Δ, δ
      }
      break;
    case LEM_INFT:
      if (pressed) {
        tap_code16(RALT(KC_1));         // ™, ∞
      }
      break;
    case TM_TRADM:
      if (pressed) {
        tap_code16(SAGR(KC_1));         // ™, ∞
      }
      break;
    case EUR_EURO:
      if (pressed) {
        tap_code16(RALT(KC_E));         // €
      }
      break;
    case THO_THRN:
      if (pressed) {
        tap_code16(RALT(KC_T));         // Þ, þ
      }
      break;
    case LAR_LARR:
      if (pressed) {
        tap_code16(RALT(KC_0));         // ←
      }
      break;
    case RAW_RARR:
      if (pressed) {
        tap_code16(RALT(KC_MINS));      // →
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
