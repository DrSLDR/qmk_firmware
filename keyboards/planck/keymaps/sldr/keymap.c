/* Copyright 2015-2021 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "version.h"
#include "muse.h"


enum planck_layers {
  _WM,
  _LOWER,
  _RAISE,
  _ADJUST,
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  // QMK bonus points
  VRSN,
  QMAK,
  // Swedish-mode hackery
  SWE_TOG,
  SWE_DOT,
  SWE_COM,
  SWE_SLS,
  SWE_SCL,
  SWE_QOT,
  SWE_TIL,
  SWE_GRV,
  SWE_AT,
  SWE_DOL,
  SWE_AMP,
  SWE_CAR,
  SWE_AST,
  SWE_LPR,
  SWE_RPR,
  SWE_PIP,
  SWE_BSL,
  SWE_LBR,
  SWE_RBR,
  SWE_LCB,
  SWE_RCB,
  SWE_EQL,
  SWE_MIN,
  SWE_UND,
  SWE_PLS,
};

// Modifier store we will need later
uint8_t mods;
// Declare a "we are in Sweden" toggle
static bool swe_mode;
static bool swe_key_held;
static uint16_t swe_held_kc;

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define PIPE  S(KC_BSLS)
#define ARING RALT(KC_A)    // Å, å
#define ADIAE RALT(KC_QUOT) // Ä, ä
#define ODIAE RALT(KC_O)    // Ö, ö
#define EACUT RALT(KC_Y)    // É, é
// Ugly QMK hackarounds
#define RESET QK_BOOTLOADER
#define DEBUG QK_DEBUG_TOGGLE
// More Swedish-mode hackery
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
  PAR_SECT,       // °, §
  EUR_EURO,       // €
};

const uint16_t PROGMEM ao_combo[]  = {KC_A, KC_O, COMBO_END};       // Å, å
const uint16_t PROGMEM ae_combo[]  = {KC_A, KC_E, COMBO_END};       // Ä, ä
const uint16_t PROGMEM eo_combo[]  = {KC_E, KC_O, COMBO_END};       // Ö, ö
const uint16_t PROGMEM eu_combo[]  = {KC_E, KC_U, COMBO_END};       // É, é
const uint16_t PROGMEM par_combo[] = {KC_P, KC_A, KC_R, COMBO_END}; // §
const uint16_t PROGMEM eur_combo[] = {KC_E, KC_U, KC_R, COMBO_END}; // €

combo_t key_combos[COMBO_COUNT] = {
  [AO_ARING] = COMBO_ACTION(ao_combo),      // Å, å
  [AE_ADIAE] = COMBO_ACTION(ae_combo),      // Ä, ä
  [EO_ODIAE] = COMBO_ACTION(eo_combo),      // Ö, ö
  [EU_EACUT] = COMBO_ACTION(eu_combo),      // É, é
  [PAR_SECT] = COMBO_ACTION(par_combo),     // §
  [EUR_EURO] = COMBO_ACTION(eur_combo),     // €
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Workman
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   D  |   R  |   W  |   B  |   J  |   F  |   U  |   P  |   ;  |  <-  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   H  |   T  |   G  |   Y  |   N  |   E  |   O  |   I  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   M  |   C  |   V  |   K  |   L  |   ,  |   .  |   /  | Shift|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  | Alt  | NOOP | Meta | Lower| Space| Enter| Raise| Left | Up   | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_WM] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    KC_J,    KC_F,    KC_U,    KC_P,    SWE_SCL, KC_BSPC,
    KC_LCTL, KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    SWE_QOT,
    KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    KC_K,    KC_L,    SWE_COM, SWE_DOT, SWE_SLS, KC_RSFT,
    KC_ESC,  KC_LALT, XXXXXXX, KC_LGUI, LOWER,   KC_SPC,  KC_ENT,  RAISE,   KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   [  |   ]  |   =  |   +  | Caps |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   {  |   }  |   -  |   _  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Home | PgUp | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    SWE_TIL, KC_EXLM, SWE_AT,  KC_HASH, SWE_DOL, KC_PERC, SWE_CAR, SWE_AMP, SWE_AST, SWE_LPR, SWE_RPR, SWE_PIP,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   SWE_LBR, SWE_RBR, SWE_EQL, SWE_PLS, KC_CAPS,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  SWE_LCB, SWE_RCB, SWE_MIN, SWE_UND, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |   \  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   [  |   ]  |   =  |   +  | Caps |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   {  |   }  |   -  |   _  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Home | PgUp | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    SWE_GRV, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    SWE_BSL,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   SWE_LBR, SWE_RBR, SWE_EQL, SWE_PLS, KC_CAPS,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  SWE_LCB, SWE_RCB, SWE_MIN, SWE_UND, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END
),

/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * | MAKE | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|TermOn|TermOf|      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | SEMOD|      |      |      |      |  VERSION    |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    QMAK,    RESET,   DEBUG,   RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, RGB_VAD, KC_DEL ,
    _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, _______, _______,  _______, _______, _______,
    _______, _______, _______, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______,  _______, _______, _______,
    SWE_TOG, _______, _______, _______, _______, VRSN,    VRSN   , _______, _______,  _______, _______, _______
)

};

// INIT FUNCTION ///////////////////////////////////////////////////////////////

void keyboard_post_init_user(){
  swe_mode = true;
  swe_key_held = false;
  swe_held_kc = 0;
}

// MISC ////////////////////////////////////////////////////////////////////////

#ifdef AUDIO_ENABLE
  float plover_song[][2]     = SONG(PLOVER_SOUND);
  float plover_gb_song[][2]  = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  const bool p = record->event.pressed;
  switch (keycode) {
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
    // Swedish mode setter
    case SWE_TOG:
      if (p) {
        swe_mode = !swe_mode;
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
    case SWE_SCL:                         // Semicolon key
      swescape(KC_SCLN, p);
      sweheld(SWE_SCL, p);
      remap_shift(KC_SWE_COL_16, KC_SWE_SCL_16, p);
      return false;
    case SWE_QOT:                         //  Quote key
      swescape(KC_QUOT, p);
      sweheld(SWE_QOT, p);
      remap_shift(KC_SWE_DQT_16, KC_SWE_QOT, p);
      return false;
    case SWE_TIL:                         // Tilde key
      swescape16(KC_TILD, p);
      if (p) {
        tap_code16(KC_SWE_TIL_16);
        tap_code(KC_SPACE);
      }
      return false;
    case SWE_GRV:                         // Grave key
      swescape(KC_GRV, p);
      if (p) {
        tap_code(KC_SWE_ACT);
        tap_code(KC_SPACE);
      }
      return false;
    case SWE_AT:                          // At-sign
      swescape16(KC_AT, p);
      sweheld(SWE_AT, p);
      remap16(KC_SWE_AT_16, p);
      return false;
    case SWE_DOL:                         // Dollar sign
      swescape16(KC_DLR, p);
      sweheld(SWE_DOL, p);
      remap16(KC_SWE_DOL_16, p);
      return false;
    case SWE_CAR:                         // Caret
      swescape16(KC_CIRC, p);
      if (p) {
        tap_code16(KC_SWE_CAR_16);
        tap_code(KC_SPACE);
      }
      return false;
    case SWE_AMP:                         // Ampersand
      swescape16(KC_AMPR, p);
      sweheld(SWE_AMP, p);
      remap16(KC_SWE_AMP_16, p);
      return false;
    case SWE_AST:                         // Asterisk
      swescape16(KC_ASTR, p);
      sweheld(SWE_AST, p);
      remap16(KC_SWE_AST_16, p);
      return false;
    case SWE_LPR:                         // Left paren
      swescape16(KC_LPRN, p);
      sweheld(SWE_LPR, p);
      remap16(KC_SWE_LPR_16, p);
      return false;
    case SWE_RPR:                         // Right paren
      swescape16(KC_RPRN, p);
      sweheld(SWE_RPR, p);
      remap16(KC_SWE_RPR_16, p);
      return false;
    case SWE_PIP:                         // Pipe
      swescape16(PIPE, p);
      sweheld(SWE_PIP, p);
      remap16(KC_SWE_PIP_16, p);
      return false;
    case SWE_BSL:                         // Backslash
      swescape16(KC_BSLS, p);
      sweheld(SWE_BSL, p);
      remap16(KC_SWE_BSL_16, p);
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

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

#ifdef ENCODER_PRESENT
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
    return true;
}
#endif

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_song); }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_gb_song); }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
    return true;
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}

void process_combo_event(uint16_t combo_index, bool pressed) {
   switch(combo_index) {
      case AO_ARING:                  // Å, å
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_AO);
        } else {
          tap_code16(ALGR(KC_A));
        }
      }
      return;
    case AE_ADIAE:                    // Ä, ä
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_AE);
        } else {
          tap_code16(ALGR(KC_QUOT));
        }
      }
      return;
    case EO_ODIAE:                    // Ö, ö
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_OE);
        } else {
          tap_code16(ALGR(KC_O));
        }
      }
      return;
    case EU_EACUT:                    // É, é
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
      return;
    case PAR_SECT:                    // §
      if (pressed) {
        if (swe_mode) {
          tap_code(KC_SWE_PAR);
        } else {
          tap_code16(ALGR(KC_2));
        }
      }
      return;
    case EUR_EURO:                    // €
      if (pressed) {
        tap_code16(ALGR(KC_E));
      }
      break;
   }
}
