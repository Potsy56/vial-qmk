/* Copyright 2020 Josef Adamcik
 * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
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

enum sofle_layers {
    _QWERTY,
    _COLEMAK,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
	KC_QWERTY = QK_KB_0,	//QWERTY layout
	KC_COLEMAK,				//COLEMAK layout
	KC_PRVWD,				//go to previous word
	KC_NXTWD,				//go to next word
	KC_LSTRT,				//go to start of line
	KC_LEND,				//go to end of line
	ATAB_F,					//Alt+Tab -> Forwards
	ATAB_R,					//Alt+Tab -> Reverse
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP),           ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }, //QWERTY
    [1] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) }, //COLEMAK
    [2] = { ENCODER_CCW_CW( ATAB_R,  ATAB_F),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI) }, //LOWER
    [3] = { ENCODER_CCW_CW(C(KC_Z), C(KC_Y)),           ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)}, //RAISE
    [4] = { ENCODER_CCW_CW(_______, _______),           ENCODER_CCW_CW(_______, _______) }  //ADJUST
};
#endif

//Variables for custom keycodes
#ifdef SUPER_ALT_TAB_ENABLE
	bool is_alt_tab_active = false; // Super Alt Tab Code
	uint16_t alt_tab_timer = 0;
#endif

//init colours
HSV HSV_UNDERGLOW = {0, 255, 255};
HSV HSV_INDICATOR = {0, 255, 255};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * .-----------------------------------------.                    .-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  \   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | LCtl |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * '-----------------------------------------/       /    \       \-----------------------------------------'
 *            | LGUI | LAlt | LCtl | LOWER| / Space /      \ Enter \  |RAISE | RCtl | Left |Right |
 *            |      |      |      |      |/       /        \       \ |      |      |  <-  |  ->  |
 *            '-----------------------------------'          '-----------------------------------'
 */
[0] = LAYOUT(
  QK_GESC,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_BSPC,
   KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSLS,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  SC_LSPO,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,   XXXXXXX, KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  SC_RSPC,
                 KC_LGUI, KC_LALT,  KC_SPC, TL_LOWR,  KC_SPC,    KC_ENT, TL_UPPR, KC_SPC,  KC_LEFT, KC_RGHT
),

/*
 * COLEMAK
 * .-----------------------------------------.                    .-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   F  |   P  |   G  |                    |   J  |   L  |   U  |   Y  |   ;  |  \   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   A  |   R  |   S  |   T  |   D  |-------.    ,-------|   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   K  |   M  |   ,  |   .  |   /  |RShift|
 * '-----------------------------------------/       /    \       \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | / Enter /      \ Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /        \       \ |      |      |      |      |
 *            `-----------------------------------'          '-----------------------------------'
 */
[1] = LAYOUT(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_BSPC,
  KC_ESC,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_G,                        KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN,  KC_BSLS,
  KC_TAB,   KC_A,   KC_R,    KC_S,    KC_T,    KC_D,                        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,      XXXXXXX, KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                       _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),

/* LOWER
 * .-----------------------------------------.                    .-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * '-----------------------------------------/       /    \       \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /      \ Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /        \       \ |      |      |      |      |
 *            `-----------------------------------'          '-----------------------------------'
 */
[2] = LAYOUT(
  KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                       _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),

/* RAISE
 * .-----------------------------------------.                    .-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /    \       \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | / Enter /      \ Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /        \       \ |      |      |      |      |
 *            `-----------------------------------'          '-----------------------------------'
 */
[3] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                      _______, _______ , _______,  _______,  _______, _______,
  _______,  KC_INS, KC_PSCR,  KC_APP, XXXXXXX, XXXXXXX,                      KC_PGUP, KC_PRVWD,   KC_UP, KC_NXTWD, C(KC_BSPC), KC_BSPC,
  _______, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS,                      KC_PGDN,  KC_LEFT, KC_DOWN,  KC_RGHT,  KC_DEL,  KC_BSPC,
  _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX, _______,    _______, XXXXXXX, KC_LSTRT, XXXXXXX,  KC_LEND, XXXXXXX, _______,
                    _______, _______, _______, _______, _______,    _______, _______,  _______, _______,  _______
),

/* ADJUST
 * .-----------------------------------------.                    .-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |QK_BOOT|     |QWERTY|COLMAK|      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /    \       \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | / Enter /      \ Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /        \       \ |      |      |      |      |
 *            `-----------------------------------'          '-----------------------------------'
 */
 [4] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT, XXXXXXX,KC_QWERTY,KC_COLEMAK, XXXXXXX,XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                    _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______
  )
};


