#include QMK_KEYBOARD_H

// Layer indices
#define _BASE    0
#define _SYMBOLS 1
#define _NUMBERS 2
#define _NAV     3

// LED indices (from g_led_config in halcyon.c)
// Left hand: LAYOUT visual order maps pinky→col4, inner→col0
// Right hand: LAYOUT visual order maps inner→col0, pinky→col4
#define LED_L_THUMB_OUTER 21   // OSM Shift position
#define LED_L_THUMB_INNER 22   // Space/Meh position
#define LED_R_THUMB_INNER 44   // OSM Ctrl position
#define LED_R_THUMB_OUTER 45   // TO(layer) position

// Home row mod keys (base layer)
#define LED_A_ALT   11  // left pinky home
#define LED_O_GUI   12  // left ring home
#define LED_N_GUI   37  // right ring home
#define LED_S_ALT   38  // right pinky home

// Navigation keys shared across layers 1-2
#define LED_ESC     6   // left top pinky
#define LED_TAB     11  // left home pinky (same physical key as a)
#define LED_BKSP    33  // right top inner
#define LED_ENTER   38  // right home pinky (same physical key as s)

// Arrow keys (nav layer, right home row)
#define LED_LEFT    34
#define LED_DOWN    35
#define LED_UP      36
#define LED_RIGHT   37

// Layer background colors (H, S, V)
#define HSV_RGB_BASE     0,   0,  40   // dim white
#define HSV_RGB_SYMBOLS  85, 255, 120  // green
#define HSV_RGB_NUMBERS 170, 255, 120  // blue
#define HSV_RGB_NAV      43, 255, 120  // yellow

// Functional highlight: same hue as layer, full brightness
#define HSV_RGB_HIGHLIGHT_BASE     0,   0, 255
#define HSV_RGB_HIGHLIGHT_SYMBOLS  85, 255, 255
#define HSV_RGB_HIGHLIGHT_NUMBERS 170, 255, 255
#define HSV_RGB_HIGHLIGHT_NAV      43, 255, 255

// One-shot modifier indicator
#define HSV_RGB_OSM_ARMED  0, 255, 150  // solid red

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
    //  +---------+---------+---------+---------+---------+   +---------+---------+---------+---------+---------+
    //  | G+S+Lf  | ScLf    | MsUp    | ScRt    | G+S+Rt  |   | Back    | PTab    | NTab    | Fwd     | BKSP    |
         G(S(KC_LEFT)), MS_WHLL, MS_UP, MS_WHLR, G(S(KC_RGHT)), KC_WBAK, C(S(KC_TAB)), C(KC_TAB), KC_WFWD, KC_BSPC,
    //  | G+S+Up  | MsLf    | MsDn    | MsRt    | Del     |   | Left    | Down    | Up      | Right   | ENTER   |
         G(S(KC_UP)), MS_LEFT, MS_DOWN, MS_RGHT, KC_DEL,      KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_ENT,
    //  | G+S+Dn  | Btn2    | ScUp    | ScDn    | G+S+A+M |   | Btn1    | Home    | PgUp    | PgDn    | BOOT    |
         G(S(KC_DOWN)), MS_BTN2, MS_WHLU, MS_WHLD, G(S(A(KC_M))), MS_BTN1, KC_HOME, KC_PGUP, KC_PGDN, QK_BOOT,
    //                                | TO(0)   | (none)  |   | RAlt    | TO(1)   |
                                       TO(_BASE), KC_NO,       KC_RALT, TO(_SYMBOLS),
    //  Module positions
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

