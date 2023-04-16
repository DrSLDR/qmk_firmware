#include QMK_KEYBOARD_H
#include "version.h"

enum layers {
    BASE,  // default layer
    UTIL,  // utility
    MDIA,  // media keys
    NMPD,  // Numpad
};

enum custom_keycodes {
#ifdef ORYX_CONFIGURATOR
    VRSN = EZ_SAFE_RANGE,
#else
    VRSN = SAFE_RANGE,
    QMAK,
    LUP,
    LDN,
    // Swedish-mode hackery
    SWE_TOG,
    SWE_DOT,
    SWE_COM,
    SWE_SLS,
    SWE_SCL,
    SWE_QOT,
    SWE_GRV,
    SWE_2,
    SWE_4,
    SWE_6,
    SWE_7,
    SWE_8,
    SWE_9,
    SWE_0,
    SWE_BSL,
    SWE_LBR,
    SWE_RBR,
    SWE_LCB,
    SWE_RCB,
    SWE_EQL,
    SWE_MIN,
    SWE_UND,
    SWE_PLS,
    SWE_MODE,
#endif
};

// Declare variables we'll play with later
uint8_t mods;
static uint8_t active_base_layer;
// Declare a Swedish mode enum
enum SWEDISH_MODE {
  ON,
  ON_SANS_NUMROW,
  OFF,
};
// Declare "we are in Sweden" toggles
static uint8_t active_swe_mode;
static bool swe_mode;
static bool swe_key_held;
static uint16_t swe_held_kc;

// Declare the layer management function
void move_layer(bool up);

// Layer management shortcuts
#define ABL_ON layer_on(active_base_layer)
#define ABL_OFF layer_off(active_base_layer)

// Helpful shorthands
#define CRGHT C(KC_RGHT)
#define CUP   C(KC_UP)
#define CDOWN C(KC_DOWN)
#define CLEFT C(KC_LEFT)
#define ESCUT LT(UTIL, KC_ESC)

// Proto-Swedish-mode shorthand
#define PIPE  S(KC_BSLS)
#define ARING RALT(KC_A)    // Å, å
#define ADIAE RALT(KC_QUOT) // Ä, ä
#define ODIAE RALT(KC_O)    // Ö, ö
#define EACUT RALT(KC_Y)    // É, é
// Actual swedish mode keycodes
#define KC_SWE_AO     KC_LBRC
#define KC_SWE_AE     KC_QUOT
#define KC_SWE_OE     KC_SCLN
#define KC_SWE_PAR    KC_GRV
#define KC_SWE_ACT    KC_EQL
#define KC_SWE_LAN    KC_NUBS
#define KC_SWE_QOT    KC_BSLS
#define KC_SWE_MIN    KC_SLSH
#define KC_SWE_PLS    KC_MINS
#define KC_SWE_COL_16 S(KC_DOT)
#define KC_SWE_SCL_16 S(KC_COMM)
#define KC_SWE_RAN_16 S(KC_NUBS)
#define KC_SWE_GRV_16 S(KC_EQL)
#define KC_SWE_SLS_16 S(KC_7)
#define KC_SWE_QST_16 S(KC_MINS)
#define KC_SWE_DQT_16 S(KC_2)
#define KC_SWE_TIL_16 ALGR(KC_RBRC)
#define KC_SWE_AT_16  ALGR(KC_2)
#define KC_SWE_DOL_16 ALGR(KC_4)
#define KC_SWE_AMP_16 S(KC_6)
#define KC_SWE_CAR_16 S(KC_RBRC)
#define KC_SWE_AST_16 S(KC_NUHS)
#define KC_SWE_LPR_16 S(KC_8)
#define KC_SWE_RPR_16 S(KC_9)
#define KC_SWE_PIP_16 ALGR(KC_NUBS)
#define KC_SWE_BSL_16 ALGR(KC_MINS)
#define KC_SWE_LCB_16 ALGR(KC_7)
#define KC_SWE_RCB_16 ALGR(KC_0)
#define KC_SWE_LBR_16 ALGR(KC_8)
#define KC_SWE_RBR_16 ALGR(KC_9)
#define KC_SWE_EQL_16 S(KC_0)
#define KC_SWE_UND_16 S(KC_SWE_MIN)
// Gnarly as sin function macro to handle press/depress remapping
#define remap(K, P) (P ? register_code(K) : unregister_code(K))
#define remap16(K, P) (P ? register_code16(K) : unregister_code16(K))
// Extremely gnarly shorthand for the swe-held-key handler
#define sweheld(K, P) if (P) {swe_key_held=true; swe_held_kc=K;} else {swe_key_held=false;}
// More shorthand to the people
// Handles "simple" shift-dependent key remapping
#define remap_shift(SK, NK, P) ((get_mods() & MOD_MASK_SHIFT) ? remap16(SK, P) : remap16(NK, P))
// Provides an early-exit for non-swedish remappings
#define swescape(K, P) if (!swe_mode) {remap(K, P); return false;}
#define swescape16(K, P) if (!swe_mode) {remap16(K, P); return false;}

