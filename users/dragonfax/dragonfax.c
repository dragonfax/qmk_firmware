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

 #include "quantum.h"

extern keymap_config_t keymap_config;

enum custom_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  NEW_SAFE_RANGE,
};

#define KC_VOLDN KC__VOLDOWN
#define NUM_ROW {KC_GRAVE,KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  _______ }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* http://www.keyboard-layout-editor.com/#/gists/e0f05dcdbec66445c2f4449b9298b2b1 */

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  Ent |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | '    |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |      | OPT  | CMD  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = {
#if MATRIX_ROWS == 5
  NUM_ROW ,
#endif
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______},
  {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT },
  {_______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_QUOT},
  {KC_LCTL, _______, KC_LALT, KC_LGUI, LOWER,   SH_T(KC_SPC),  SH_T(KC_SPC),  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  Del |  Del |  Del |  Del |      | Left | Down | Up   |Right |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      | Mute | VolDn| VolUp|      |             |   [  |   ]  |   \  |   -  |  =   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | HOME | PgDn | PgUp | END  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = {
#if MATRIX_ROWS == 5
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
#else
  NUM_ROW ,
#endif
  {_______, KC_BSPC,  KC_BSPC,  KC_BSPC,  KC_BSPC,  _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,_______,_______},
  {_______, KC__MUTE,KC_VOLDN,KC__VOLUP,_______,_______, _______, KC_LBRC, KC_RBRC,KC_BSLASH,KC_MINUS,KC_EQUAL},
  {_______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END}
},

/* Adjust (Lower + Raise) */
[_ADJUST] = {
  {_______, _______, _______, _______, _______, RESET  , _______, _______, _______, _______, _______, _______},
#if MATRIX_ROWS == 5
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
#endif
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
  {_______, KC_ASON, KC_ASOFF,_______, _______, _______, _______, _______, _______, _______, _______, _______},
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
