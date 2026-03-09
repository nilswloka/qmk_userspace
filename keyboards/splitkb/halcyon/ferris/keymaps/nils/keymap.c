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

// Tap-dance indices
enum {
    TD_COMM_DASH,   // tap = , / hold = -
    TD_DOT_EXLM,   // tap = . / hold = !
    TD_COLN_SCLN,   // tap = : / hold = ;
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
         KC_QUOT,          TD(TD_COMM_DASH), TD(TD_DOT_EXLM), KC_P, KC_Y,    KC_F,     KC_G,     KC_C,     KC_R,        KC_L,
    //  | a       | o               | e              | u       | i       |   | d       | h       | t       | n          | s          |
         KC_A,         KC_O,         KC_E,           KC_U,     KC_I,        KC_D,     KC_H,     KC_T,     KC_N,         KC_S,
    //  | : / ;  | q               | j              | k       | x       |   | b       | m       | w       | v          | z          |
         TD(TD_COLN_SCLN), KC_Q,     KC_J,           KC_K,     KC_X,        KC_B,     KC_M,     KC_W,     KC_V,        KC_Z,
    //  +--------+-----------------+---+------------+---------+         +---+         +---------+---------+------------+------------+
    //                                 | SPC        | OSM SFT |         |   | OSM CTL | TO(1)   |
                                        KC_SPC,              OSM(MOD_LSFT), OSM(MOD_LCTL), TO(_SYMBOLS),
    //  Module positions (all unused)
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [_SYMBOLS] = LAYOUT_ferris_hlc(
    //  +---------+---------+---------+---------+---------+   +---------+---------+---------+---------+---------+
    //  | ESC     | @       | #       | $       | %       |   | ^       | &       | *       | ;       | BKSP    |
         KC_ESC,   KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,     KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_SCLN,  KC_BSPC,
    //  | TAB     | =       | '       | "       | +       |   | \       | {       | }       | |       | ENTER   |
         KC_TAB,   KC_EQL,   KC_QUOT,  KC_DQUO,  KC_PLUS,     KC_BSLS,  KC_LCBR,  KC_RCBR,  KC_PIPE,  KC_ENT,
    //  | ~       | `       | /       | _       | ?       |   | [       | (       | )       | ]       | TO(3)   |
         KC_TILD,  KC_GRV,   KC_SLSH,  KC_UNDS,  KC_QUES,     KC_LBRC,  KC_LPRN,  KC_RPRN,  KC_RBRC,  TO(_NAV),
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

// ─── Combos (bracket pairs on innermost columns) ──────────────────────
//
// Left hand → opening bracket, right hand → closing bracket
// Top row: ()   Home row: []   Bottom row: {}

// OSM AltGr combos (index + middle, top row, symmetric)
const uint16_t PROGMEM combo_ralt_r[]   = {KC_G, KC_C, COMBO_END};
const uint16_t PROGMEM combo_ralt_l[]   = {KC_P, TD(TD_DOT_EXLM), COMBO_END};

const uint16_t PROGMEM combo_lparen[]   = {KC_P, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_rparen[]   = {KC_F, KC_G, COMBO_END};
const uint16_t PROGMEM combo_lbracket[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM combo_rbracket[] = {KC_D, KC_H, COMBO_END};
const uint16_t PROGMEM combo_lbrace[]   = {KC_K, KC_X, COMBO_END};
const uint16_t PROGMEM combo_rbrace[]   = {KC_B, KC_M, COMBO_END};

// OSM modifiers — 3-key combos on top row (all layers)
// Left 1-2-3 (pinky+ring+middle): OSM Alt
const uint16_t PROGMEM combo_osm_lalt[] = {KC_QUOT, TD(TD_COMM_DASH), TD(TD_DOT_EXLM), COMBO_END};
// Left 2-3-4 (ring+middle+index): OSM GUI
const uint16_t PROGMEM combo_osm_lgui[] = {TD(TD_COMM_DASH), TD(TD_DOT_EXLM), KC_P, COMBO_END};
// Right 1-2-3 (pinky+ring+middle): OSM Alt
const uint16_t PROGMEM combo_osm_ralt[] = {KC_L, KC_R, KC_C, COMBO_END};
// Right 2-3-4 (ring+middle+index): OSM GUI
const uint16_t PROGMEM combo_osm_rgui[] = {KC_R, KC_C, KC_G, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_ralt_r,   OSM(MOD_RALT)),
    COMBO(combo_ralt_l,   OSM(MOD_RALT)),
    COMBO(combo_lparen,   KC_LPRN),
    COMBO(combo_rparen,   KC_RPRN),
    COMBO(combo_lbracket, KC_LBRC),
    COMBO(combo_rbracket, KC_RBRC),
    COMBO(combo_lbrace,   KC_LCBR),
    COMBO(combo_rbrace,   KC_RCBR),
    COMBO(combo_osm_lalt, OSM(MOD_LALT)),
    COMBO(combo_osm_lgui, OSM(MOD_LGUI)),
    COMBO(combo_osm_ralt, OSM(MOD_LALT)),
    COMBO(combo_osm_rgui, OSM(MOD_RGUI)),
};

// ─── RGB Matrix Indicators ────────────────────────────────────────────

// Precomputed unlocked OSM color (HSV 0, 255, 150 → pure red at value 150)
static const RGB osm_unlocked_rgb = {150, 0, 0};

static inline void set_osm_led(uint8_t led, bool locked, uint16_t now) {
    if (locked) {
        uint16_t t     = now % 500;
        uint8_t  pulse = (t < 250) ? t : (500 - t);
        uint8_t  val   = 80 + (pulse * 120) / 250;
        HSV      hsv   = {0, 255, val};
        RGB      rgb   = hsv_to_rgb(hsv);
        rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_matrix_set_color(led, osm_unlocked_rgb.r, osm_unlocked_rgb.g, osm_unlocked_rgb.b);
    }
}

// Cached layer colors — recomputed only on layer change
static RGB cached_bg_rgb;
static RGB cached_hi_rgb;
static uint8_t cached_layer = 0xFF;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);

    if (layer != cached_layer) {
        uint8_t bg_h, bg_s, bg_v;
        switch (layer) {
            case _BASE:    bg_h = 0;   bg_s = 0;   bg_v = 40;  break;
            case _SYMBOLS: bg_h = 85;  bg_s = 255; bg_v = 120; break;
            case _NUMBERS: bg_h = 170; bg_s = 255; bg_v = 120; break;
            case _NAV:     bg_h = 43;  bg_s = 255; bg_v = 120; break;
            default:       bg_h = 0;   bg_s = 0;   bg_v = 40;  break;
        }
        HSV bg_hsv = {bg_h, bg_s, bg_v};
        cached_bg_rgb = hsv_to_rgb(bg_hsv);

        uint8_t hi_s2 = (layer == _BASE) ? 0 : 255;
        HSV hi_hsv = {bg_h, hi_s2, 255};
        cached_hi_rgb = hsv_to_rgb(hi_hsv);

        cached_layer = layer;
    }

    RGB bg_rgb = cached_bg_rgb;
    RGB hi_rgb = cached_hi_rgb;

    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, bg_rgb.r, bg_rgb.g, bg_rgb.b);
    }

    // Helper: only set LEDs within the current batch range
    #define SET_HI(led) if ((led) >= led_min && (led) < led_max) \
        rgb_matrix_set_color((led), hi_rgb.r, hi_rgb.g, hi_rgb.b)

    switch (layer) {
        case _SYMBOLS:
        case _NUMBERS:
            // Navigation keys
            SET_HI(LED_ESC);
            SET_HI(LED_TAB);
            SET_HI(LED_BKSP);
            SET_HI(LED_ENTER);
            break;
        case _NAV:
            // Arrow keys
            SET_HI(LED_LEFT);
            SET_HI(LED_DOWN);
            SET_HI(LED_UP);
            SET_HI(LED_RIGHT);
            break;
    }

    #undef SET_HI

    // One-shot modifier indicators
    uint8_t osm      = get_oneshot_mods();
    uint8_t osm_lock = get_oneshot_locked_mods();
    uint16_t now     = timer_read();

    if (osm_lock & MOD_MASK_SHIFT || osm & MOD_MASK_SHIFT)
        if (LED_L_THUMB_OUTER >= led_min && LED_L_THUMB_OUTER < led_max)
            set_osm_led(LED_L_THUMB_OUTER, osm_lock & MOD_MASK_SHIFT, now);
    if (osm_lock & MOD_MASK_CTRL || osm & MOD_MASK_CTRL)
        if (LED_R_THUMB_INNER >= led_min && LED_R_THUMB_INNER < led_max)
            set_osm_led(LED_R_THUMB_INNER, osm_lock & MOD_MASK_CTRL, now);
    return false;
}