// Combo things
enum combos {
  AO_ARING,       // Å, å
  AE_ADIAE,       // Ä, ä
  EO_ODIAE,       // Ö, ö
  EU_EACUT,       // É, é
  PAR_SECT,       // §
  EUR_EURO,       // €
};

const uint16_t PROGMEM ao_combo[]  = {KC_A, KC_O, COMBO_END};       // Å, å
const uint16_t PROGMEM ae_combo[]  = {KC_A, KC_E, COMBO_END};       // Ä, ä
const uint16_t PROGMEM eo_combo[]  = {KC_E, KC_O, COMBO_END};       // Ö, ö
const uint16_t PROGMEM eu_combo[]  = {KC_E, KC_U, COMBO_END};       // É, é
const uint16_t PROGMEM par_combo[] = {KC_P, KC_A, KC_R, COMBO_END}; // °, §
const uint16_t PROGMEM eur_combo[] = {KC_E, KC_U, KC_R, COMBO_END}; // €

combo_t key_combos[COMBO_COUNT] = {
  [AO_ARING] = COMBO_ACTION(ao_combo),      // Å, å
  [AE_ADIAE] = COMBO_ACTION(ae_combo),      // Ä, ä
  [EO_ODIAE] = COMBO_ACTION(eo_combo),      // Ö, ö
  [EU_EACUT] = COMBO_ACTION(eu_combo),      // É, é
  [PAR_SECT] = COMBO_ACTION(par_combo),     // §
  [EUR_EURO] = COMBO_ACTION(eur_combo)      // €
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  | NOOP |           | NOOP |   6  |   7  |   8  |   9  |   0  | Bsp/MED|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   D  |   R  |   W  |   B  |  CS  |           |  CA  |   J  |   F  |   U  |   P  |   ;  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | Ctrl   |   A  |   S  |   H  |   T  |   G  |------|           |------|   Y  |   N  |   E  |   O  |   I  |   '    |
 * |--------+------+------+------+------+------| Meh  |           |Hyper |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   M  |   C  |   V  |      |           |      |   K  |   L  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Esc/ut| Alt  | NOOP | NOOP | LGui |                                       | UTIL | Left |  Up  | Down | Right |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | PSCR | NOOP |       | NOOP | Pl/ps|
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | NOOP |       | NOOP |      |      |
 *                                 | Del  | Space|------|       |------| Enter| Bspc |
 *                                 |      |      |Esc/ut|       | NOOP |      |      |
 *                                 `--------------------'       `--------------------'
 */
[BASE] = LAYOUT_ergodox_pretty(
  // left hand
  SWE_GRV, KC_1,    SWE_2,   KC_3,    SWE_4,   KC_5,    XXXXXXX,              XXXXXXX,      SWE_6,      SWE_7,   SWE_8,   SWE_9,   SWE_0,     LT(MDIA,KC_BSPC),
  KC_TAB,  KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    RCS(KC_NO),           LCA(KC_NO),   KC_J,       KC_F,    KC_U,    KC_P,    SWE_SCL,  SWE_BSL,
  KC_LCTL, KC_A,    KC_S,    KC_H,    KC_T,    KC_G,                                        KC_Y,       KC_N,    KC_E,    KC_O,    KC_I,     SWE_QOT,
  KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    MEH(KC_NO),           HYPR(KC_NO),  KC_K,       KC_L,    SWE_COM, SWE_DOT, SWE_SLS,  KC_RSFT,
  ESCUT,   KC_LALT, XXXXXXX, XXXXXXX, KC_LGUI,                                              MO(UTIL),   KC_LEFT, KC_UP,   KC_DOWN, KC_RIGHT,

                                              KC_PSCR,  XXXXXXX,              XXXXXXX,      KC_MPLY,
                                                        XXXXXXX,              XXXXXXX,
                                      KC_DEL, KC_SPC,   ESCUT,                XXXXXXX,      KC_ENT,     KC_BSPC
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
 *   |       |      |      |      |      |                                       |      | Home | PgUp | PgDn | End   |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       | LyUP |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       | LyDN |      |      |
 *                                 `--------------------'       `--------------------'
 */
[UTIL] = LAYOUT_ergodox_pretty(
  // left hand
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,      KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
  KC_CAPS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______,     _______, _______, SWE_LCB, SWE_RCB, SWE_EQL, SWE_PLS, _______,
  _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,                       _______, SWE_LBR, SWE_RBR, SWE_MIN, SWE_UND, _______,
  _______, CLEFT,   CDOWN,   CUP,     CRGHT,   _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,

                                               _______, _______,     _______, _______,
                                                        _______,     LUP,
                                      _______, _______, _______,     LDN,     _______, _______
),
/* Keymap 2: Media keys and other fun modifiers
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | SWETOG |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |SEMODE|      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
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
  SWE_TOG,  _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______,  _______, _______, KC_MS_U, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
  _______,  _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                       _______, _______, _______, _______, _______, KC_MPLY,
  _______,  _______, _______, _______, _______, _______, _______,     _______, _______, _______, KC_MPRV, KC_MNXT, _______, _______,
  SWE_MODE, _______, _______, KC_BTN1, KC_BTN2,                                         KC_VOLU, KC_VOLD, KC_MUTE, _______, _______,

                                               VRSN,    QMAK,        QK_BOOT, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, KC_WBAK
),
/* Keymap 3: Right hand numpad
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
[NMPD] = LAYOUT_ergodox_pretty(
  // left hand
  _______, _______, _______, _______, _______, _______, _______,     KC_NUM,  _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______,     _______, KC_P7,   KC_P8,   KC_P9,  _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                       KC_P4,   KC_P5,   KC_P6,  _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______,     KC_P0,   KC_P1,   KC_P2,   KC_P3,  _______, _______, _______,
  _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______,

                                               _______, _______,     _______, _______,
                                                        _______,     _______,
                                      _______, _______, _______,     _______, _______, _______
),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  const bool p = record->event.pressed;
  switch(keycode) {
    case VRSN:
      if (p) {
        SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
    case QMAK:
      if (p) {
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
      }
      return false;
    case LUP:
      if (p) {
        move_layer(true);
      }
      return false;
    case LDN:
      if(p) {
        move_layer(false);
      }
      return false;
    // Swedish mode setter
    case SWE_TOG:
      if (p) {
        swe_mode = !swe_mode;
        switch(active_swe_mode) {
          case ON:
            active_swe_mode = OFF;
            break;
          case OFF:
            active_swe_mode = ON_SANS_NUMROW;
            break;
          case ON_SANS_NUMROW:
            active_swe_mode = ON;
            break;
        }
      }
      return false;
    // Tell the world about which mode we are in
    case SWE_MODE:
      if (p) {
        switch(active_swe_mode) {
          case ON:
            SEND_STRING("ON");
            break;
          case OFF:
            SEND_STRING("OFF");
            break;
          case ON_SANS_NUMROW:
            SEND_STRING("NUMROW");
            break;
        }
      }
      return false;
    // Special Swedish modifier handling
    case KC_LSFT:
    case KC_LCTL:
    case KC_LALT:
    case KC_RSFT:
    case KC_RCTL:
    case KC_ALGR:
      if (swe_mode && swe_key_held) {
        bool rp = p;
        record->event.pressed = false;
        process_record_user(swe_held_kc, record);
        record->event.pressed = rp;
      }
      return true;
    // Swedish key remappings
    case SWE_DOT:                         // Period key
      swescape(KC_DOT, p);
      sweheld(SWE_DOT, p);
      remap_shift(KC_SWE_RAN_16, KC_DOT, p);
      return false;
    case SWE_COM:                         // Comma key
      swescape(KC_COMM, p);
      sweheld(SWE_COM, p);
      mods = get_mods();
      if(mods & MOD_MASK_SHIFT) {         // <
        clear_mods();
        remap(KC_SWE_LAN, p);
        set_mods(mods);
      } else {                            // ,
        remap16(KC_COMM, p);
      }
      return false;
    case SWE_SLS:                         // Slash key
      swescape(KC_SLSH, p);
      sweheld(SWE_SLS, p);
      remap_shift(KC_SWE_QST_16, KC_SWE_SLS_16, p);
      return false;
    case SWE_QOT:                         //  Quote key
      swescape(KC_QUOT, p);
      sweheld(SWE_QOT, p);
      remap_shift(KC_SWE_DQT_16, KC_SWE_QOT, p);
      return false;
    case SWE_SCL:                         // Semicolon key
      swescape(KC_SCLN, p);
      sweheld(SWE_SCL, p);
      remap_shift(KC_SWE_COL_16, KC_SWE_SCL_16, p);
      return false;
    case SWE_BSL:                         // Backslash/Pipe key
      swescape16(KC_BSLS, p);
      sweheld(SWE_BSL, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_PIP_16, p);
        set_mods(mods);
      } else {
        remap16(KC_SWE_BSL_16, p);
      }
      return false;
    case SWE_GRV:                         // Grave/Tilde key
      swescape(KC_GRV, p);
      sweheld(SWE_GRV, p);
      if (p) {
        mods = get_mods();
        if (mods & MOD_MASK_SHIFT) {
          clear_mods();
          tap_code16(KC_SWE_TIL_16);
          tap_code(KC_SPACE);
          set_mods(mods);
        } else {
          tap_code16(KC_SWE_GRV_16);
          tap_code(KC_SPACE);
        }
      }
      return false;
    case SWE_2:                             // 2-key
      swescape(KC_2, p);
      sweheld(SWE_2, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_AT_16, p);
        set_mods(mods);
      } else {
        remap(KC_2, p);
      }
      return false;
    case SWE_4:                             // 4-key
      swescape(KC_4, p);
      sweheld(SWE_4, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_DOL_16, p);
        set_mods(mods);
      } else {
        remap(KC_4, p);
      }
      return false;
    case SWE_6:                             // 6-key
      swescape(KC_6, p);
      sweheld(SWE_6, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        if (p) {
          clear_mods();
          tap_code16(KC_SWE_CAR_16);
          tap_code(KC_SPACE);
          set_mods(mods);
        }
      } else {
        remap(KC_6, p);
      }
      return false;
    case SWE_7:                             // 7-key
      swescape(KC_7, p);
      sweheld(SWE_7, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_AMP_16, p);
        set_mods(mods);
      } else {
        remap(KC_7, p);
      }
      return false;
    case SWE_8:                             // 8-key
      swescape(KC_8, p);
      sweheld(SWE_8, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_AST_16, p);
        set_mods(mods);
      } else {
        remap(KC_8, p);
      }
      return false;
    case SWE_9:                             // 9-key
      swescape(KC_9, p);
      sweheld(SWE_9, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_LPR_16, p);
        set_mods(mods);
      } else {
        remap(KC_9, p);
      }
      return false;
    case SWE_0:                             // 0-key
      swescape(KC_0, p);
      sweheld(SWE_0, p);
      mods = get_mods();
      if (mods & MOD_MASK_SHIFT) {
        clear_mods();
        remap16(KC_SWE_RPR_16, p);
        set_mods(mods);
      } else {
        remap(KC_0, p);
      }
      return false;
    case SWE_LBR:                         // Left bracket
      swescape16(KC_LBRC, p);
      sweheld(SWE_LBR, p);
      remap16(KC_SWE_LBR_16, p);
      return false;
    case SWE_RBR:                         // Right bracket
      swescape16(KC_RBRC, p);
      sweheld(SWE_RBR, p);
      remap16(KC_SWE_RBR_16, p);
      return false;
    case SWE_LCB:                         // Left curly bracket
      swescape16(KC_LCBR, p);
      sweheld(SWE_LCB, p);
      remap16(KC_SWE_LCB_16, p);
      return false;
    case SWE_RCB:                         // Right curly bracket
      swescape16(KC_RCBR, p);
      sweheld(SWE_RCB, p);
      remap16(KC_SWE_RCB_16, p);
      return false;
    case SWE_EQL:                         // Equals
      swescape16(KC_EQL, p);
      sweheld(SWE_EQL, p);
      remap16(KC_SWE_EQL_16, p);
      return false;
    case SWE_MIN:                         // Minus
      swescape16(KC_MINS, p);
      sweheld(SWE_MIN, p);
      remap(KC_SWE_MIN, p);
      return false;
    case SWE_UND:                         // Underscore
      swescape16(KC_UNDS, p);
      sweheld(SWE_UND, p);
      remap16(KC_SWE_UND_16, p);
      return false;
    case SWE_PLS:                         // Plus
      swescape16(KC_PLUS, p);
      sweheld(SWE_PLS, p);
      remap(KC_SWE_PLS, p);
      return false;
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
  active_base_layer = BASE;
  active_swe_mode = ON;
  swe_mode = true;
  swe_key_held = false;
  swe_held_kc = 0;
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
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_3
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
#endif
            break;
        case 4:
            ergodox_right_led_3_on();
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
    case AO_ARING:                      // Å, å
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_AO);
        } else {
          tap_code16(ALGR(KC_A));
        }
      }
      break;
    case AE_ADIAE:                      // Ä, ä
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_AE);
        } else {
          tap_code16(ALGR(KC_QUOT));
        }
      }
      break;
    case EO_ODIAE:                      // Ö, ö
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_OE);
        } else {
          tap_code16(ALGR(KC_O));
        }
      }
      break;
    case EU_EACUT:                      // É, é
      if (pressed) {
        if (swe_mode) {
          mods = get_mods();
          clear_mods();
          tap_code(KC_SWE_ACT);
          set_mods(mods);
          tap_code(KC_E);
        } else {
          tap_code16(ALGR(KC_Y));
        }
      }
      break;
    case PAR_SECT:                      // §
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_PAR);
        } else {
          tap_code16(ALGR(KC_2));
        }
      }
      break;
    case EUR_EURO:
      if (pressed) {
        tap_code16(RALT(KC_E));         // €
      }
      break;
  }
}

// Layer manager
void move_layer(bool up){
  switch(active_base_layer){
    case BASE:
      if(up) {
        active_base_layer = NMPD;
        ABL_ON;
      }
      break;
    case NMPD:
      if(!up) {
        ABL_OFF;
        active_base_layer = BASE;
      }
    default:
      break;
  }
}
