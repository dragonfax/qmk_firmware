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

#include "planck.h"
#include "action_layer.h"

extern keymap_config_t keymap_config;

enum planck_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
};

enum {
  TD_BRACKETS = 0,
  TD_DASHES,
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
  // Tap once for [, twice for ], 3 times for \ (back slash)
  [TD_BRACKETS] = ACTION_TAP_DANCE_FN(dance_brackets),
  [TD_DASHES] = ACTION_TAP_DANCE_DOUBLE(KC_MINUS,KC_EQUAL),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |  []\ |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  Ent |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | 'Shft|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | NOOP | OPT  | CMD  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = {
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    TD(TD_BRACKETS)},
  {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT},
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_QUOT) },
  {KC_LCTL, KC_NO,   KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | - =  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  Del |  Del |  Del |  Del |  Del |  Del |   Del|   Del|   Del|   Del|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | HOME | PgDn | PgUp | END  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = {
  {KC_GRAVE,KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    TD(TD_DASHES)},
  {_______, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, KC_BSPC, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END}
},

/* Adjust (Lower + Raise) */
[_ADJUST] = {
  {_______, _______, _______, _______, _______, RESET  , _______, _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
}


};


bool raise_pressed = false;
bool lower_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case LOWER:
          lower_pressed = record->event.pressed;
          if (record->event.pressed) {
            layer_on(_LOWER);
            if (lower_pressed && raise_pressed) {
              layer_on(_ADJUST);
            }
          } else {
            layer_off(_LOWER);
            layer_off(_ADJUST);
          }
          return false;
          break;
        case RAISE:
          raise_pressed = record->event.pressed;
          if (record->event.pressed) {
            layer_on(_LOWER);
            if (lower_pressed && raise_pressed) {
              layer_on(_ADJUST);
            }
          } else {
            layer_off(_LOWER);
            layer_off(_ADJUST);
          }
          return false;
          break;
      }
    return true;
};