// ─── TFT Display (secondary/left half) ────────────────────────────────
//
// With HLC_TFT_DISPLAY the splitkb module (hlc_tft_display.c) owns LCD
// init and the surface→LCD flush.  We only draw custom content onto the
// module's global lcd_surface via the _user callbacks.
//
// With HLC_CIRQUE_TRACKPAD the module is not compiled, so we initialise
// the ST7789 and flush ourselves.
//
// Layout (135×240 portrait):
//   Top:    layer number image (colored per layer)
//   Middle: one-shot Shift / Ctrl indicators
//   Bottom: (reserved)

#include "qp.h"
#include "qp_surface.h"

// Graphics assets (object files added to SRC in rules.mk)
extern const uint8_t font_Retron2000_27[];
extern const uint8_t font_Hack_16[];
extern const uint8_t gfx_0[];
extern const uint8_t gfx_1[];
extern const uint8_t gfx_2[];
extern const uint8_t gfx_3[];
extern const uint8_t gfx_undef[];

#ifdef HLC_TFT_DISPLAY
// Module provides lcd and lcd_surface as globals
extern painter_device_t lcd;
extern painter_device_t lcd_surface;
#else
// Layer HSV colors (from hlc_tft_display.h — not available without the module)
#define HSV_LAYER_0   0,   0, 160
#define HSV_LAYER_1  23,  89, 255
#define HSV_LAYER_2  43,  71, 255
#define HSV_LAYER_3   0,  82, 255
static painter_device_t      lcd;
static painter_device_t      lcd_surface;
#endif

