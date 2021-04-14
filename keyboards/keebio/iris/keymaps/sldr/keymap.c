#include QMK_KEYBOARD_H


#define _WM 0
#define _QWER 1
#define _LOWER 3
#define _RAISE 2
#define _FN 4
#define _OSL 5

// Keycodes for grid fitting
#define CRGHT C(KC_RGHT)
#define CUP   C(KC_UP)
#define CDOWN C(KC_DOWN)
#define CLEFT C(KC_LEFT)
#define ESCFN LT(_FN, KC_ESC)
#define OOSSLL OSL(_OSL)

// Variables
static uint8_t active_base_layer;

// Layer manager
void move_layer(bool up);

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

enum custom_keycodes {
  LUP = SAFE_RANGE,
  LDOWN,
  HARR1,
  HARR2
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_WM] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                               KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_CAPS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB,  KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,                               KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSLS,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LCTL, KC_A,    KC_S,    KC_H,    KC_T,    KC_G,                               KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    OOSSLL,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    ESCFN,            KC_LALT, KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    KC_DEL,  KC_LGUI, KC_SPC,                    KC_ENT,  MO(_FN), KC_BSPC
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_FN] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F11,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,                            _______, KC_LCBR, KC_RCBR, KC_EQL,  KC_PLUS, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,                            _______, KC_LBRC, KC_RBRC, KC_QUOT, KC_DQUO, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, CLEFT,   CDOWN,   CUP,     CRGHT,   _______, LDOWN,            LUP,     _______, _______, _______, KC_MINS, KC_UNDS, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_OSL] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, HARR1,   HARR2,   _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_QWER] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______,          _______, KC_N,    KC_M,    _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_RAISE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_TOG, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                            KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     RGB_MOD, KC_MPRV, KC_MNXT, KC_VOLU, KC_PGUP, KC_UNDS,                            KC_EQL,  KC_HOME, RGB_HUI, RGB_SAI, RGB_VAI, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_MUTE, KC_MSTP, KC_MPLY, KC_VOLD, KC_PGDN, KC_MINS, KC_LPRN,          _______, KC_PLUS, KC_END,  RGB_HUD, RGB_SAD, RGB_VAD, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),
};

// INIT FUNCTION ///////////////////////////////////////////////////////////////

void keyboard_post_init_user(){
  active_base_layer = _WM;
}

// LAYER CONTROL ///////////////////////////////////////////////////////////////

void move_layer(bool up){
  switch(active_base_layer){
    case _WM:
      if(up){
        active_base_layer = _QWER;
        layer_on(_QWER);
      }
      break;
    case _QWER:
      if(up){

      }
      else {
        active_base_layer = _WM;
        layer_off(_QWER);
      }
      break;
  }
}

// CUSTOM KEYCODING ////////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode){
    case LUP: // Layer up
      if(record->event.pressed){
        move_layer(true);
        return false;
      }
      break;
    case LDOWN: // Layer down
      if(record->event.pressed){
        move_layer(false);
        return false;
      }
      break;
    case HARR1: // ->
      if(record->event.pressed){
        SEND_STRING("->");
      }
      break;
    case HARR2: // =>
      if(record->event.pressed){
        SEND_STRING("=>");
      }
      break;
  }
  return true;
}

// COMBOS //////////////////////////////////////////////////////////////////////

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
