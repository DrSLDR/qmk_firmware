#include QMK_KEYBOARD_H
#include "version.h"

enum layers {
    QWER,  // QWERTY layer
    WMAN,  // Workman layer
    UTIL,  // Utility layer
};

enum custom_keycodes {
    VRSN = SAFE_RANGE,
};

// State variables
static uint8_t topmost_active_layer;

// Combo declarations
// Combo things
enum combos {
  AO_ARING,       // Å, å
  AE_ADIAE,       // Ä, ä
  EO_ODIAE,       // Ö, ö
  EZ_EACUT,       // É, é
  SS_CAPS         // Caps via shifts
};

const uint16_t PROGMEM ao_combo[]  = {KC_A, KC_O, COMBO_END};       // Å, å
const uint16_t PROGMEM ae_combo[]  = {KC_A, KC_E, COMBO_END};       // Ä, ä
const uint16_t PROGMEM eo_combo[]  = {KC_E, KC_O, COMBO_END};       // Ö, ö
const uint16_t PROGMEM ez_combo[]  = {KC_E, KC_Z, COMBO_END};       // É, é
const uint16_t PROGMEM ss_combo[]  = {KC_LSFT, KC_RSFT, COMBO_END}; // Caps

combo_t key_combos[] = {
    COMBO(ao_combo, RALT(KC_W)),
    COMBO(ae_combo, RALT(KC_A)),
    COMBO(eo_combo, RALT(KC_O)),
    COMBO(ez_combo, RALT(KC_G)),
    COMBO(ss_combo, KC_CAPS),
};

// Functional macros
#define SL_WHAT KC_NO
#define SL_ESC LT(UTIL,KC_ESC)
#define SL_UTIL MO(UTIL)
#define SL_OSUT OSL(UTIL)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: QWERTY layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Grave  |   1  |   2  |   3  |   4  |   5  | noop |           | noop |   6  |   7  |   8  |   9  |   0  |  Bspc  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  | noop |           | noop |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtl   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '    |
 * |--------+------+------+------+------+------| OSL  |           | noop |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  | Util |           |      |   N  |   M  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |ESC/Ut| LGUI | noop | LAlt | Util |                                       | Util | Left |  Up  | Down | Right|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Prntsc| Ins  |       | RAlt | noop |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 | Space| Del  |------|       |------| Bspc | Entr |
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
[QWER] = LAYOUT_ergodox_pretty(
  // left hand
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    SL_WHAT,     SL_WHAT, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    SL_WHAT,     SL_WHAT, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    SL_OSUT,     SL_WHAT, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
  SL_ESC,  KC_LGUI, SL_WHAT, KC_LALT, SL_UTIL,                                         SL_UTIL, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT,
                                               KC_PSCR, KC_INS,      KC_RALT, SL_WHAT,
                                                        KC_HOME,     KC_PGUP,
                                      KC_SPC,  KC_DEL,  KC_END,      KC_PGDN, KC_BSPC, KC_ENT
),
/* Keymap 1: Workman Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |   D  |   R  |   W  |   B  |      |           |      |   J  |   F  |   U  |   P  |   ;  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |   H  |   T  |      |------|           |------|   Y  |   N  |   E  |   O  |   I  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |   M  |   C  |   V  |      |           |      |   K  |   L  |      |      |      |        |
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
[WMAN] = LAYOUT_ergodox_pretty(
  // left hand
  _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, KC_D,    KC_R,    KC_W,    KC_B,    _______,     _______, KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, _______,
  _______, _______, _______, KC_H,    KC_T,    _______,                       KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    _______,
  _______, _______, _______, KC_M,    KC_C,    KC_V,    _______,     _______, KC_K,    KC_L,    _______, _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______,
                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),
/* Keymap 2: Utility layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  | NumL |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |  7   |  8   |  9   |      |      |           |      |      |      |      |      |      |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |  4   |  5   |  6   |      |------|           |------|      |  [   |  ]   | -    | =    |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |  1   |  2   |  3   |      |      |           |      |      |  (   |  )   |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      | Home | PgUp | PgDn | End  |
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
  VRSN,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_NUM,      _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  _______, _______, KC_P7,   KC_P8,   KC_P9,   _______, _______,     _______, _______, _______, _______, _______, _______, KC_F12,
  _______, _______, KC_P4,   KC_P5,   KC_P6,   _______,                       _______, KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  _______,
  _______, _______, KC_P1,   KC_P2,   KC_P3,   _______, _______,     _______, _______, KC_LPRN, KC_RPRN, _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,
                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      _______, _______, QK_MAKE,     _______, _______, _______
),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case VRSN:
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
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

    topmost_active_layer = WMAN;

    layer_on(WMAN);
    layer_off(QWER);
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
