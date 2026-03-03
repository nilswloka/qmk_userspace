#include QMK_KEYBOARD_H

// Layer indices
#define _BASE    0
#define _SYMBOLS 1
#define _NUMBERS 2
#define _NAV     3

// Tap-dance indices
enum {
    TD_COMM_DASH,  // tap = , / hold = -
    TD_DOT_EXLM,  // tap = . / hold = !
    TD_COLN_SCLN,  // tap = : / hold = ;
};

// Tap-dance state detection
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
} td_state_t;

static td_state_t td_state;

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }
    return TD_UNKNOWN;
}

void td_comm_dash_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:  register_code(KC_COMM); break;
        case TD_SINGLE_HOLD: register_code(KC_MINS); break;
        default: break;
    }
}

void td_comm_dash_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:  unregister_code(KC_COMM); break;
        case TD_SINGLE_HOLD: unregister_code(KC_MINS); break;
        default: break;
    }
}

void td_dot_exlm_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:  register_code(KC_DOT); break;
        case TD_SINGLE_HOLD: register_code16(KC_EXLM); break;
        default: break;
    }
}

void td_dot_exlm_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:  unregister_code(KC_DOT); break;
        case TD_SINGLE_HOLD: unregister_code16(KC_EXLM); break;
        default: break;
    }
}

void td_coln_scln_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:  register_code16(KC_COLN); break;
        case TD_SINGLE_HOLD: register_code(KC_SCLN); break;
        default: break;
    }
}

void td_coln_scln_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:  unregister_code16(KC_COLN); break;
        case TD_SINGLE_HOLD: unregister_code(KC_SCLN); break;
        default: break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_COMM_DASH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_comm_dash_finished, td_comm_dash_reset),
    [TD_DOT_EXLM]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_dot_exlm_finished, td_dot_exlm_reset),
    [TD_COLN_SCLN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_coln_scln_finished, td_coln_scln_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_ferris_hlc(
    //  +--------+-----------------+----------------+---------+---------+   +---------+---------+---------+------------+------------+
    //  | '      | , / -           | . / !          | p       | y       |   | f       | g       | c       | r          | l          |
         KC_QUOT,  TD(TD_COMM_DASH), TD(TD_DOT_EXLM), KC_P,     KC_Y,        KC_F,     KC_G,     KC_C,     KC_R,        KC_L,
    //  | a/Alt  | o/GUI           | e              | u       | i       |   | d       | h       | t       | n/GUI      | s/Alt      |
         LALT_T(KC_A), LGUI_T(KC_O), KC_E,           KC_U,     KC_I,        KC_D,     KC_H,     KC_T,     RGUI_T(KC_N), LALT_T(KC_S),
    //  | : / ;  | q               | j              | k       | x       |   | b       | m       | w       | v          | z          |
         TD(TD_COLN_SCLN), KC_Q,     KC_J,           KC_K,     KC_X,        KC_B,     KC_M,     KC_W,     KC_V,        KC_Z,
    //  +--------+-----------------+---+------------+---------+         +---+         +---------+---------+------------+------------+
    //                                 | SPC/MEH    | OSM SFT |         |   | OSM CTL | TO(1)   |
                                        MT(MOD_MEH, KC_SPC), OSM(MOD_LSFT), OSM(MOD_LCTL), TO(_SYMBOLS),
    //  Module positions (all unused)
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [_SYMBOLS] = LAYOUT_ferris_hlc(
    //  +---------+---------+---------+---------+---------+   +---------+---------+---------+---------+---------+
    //  | ESC     | @       | #       | $       | %       |   | ^       | &       | *       | ;       | BKSP    |
         KC_ESC,   KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,     KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_SCLN,  KC_BSPC,
    //  | TAB     | =       | '       | "       | +       |   | \       | {       | }       | |       | ENTER   |
         KC_TAB,   KC_EQL,   KC_QUOT,  KC_DQUO,  KC_PLUS,     KC_BSLS,  KC_LCBR,  KC_RCBR,  KC_PIPE,  KC_ENT,
    //  | ~       | `       | /       | _       | (none)  |   | [       | (       | )       | ]       | TO(3)   |
         KC_TILD,  KC_GRV,   KC_SLSH,  KC_UNDS,  KC_NO,       KC_LBRC,  KC_LPRN,  KC_RPRN,  KC_RBRC,  TO(_NAV),
    //                                | TO(0)   | OSM SFT |   | RAlt    | TO(2)   |
                                       TO(_BASE), OSM(MOD_LSFT), KC_RALT, TO(_NUMBERS),
    //  Module positions
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [_NUMBERS] = LAYOUT_ferris_hlc(
    //  +---------+---------+---------+---------+---------+   +---------+---------+---------+---------+---------+
    //  | ESC     | F1      | F2      | F3      | F4      |   | -       | 7       | 8       | 9       | BKSP    |
         KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,       KC_MINS,  KC_7,     KC_8,     KC_9,     KC_BSPC,
    //  | TAB     | F5      | F6      | F7      | F8      |   | +       | 4       | 5       | 6       | ENTER   |
         KC_TAB,   KC_F5,    KC_F6,    KC_F7,    KC_F8,       KC_PLUS,  KC_4,     KC_5,     KC_6,     KC_ENT,
    //  | (none)  | F9      | F10     | .       | =       |   | 0       | 1       | 2       | 3       | *       |
         KC_NO,    KC_F9,    KC_F10,   KC_DOT,   KC_EQL,      KC_0,     KC_1,     KC_2,     KC_3,     KC_ASTR,
    //                                | TO(0)   | GUI     |   | RAlt    | TO(3)   |
                                       TO(_BASE), KC_LGUI,     KC_RALT, TO(_NAV),
    //  Module positions
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [_NAV] = LAYOUT_ferris_hlc(
    //  Stub layer -- QK_BOOT on bottom-right, thumb cluster populated
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, QK_BOOT,
    //                                | TO(0)   | (trns)  |   | RAlt    | TO(1)   |
                                       TO(_BASE), KC_TRNS,     KC_RALT, TO(_SYMBOLS),
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]    = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_SYMBOLS] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_NUMBERS] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
    [_NAV]     = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN) },
};
#endif
