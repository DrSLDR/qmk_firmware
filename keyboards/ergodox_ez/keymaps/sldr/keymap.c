#include QMK_KEYBOARD_H
#include "version.h"

enum layers {
    BASE,  // default layer
    UTIL,  // utility
    MDIA,  // media keys
};

enum custom_keycodes {
#ifdef ORYX_CONFIGURATOR
    VRSN = EZ_SAFE_RANGE,
#else
    VRSN = SAFE_RANGE,
    QMAK
#endif
};

// Declare variables we'll play with later
uint8_t mods;

// Helpful shorthands
#define CRGHT C(KC_RGHT)
#define CUP   C(KC_UP)
#define CDOWN C(KC_DOWN)
#define CLEFT C(KC_LEFT)
#define ESCUT LT(UTIL, KC_ESC)

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
  THO_THRN        // Þ, þ
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
  [THO_THRN] = COMBO_ACTION(tho_combo)      // Þ, þ
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  | NOOP |           | NOOP |   6  |   7  |   8  |   9  |   0  | Bsp/MED|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   D  |   R  |   W  |   B  |  SA  |           |  CA  |   J  |   F  |   U  |   P  |   ;  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | Ctrl   |   A  |   S  |   H  |   T  |   G  |------|           |------|   Y  |   N  |   E  |   O  |   I  |   '    |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   M  |   C  |   V  |      |           |      |   K  |   L  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Esc/ut| Alt  | NOOP | NOOP | LGui |                                       | UTIL | Left |  Up  | Down | Right |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | PSCR | NOOP |       | NOOP | Pl/ps|
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | NOOP |       | NOOP |      |      |
 *                                 | Tab  | Space|------|       |------| Enter| Bspc |
 *                                 |      |      | NOOP |       | NOOP |      |      |
 *                                 `--------------------'       `--------------------'
 */
[BASE] = LAYOUT_ergodox_pretty(
  // left hand
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    XXXXXXX,              XXXXXXX,      KC_6,       KC_7,    KC_8,    KC_9,    KC_0,     LT(MDIA,KC_BSPC),
  KC_DEL,  KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    LSA(KC_NO),           LCA(KC_NO),   KC_J,       KC_F,    KC_U,    KC_P,    KC_SCLN,  KC_BSLS,
  KC_LCTL, KC_A,    KC_S,    KC_H,    KC_T,    KC_G,                                        KC_Y,       KC_N,    KC_E,    KC_O,    KC_I,     KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    HYPR(KC_NO),          MEH(KC_NO),   KC_K,       KC_L,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
  ESCUT,   KC_LALT, XXXXXXX, XXXXXXX, KC_LGUI,                                              MO(UTIL),   KC_LEFT, KC_UP,   KC_DOWN, KC_RIGHT,
                                              KC_PSCR,  XXXXXXX,              XXXXXXX,      KC_MPLY,
                                                        XXXXXXX,              XXXXXXX,
                                      KC_TAB, KC_SPC,   XXXXXXX,              XXXXXXX,      KC_ENT,     KC_BSPC
),
/* Keymap 1: Utility layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |  F1  |  F2  |  F3  |  F4  |  F5  |  F11 |           |  F12 |  F6  |  F7  |  F8  |  F9  |  F10 |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |  Caps   | Home | PgDn | PgUp | End  |      |      |           |      |      |   {  |   }  |   =  |   +  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |  Left|  Down|  Up  |Right |      |------|           |------|      |   [  |   ]  |   -  |   _  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         | CLeft| CDown| CUp  |CRight|      |      |           |      |      |      |      |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[UTIL] = LAYOUT_ergodox_pretty(
  // left hand
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,      KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
  KC_CAPS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______,     _______, _______, KC_LCBR, KC_RCBR, KC_EQL,  KC_PLUS, _______,
  _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,                       _______, KC_LBRC, KC_RBRC, KC_MINS, KC_UNDS, _______,
  _______, CLEFT,   CDOWN,   CUP,     CRGHT,   _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, _______, _______, _______,  _______,
                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),
/* Keymap 2: Media keys and other fun modifiers
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | VER  | MAKE |       | BOOT |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[MDIA] = LAYOUT_ergodox_pretty(
  // left hand
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, KC_MS_U, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                       _______, _______, _______, _______, _______, KC_MPLY,
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, KC_MPRV, KC_MNXT, _______, _______,
  _______, _______, _______, KC_BTN1, KC_BTN2,                                         KC_VOLU, KC_VOLD, KC_MUTE, _______, _______,

                                               VRSN,    QMAK,        QK_BOOT, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, KC_WBAK
),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case VRSN:
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                return false;
            case QMAK:
                mods = get_mods();
                if ((mods & (MOD_MASK_SHIFT)) && (mods & (MOD_MASK_CTRL))) {
                    clear_mods();
                    SEND_STRING("qmk flash -j 2 --keyboard " QMK_KEYBOARD " --keymap " QMK_KEYMAP);
                    wait_ms(150);
                    tap_code(KC_ENT);
                    reset_keyboard();
                } else if (mods & (MOD_MASK_SHIFT)) {
                    clear_mods();
                    SEND_STRING("qmk flash -j 2 --keyboard " QMK_KEYBOARD " --keymap " QMK_KEYMAP);
                } else {
                    SEND_STRING("qmk compile -j 2 --keyboard " QMK_KEYBOARD " --keymap " QMK_KEYMAP);
                }
                return false;
        }
    }
    return true;
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
    rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case 0:
#ifdef RGBLIGHT_COLOR_LAYER_0
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
            break;
        case 1:
            ergodox_right_led_1_on();
#ifdef RGBLIGHT_COLOR_LAYER_1
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
#endif
            break;
        case 2:
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_2
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
#endif
            break;
        case 3:
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_3
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
#endif
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_4
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
#endif
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_5
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
#endif
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_6
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
#endif
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_7
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
#endif
            break;
        default:
            break;
    }

    return state;
};

/* Combo management */
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
   }
}
