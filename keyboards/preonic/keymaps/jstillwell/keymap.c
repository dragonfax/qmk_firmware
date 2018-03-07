/* Copyright 2015-2017 Jack Humbert
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

#include "preonic.h"
#include "action_layer.h"

enum preonic_layers {
  _QWERTY,
  _LOWER,
  _ADJUST
};

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
};

enum {
  TD_ESC_GRAV = 0,
  TD_BRACKETS,
  TD_ENT_QUOT,
};

void dance_brackets (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code(KC_LBRC);
    unregister_code(KC_LBRC);
    reset_tap_dance (state);
  } else if (state->count == 2) {
    register_code(KC_RBRC);
    unregister_code(KC_RBRC);
    reset_tap_dance (state);
  } else {
    register_code(KC_BSLASH);
    unregister_code(KC_BSLASH);
    reset_tap_dance (state);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  //Tap once for Esc, twice for Caps Lock
  [TD_BRACKETS] = ACTION_TAP_DANCE_FN(dance_brackets),
// Other declarations would go here, separated by commas, if you have them
};

const uint16_t PROGMEM fn_actions[] = {
  ACTION_SWAP_HANDS_TAP_KEY(KC_SPC),
};

enum process_combo_event {
  RESET_COMBO_INDEX,
};

const uint16_t PROGMEM test_combo[] = {KC_Q, KC_P, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
  [RESET_COMBO_INDEX] = COMBO_ACTION(test_combo)
};

void process_combo_event(uint8_t combo_index, bool pressed) {
    switch(combo_index) {
    case RESET_COMBO_INDEX:
        if (pressed) {
          reset_keyboard();
        }
        break;
    }
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |  [   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Shift|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | NOOP | OPT  | CMD  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = {
  {KC_GRAVE,KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC},
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    TD(TD_BRACKETS)},
  {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENT) },
  {KC_LCTL, KC_NO,   KC_LGUI, KC_LALT, LOWER,   KC_FN0,  KC_FN0,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   -  |  =   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |   `  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |  ]   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  Del |  Del |  Del |  Del |  Del |  Del |   Del|   Del|   Del|   Del|  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |     Bksp    |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = {
  {_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_MINUS,KC_EQUAL},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, KC_BSPC, KC_BSPC, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END}
},

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case LOWER:
          if (record->event.pressed) {
            layer_on(_LOWER);
          } else {
            layer_off(_LOWER);
          }
          return false;
          break;
        case RAISE:
          if (record->event.pressed) {
            layer_on(_LOWER);
          } else {
            layer_off(_LOWER);
          }
          return false;
          break;
      }
    return true;
};