// ─── RGB Matrix Indicators ────────────────────────────────────────────

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);

    // Pick layer background color
    uint8_t bg_h, bg_s, bg_v;
    switch (layer) {
        case _BASE:    bg_h = 0;   bg_s = 0;   bg_v = 40;  break;
        case _SYMBOLS: bg_h = 85;  bg_s = 255; bg_v = 120; break;
        case _NUMBERS: bg_h = 170; bg_s = 255; bg_v = 120; break;
        case _NAV:     bg_h = 43;  bg_s = 255; bg_v = 120; break;
        default:       bg_h = 0;   bg_s = 0;   bg_v = 40;  break;
    }

    // Convert HSV to RGB once for the background
    HSV bg_hsv = {bg_h, bg_s, bg_v};
    RGB bg_rgb = hsv_to_rgb(bg_hsv);

    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            rgb_matrix_set_color(i, 0, 0, 0);  // underglow off
        } else {
            rgb_matrix_set_color(i, bg_rgb.r, bg_rgb.g, bg_rgb.b);
        }
    }

    // Pick highlight color (same hue, full brightness)
    uint8_t hi_h = bg_h, hi_s = (layer == _BASE) ? 0 : 255, hi_v = 255;
    HSV hi_hsv = {hi_h, hi_s, hi_v};
    RGB hi_rgb = hsv_to_rgb(hi_hsv);

    switch (layer) {
        case _BASE:
            // Home row mods
            rgb_matrix_set_color(LED_A_ALT, hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_O_GUI, hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_N_GUI, hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_S_ALT, hi_rgb.r, hi_rgb.g, hi_rgb.b);
            break;
        case _SYMBOLS:
        case _NUMBERS:
            // Navigation keys
            rgb_matrix_set_color(LED_ESC,   hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_TAB,   hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_BKSP,  hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_ENTER, hi_rgb.r, hi_rgb.g, hi_rgb.b);
            break;
        case _NAV:
            // Arrow keys
            rgb_matrix_set_color(LED_LEFT,  hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_DOWN,  hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_UP,    hi_rgb.r, hi_rgb.g, hi_rgb.b);
            rgb_matrix_set_color(LED_RIGHT, hi_rgb.r, hi_rgb.g, hi_rgb.b);
            break;
    }

    // One-shot modifier indicators
    uint8_t osm      = get_oneshot_mods();
    uint8_t osm_lock = get_oneshot_locked_mods();

    if (osm_lock & MOD_MASK_SHIFT) {
        // Locked: pulsing red (triangle wave ~2 Hz)
        uint16_t t = timer_read() % 500;
        uint8_t pulse = (t < 250) ? t : (500 - t);  // 0–250
        uint8_t val = 80 + (pulse * 120) / 250;      // 80–200
        HSV osm_hsv = {0, 255, val};
        RGB osm_rgb = hsv_to_rgb(osm_hsv);
        rgb_matrix_set_color(LED_L_THUMB_OUTER, osm_rgb.r, osm_rgb.g, osm_rgb.b);
    } else if (osm & MOD_MASK_SHIFT) {
        // Armed: solid red
        HSV osm_hsv = {0, 255, 150};
        RGB osm_rgb = hsv_to_rgb(osm_hsv);
        rgb_matrix_set_color(LED_L_THUMB_OUTER, osm_rgb.r, osm_rgb.g, osm_rgb.b);
    }

    if (osm_lock & MOD_MASK_CTRL) {
        uint16_t t = timer_read() % 500;
        uint8_t pulse = (t < 250) ? t : (500 - t);
        uint8_t val = 80 + (pulse * 120) / 250;
        HSV osm_hsv = {0, 255, val};
        RGB osm_rgb = hsv_to_rgb(osm_hsv);
        rgb_matrix_set_color(LED_R_THUMB_INNER, osm_rgb.r, osm_rgb.g, osm_rgb.b);
    } else if (osm & MOD_MASK_CTRL) {
        HSV osm_hsv = {0, 255, 150};
        RGB osm_rgb = hsv_to_rgb(osm_hsv);
        rgb_matrix_set_color(LED_R_THUMB_INNER, osm_rgb.r, osm_rgb.g, osm_rgb.b);
    }

    return false;
}

// ─── TFT Display (secondary/left half) ────────────────────────────────
//
// The display module source (hlc_tft_display.c) is not compiled when
// HLC_CIRQUE_TRACKPAD is set, so we initialise the ST7789 LCD and render
// directly.  The ST7789 SPI driver and Quantum Painter are always built
// by the Halcyon base rules.
//
// Layout (135×240 portrait):
//   Top:    layer number image (colored per layer)
//   Middle: one-shot Shift / Ctrl indicators
//   Bottom: (reserved)

#include "qp.h"
#include "qp_surface.h"

// Graphics assets (object files added to SRC in rules.mk)
extern const uint8_t font_Retron2000_27[];
extern const uint8_t font_Retron2000_underline_27[];
extern const uint8_t gfx_0[];
extern const uint8_t gfx_1[];
extern const uint8_t gfx_2[];
extern const uint8_t gfx_3[];
extern const uint8_t gfx_undef[];