void matrix_scan_user(void) {
	#ifdef SUPER_ALT_TAB_ENABLE
		if (is_alt_tab_active) {	//Allows for use of super alt tab.
			if (timer_elapsed(alt_tab_timer) > 1000) {
				unregister_code(KC_LALT);
				is_alt_tab_active = false;
			}
		}
	#endif
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
		#ifdef SUPER_ALT_TAB_ENABLE
		case ATAB_F:	//Alt tab forwards
			if (record->event.pressed) {
				if (!is_alt_tab_active) {
					is_alt_tab_active = true;
					register_code(KC_LALT);
				}
					alt_tab_timer = timer_read();
					register_code(KC_TAB);
				} else {
					unregister_code(KC_TAB);
				}
			return true;
		case ATAB_R:	//Alt tab reverse
			if (record->event.pressed) {
				if (!is_alt_tab_active) {
					is_alt_tab_active = true;
					register_code(KC_LALT);
				}
					alt_tab_timer = timer_read();
					register_mods(mod_config(MOD_LSFT));
					register_code(KC_TAB);
				} else {
					unregister_mods(mod_config(MOD_LSFT));
					unregister_code(KC_TAB);
				}
			return true;
		#endif
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK);
            }
            return false;
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
            }
            break;
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
    }
    return true;
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

	switch (get_highest_layer(default_layer_state)) { // set underglow colour based on default layer
		case 0: //_QWERTY
			HSV_UNDERGLOW.h = 128; //HSV_CYAN
			HSV_UNDERGLOW.s = 255;
			HSV_UNDERGLOW.v = 255;
			break;
		case 1: //_COLEMAK
			HSV_UNDERGLOW.h = 213; //HSV_MAGENTA
			HSV_UNDERGLOW.s = 255;
			HSV_UNDERGLOW.v = 255;
			break;
		default:
			HSV_UNDERGLOW.h = 0; //WHITE
			HSV_UNDERGLOW.s = 0;
			HSV_UNDERGLOW.v = 255;
			break;
	}

	switch (get_highest_layer(layer_state)) { // set indicator colour based on current layer state
		case 2:
			HSV_INDICATOR.h =  43; //HSV_YELLOW
			HSV_INDICATOR.s = 255;
			HSV_INDICATOR.v = INDICATOR_BRIGHTNESS;
			break;
		case 3:
			HSV_INDICATOR.h =  85; //HSV_GREEN
			HSV_INDICATOR.s = 255;
			HSV_INDICATOR.v = INDICATOR_BRIGHTNESS;
			break;
		case 4:
			HSV_INDICATOR.h =   0; //HSV_RED
			HSV_INDICATOR.s = 255;
			HSV_INDICATOR.v = INDICATOR_BRIGHTNESS;
			break;
		case 5:
			HSV_INDICATOR.h = 170; //HSV_BLUE
			HSV_INDICATOR.s = 255;
			HSV_INDICATOR.v = INDICATOR_BRIGHTNESS;
			break;
		case 6:
			HSV_INDICATOR.h = 234; //HSV_PINK
			HSV_INDICATOR.s = 128;
			HSV_INDICATOR.v = INDICATOR_BRIGHTNESS;
		default:
			HSV_INDICATOR.h = HSV_UNDERGLOW.h; //use underglow colour as default
			HSV_INDICATOR.s = HSV_UNDERGLOW.s;
			HSV_INDICATOR.v = INDICATOR_BRIGHTNESS;
			break;
	}

/**
------- HSV COLOUR REFERENCES -------
**These don't work in VIAL**
-> see QMK pull request #24471:
	Rename RGB and HSV structs

#define HSV_AZURE       132, 102, 255
#define HSV_BLACK         0,   0,   0
#define HSV_BLUE        170, 255, 255
#define HSV_CHARTREUSE   64, 255, 255
#define HSV_CORAL        11, 176, 255
#define HSV_CYAN        128, 255, 255
#define HSV_GOLD         36, 255, 255
#define HSV_GOLDENROD    30, 218, 218
#define HSV_GREEN        85, 255, 255
#define HSV_MAGENTA     213, 255, 255
#define HSV_ORANGE       21, 255, 255
#define HSV_PINK        234, 128, 255
#define HSV_PURPLE      191, 255, 255
#define HSV_RED           0, 255, 255
#define HSV_SPRINGGREEN 106, 255, 255
#define HSV_TEAL        128, 255, 128
#define HSV_TURQUOISE   123,  90, 112
#define HSV_WHITE         0,   0, 255
#define HSV_YELLOW       43, 255, 255
#define HSV_OFF         HSV_BLACK
**/


    //check brightness of LED matrix, ensure we don't go over this
	if (HSV_UNDERGLOW.v > rgb_matrix_get_val()) {
		HSV_UNDERGLOW.v = rgb_matrix_get_val();
	}
	RGB RGB_UNDERGLOW = hsv_to_rgb(HSV_UNDERGLOW);

	if (HSV_INDICATOR.v > rgb_matrix_get_val()) {
		HSV_INDICATOR.v = rgb_matrix_get_val();
	}
	RGB RGB_INDICATOR = hsv_to_rgb(HSV_INDICATOR);


	for (uint8_t i = led_min; i < led_max; i++) {
		if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) { //set underglow LEDs
			rgb_matrix_set_color(i, RGB_UNDERGLOW.r, RGB_UNDERGLOW.g, RGB_UNDERGLOW.b);
		}
		else if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_INDICATOR)) { //set indicator LEDS
			rgb_matrix_set_color(i, RGB_INDICATOR.r, RGB_INDICATOR.g, RGB_INDICATOR.b);
		}
	}
	return false;
}