// Dim color for inactive one-shot indicators
#define HSV_OSM_OFF   0, 0, 60

static painter_font_handle_t  font_layer;     // Retron2000-27 for layer number
static painter_font_handle_t  font_osm;       // Hack-16 for OSM indicators
static painter_image_handle_t img_layers[5];   // 0-3 = layer images, 4 = undef

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
    if (is_keyboard_master()) return true;

#ifndef HLC_TFT_DISPLAY
    // No display module — we own hardware init
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
#endif

    font_layer = qp_load_font_mem(font_Retron2000_27);
    font_osm   = qp_load_font_mem(font_Hack_16);

    img_layers[0] = qp_load_image_mem(gfx_0);
    img_layers[1] = qp_load_image_mem(gfx_1);
    img_layers[2] = qp_load_image_mem(gfx_2);
    img_layers[3] = qp_load_image_mem(gfx_3);
    img_layers[4] = qp_load_image_mem(gfx_undef);

    return true;
}

bool display_module_housekeeping_task_user(bool second_display) {
    if (is_keyboard_master()) return true;
    // Return false to prevent the module from drawing lock indicators on top.
    // We handle all drawing and flushing ourselves.
    if (second_display) return false;

    static layer_state_t last_layer = ~(layer_state_t)0;
    static uint8_t       last_osm   = 0xFF;
    static uint8_t       last_locks = 0xFF;

    uint8_t current_layer = get_highest_layer(layer_state | default_layer_state);
    uint8_t osm_mods  = get_oneshot_mods() | get_oneshot_locked_mods();
    uint8_t osm_state = ((osm_mods & MOD_MASK_SHIFT)  ? 1 : 0)
                      | ((osm_mods & MOD_MASK_CTRL)   ? 2 : 0)
                      | ((osm_mods & MOD_MASK_ALT)     ? 4 : 0);
    led_t led_state   = host_keyboard_led_state();
    uint8_t locks     = led_state.raw & 0x07;  // caps, num, scroll

    bool layer_changed = (layer_state != last_layer);
    bool osm_changed   = (osm_state != last_osm);
    bool locks_changed = (locks != last_locks);

    if (!layer_changed && !osm_changed && !locks_changed) goto flush;

    uint8_t lh, ls, lv;
    get_layer_hsv(current_layer, &lh, &ls, &lv);

    // Layer number image (top of screen, 75×105 starting at Y=5)
    if (layer_changed) {
        uint8_t idx = (current_layer <= 3) ? current_layer : 4;
        qp_drawimage_recolor(lcd_surface, 5, 5, img_layers[idx], lh, ls, lv, HSV_BLACK);
        last_layer = layer_state;
    }

    // One-shot modifier indicators (below layer image)
    uint8_t osm_diff = osm_state ^ last_osm;

    if (layer_changed || (osm_diff & 1)) {
        if (osm_state & 1)
            qp_drawtext_recolor(lcd_surface, 5, 115, font_osm, "SFT", lh, ls, lv, HSV_BLACK);
        else
            qp_drawtext_recolor(lcd_surface, 5, 115, font_osm, "SFT", HSV_OSM_OFF, HSV_BLACK);
    }

    if (layer_changed || (osm_diff & 2)) {
        if (osm_state & 2)
            qp_drawtext_recolor(lcd_surface, 5, 133, font_osm, "CTL", lh, ls, lv, HSV_BLACK);
        else
            qp_drawtext_recolor(lcd_surface, 5, 133, font_osm, "CTL", HSV_OSM_OFF, HSV_BLACK);
    }

    if (layer_changed || (osm_diff & 4)) {
        if (osm_state & 4)
            qp_drawtext_recolor(lcd_surface, 5, 151, font_osm, "ALT", lh, ls, lv, HSV_BLACK);
        else
            qp_drawtext_recolor(lcd_surface, 5, 151, font_osm, "ALT", HSV_OSM_OFF, HSV_BLACK);
    }
    last_osm = osm_state;

    // Lock indicators (bottom of screen)
    if (locks_changed) {
        if (led_state.caps_lock)
            qp_drawtext_recolor(lcd_surface, 5, 185, font_osm, "CAP", 0, 0, 200, HSV_BLACK);
        else
            qp_drawtext_recolor(lcd_surface, 5, 185, font_osm, "CAP", 0, 0, 50, HSV_BLACK);
        if (led_state.num_lock)
            qp_drawtext_recolor(lcd_surface, 5, 203, font_osm, "NUM", 0, 0, 200, HSV_BLACK);
        else
            qp_drawtext_recolor(lcd_surface, 5, 203, font_osm, "NUM", 0, 0, 50, HSV_BLACK);
        if (led_state.scroll_lock)
            qp_drawtext_recolor(lcd_surface, 5, 221, font_osm, "SCR", 0, 0, 200, HSV_BLACK);
        else
            qp_drawtext_recolor(lcd_surface, 5, 221, font_osm, "SCR", 0, 0, 50, HSV_BLACK);
        last_locks = locks;
    }

flush:
    qp_surface_draw(lcd_surface, lcd, 0, 0, 0);
    qp_flush(lcd);

    return false;
}
