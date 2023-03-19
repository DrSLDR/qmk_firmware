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
#define KC_SWE_COL_16 S(KC_DOT)
#define KC_SWE_SCL_16 S(KC_COMM)
#define KC_SWE_LAN_16 KC_NUBS
#define KC_SWE_RAN_16 S(KC_NUBS)
#define KC_SWE_GRV_16 S(KC_EQL)
// Gnarly as sin function macro to handle press/depress remapping
#define remap(K, P) (P ? register_code(K) : unregister_code(K))
#define remap16(K, P) (P ? register_code16(K) : unregister_code16(K))
// Extremely gnarly shorthand for the swe-held-key handler
#define sweheld(K, P) if (P) {swe_key_held=true; swe_held_kc=K;} else {swe_key_held=false;}

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
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, PIPE,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_LBRC, KC_RBRC, KC_EQL,  KC_PLUS, KC_CAPS,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_LCBR, KC_RCBR, KC_MINS, KC_UNDS, _______,
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
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_LBRC, KC_RBRC, KC_EQL,  KC_PLUS, KC_CAPS,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_LCBR, KC_RCBR, KC_MINS, KC_UNDS, _______,
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
      if (swe_mode) {
        sweheld(SWE_DOT, p);
        mods = get_mods();
        if(mods & MOD_MASK_SHIFT) {       // >
          remap16(KC_SWE_RAN_16, p);
        } else {                          // .
          remap16(KC_DOT, p);
        }
      } else {
        remap(KC_DOT, p);
      }
      return false;
    case SWE_COM:                         // Comma key
      if (swe_mode) {
        sweheld(SWE_COM, p);
        mods = get_mods();
        if(mods & MOD_MASK_SHIFT) {       // <
          clear_mods();
          remap16(KC_SWE_LAN_16, p);
          set_mods(mods);
        } else {                          // ,
          remap16(KC_COMM, p);
        }
      } else {
        remap(KC_COMM, p);
      }
      return false;
    case SWE_SCL:                         // Semicolon key
      if (swe_mode) {
        sweheld(SWE_SCL, p);
        mods = get_mods();
        if (mods & MOD_MASK_SHIFT) {      // :
          remap16(KC_SWE_COL_16, p);
        } else {                          // ;
          remap16(KC_SWE_SCL_16, p);
        }
      } else {
        remap(KC_SCLN, p);
      }
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