// Layer HSV colors (from hlc_tft_display.h — not compiled with trackpad flag)
#define HSV_LAYER_0   0,   0, 160
#define HSV_LAYER_1  23,  89, 255
#define HSV_LAYER_2  43,  71, 255
#define HSV_LAYER_3   0,  82, 255

// Dim color for inactive one-shot indicators
#define HSV_OSM_OFF   0, 0, 60

static painter_device_t      lcd;
static painter_device_t      lcd_surface;
static painter_font_handle_t font_regular;
static painter_font_handle_t font_underline;

static void get_layer_hsv(uint8_t layer, uint8_t *h, uint8_t *s, uint8_t *v) {
    switch (layer) {
        case 0:  *h = 0;  *s = 0;   *v = 160; break;
        case 1:  *h = 23; *s = 89;  *v = 255; break;
        case 2:  *h = 43; *s = 71;  *v = 255; break;
        case 3:  *h = 0;  *s = 82;  *v = 255; break;
        default: *h = 0;  *s = 255; *v = 255; break;
    }
}

bool module_post_init_user(void) {
    if (is_keyboard_master()) return false;

    // Turn on TFT backlight (GP27)
    gpio_set_pin_output(GP27);
    gpio_write_pin_high(GP27);

    lcd = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT,
        LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, LCD_SPI_MODE);

    static uint8_t fb[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(135, 240, 16)];
    lcd_surface = qp_make_rgb565_surface(LCD_WIDTH, LCD_HEIGHT, fb);

    qp_init(lcd, LCD_ROTATION);
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);
    qp_rect(lcd, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, HSV_BLACK, true);
    qp_power(lcd, true);
    qp_flush(lcd);

    qp_init(lcd_surface, LCD_ROTATION);
    qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, HSV_BLACK, true);
    qp_surface_draw(lcd_surface, lcd, 0, 0, 0);
    qp_flush(lcd);

    font_regular   = qp_load_font_mem(font_Retron2000_27);
    font_underline = qp_load_font_mem(font_Retron2000_underline_27);

    return false;
}

bool display_module_housekeeping_task_user(bool second_display) {
    if (is_keyboard_master()) return false;

    static layer_state_t last_layer = ~(layer_state_t)0;
    static uint8_t       last_osm   = 0xFF;

    uint8_t current_layer = get_highest_layer(layer_state | default_layer_state);
    uint8_t osm_mods  = get_oneshot_mods() | get_oneshot_locked_mods();
    uint8_t osm_state = ((osm_mods & MOD_MASK_SHIFT) ? 1 : 0)
                      | ((osm_mods & MOD_MASK_CTRL)  ? 2 : 0);

    bool layer_changed = (layer_state != last_layer);
    bool osm_changed   = (osm_state != last_osm);

    if (!layer_changed && !osm_changed) return false;

    uint8_t lh, ls, lv;
    get_layer_hsv(current_layer, &lh, &ls, &lv);

    // Layer number image (top of screen)
    if (layer_changed) {
        painter_image_handle_t img;
        switch (current_layer) {
            case 0:  img = qp_load_image_mem(gfx_0); break;
            case 1:  img = qp_load_image_mem(gfx_1); break;
            case 2:  img = qp_load_image_mem(gfx_2); break;
            case 3:  img = qp_load_image_mem(gfx_3); break;
            default: img = qp_load_image_mem(gfx_undef); break;
        }
        qp_drawimage_recolor(lcd_surface, 5, 5, img, lh, ls, lv, HSV_BLACK);
        qp_close_image(img);
        last_layer = layer_state;
    }

    // One-shot modifier indicators (mid-screen)
    // Redrawn on layer OR osm change — active indicators use layer color
    if (osm_state & 1) {
        qp_drawtext_recolor(lcd_surface, 5, 120, font_underline, "SFT", lh, ls, lv, HSV_BLACK);
    } else {
        qp_drawtext_recolor(lcd_surface, 5, 120, font_regular, "SFT", HSV_OSM_OFF, HSV_BLACK);
    }

    if (osm_state & 2) {
        qp_drawtext_recolor(lcd_surface, 5, 150, font_underline, "CTL", lh, ls, lv, HSV_BLACK);
    } else {
        qp_drawtext_recolor(lcd_surface, 5, 150, font_regular, "CTL", HSV_OSM_OFF, HSV_BLACK);
    }
    last_osm = osm_state;

    // Push surface to physical LCD
    qp_surface_draw(lcd_surface, lcd, 0, 0, 0);
    qp_flush(lcd);

    return false;
}
