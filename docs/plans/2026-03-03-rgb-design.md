# RGB Per-Key Layer Indication — Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add per-key RGB indicators that show active layer, highlight functional keys, and display one-shot modifier state.

**Architecture:** Single `rgb_matrix_indicators_advanced_user()` callback in keymap.c controls all 46 LEDs. Underglow off, per-key LEDs colored by layer, specific keys highlighted brighter, OSM thumb keys shown in red (solid armed, pulsing locked).

**Tech Stack:** QMK RGB Matrix API, `rgb_matrix_set_color()`, `timer_read()` for pulse animation.

---

## LED Index Reference

Derived from `g_led_config` in `users/halcyon_modules/splitkb/halcyon.c`.
The LAYOUT_ferris_hlc macro reverses left-hand columns (visual pos 0 = col 4 = pinky).

**Underglow (flag 2, set to OFF):** 0–5 (left), 23–28 (right)

**Per-key (flag 4):**

| Position          | Left LED | Right LED |
|-------------------|----------|-----------|
| Top row (pinky→inner) | 6,7,8,9,10 | 29,30,31,32,33 |
| Home row (pinky→inner) | 11,12,13,14,15 | 34,35,36,37,38 |
| Bottom row (pinky→inner) | 16,17,18,19,20 | 39,40,41,42,43 |
| Thumb (outer, inner) | 21, 22 | 44, 45 |

**Highlight targets:**

| Key function | LED index |
|---|---|
| a/Alt (base) | 11 |
| o/GUI (base) | 12 |
| n/GUI (base) | 37 |
| s/Alt (base) | 38 |
| ESC (layers 1–2) | 6 |
| TAB (layers 1–2) | 11 |
| BKSP (layers 1–2) | 33 |
| ENTER (layers 1–2) | 38 |
| Left arrow (layer 3) | 34 |
| Down arrow (layer 3) | 35 |
| Up arrow (layer 3) | 36 |
| Right arrow (layer 3) | 37 |
| OSM Shift thumb | 21 |
| OSM Ctrl thumb | 44 |

---

### Task 1: Enable RGB Matrix in build config

**Files:**
- Modify: `keyboards/splitkb/halcyon/ferris/keymaps/nils/rules.mk`
- Modify: `keyboards/splitkb/halcyon/ferris/keymaps/nils/config.h`

**Step 1: Add RGB_MATRIX_ENABLE to rules.mk**

Add after the existing feature flags (before the USER_NAME line):

```makefile
RGB_MATRIX_ENABLE = yes
```

**Step 2: Add RGB defaults to config.h**

Add after the encoder section, before the TFT display section:

```c
// RGB Matrix defaults
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SAT 0
#define RGB_MATRIX_DEFAULT_VAL 40
#define RGB_MATRIX_TIMEOUT 300000  // 5 min idle auto-off
```

**Step 3: Compile to verify RGB Matrix links**

Run: `qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1`
Expected: clean build with no errors. The `g_led_config` in halcyon.c compiles in automatically.

**Step 4: Commit**

```
feat: enable RGB Matrix in build config
```

---

### Task 2: Add LED index constants and layer color defines

**Files:**
- Modify: `keyboards/splitkb/halcyon/ferris/keymaps/nils/keymap.c`

**Step 1: Add LED index defines and color constants**

Add after the existing layer index `#define` block (after line 7), before the tap-dance enum:

```c
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
```

**Step 2: Compile to verify defines are valid**

Run: `qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1`
Expected: clean build (defines are unused so far, just syntax check).

**Step 3: Commit**

```
feat: add RGB LED index constants and layer color defines
```

---

### Task 3: Implement the RGB callback — layer colors and underglow off

**Files:**
- Modify: `keyboards/splitkb/halcyon/ferris/keymaps/nils/keymap.c`

**Step 1: Add the rgb_matrix_indicators_advanced_user callback**

Add before the TFT display section (before the `// ─── TFT Display` comment), after the encoder map `#endif`:

```c
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

    return false;
}
```

**Step 2: Compile**

Run: `qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1`
Expected: clean build. All per-key LEDs will now show the layer color.

**Step 3: Commit**

```
feat: add RGB callback with per-layer background colors
```

---

### Task 4: Add functional key highlights

**Files:**
- Modify: `keyboards/splitkb/halcyon/ferris/keymaps/nils/keymap.c`

**Step 1: Add highlight logic after the background loop**

Inside `rgb_matrix_indicators_advanced_user`, after the for-loop, before `return false`:

```c
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
```

**Step 2: Compile**

Run: `qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1`
Expected: clean build.

**Step 3: Commit**

```
feat: add functional key highlights per layer
```

---

### Task 5: Add one-shot modifier indicators with pulsing

**Files:**
- Modify: `keyboards/splitkb/halcyon/ferris/keymaps/nils/keymap.c`

**Step 1: Add OSM indicator logic**

Inside `rgb_matrix_indicators_advanced_user`, after the highlight switch, before `return false`:

```c
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
```

**Step 2: Compile**

Run: `qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1`
Expected: clean build.

**Step 3: Commit**

```
feat: add one-shot modifier LED indicators with pulsing lock
```

---

### Task 6: Final compile, push, and verify

**Step 1: Full clean build**

Run: `qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1`
Expected: clean build, no warnings.

**Step 2: Push**

```bash
git push
```

**Step 3: Visual verification (on hardware)**

Flash the .uf2 and verify:
- Base layer: all per-key LEDs dim white, home row mods (a, o, n, s) brighter
- Symbols layer: all green, ESC/TAB/BKSP/ENTER bright green
- Numbers layer: all blue, ESC/TAB/BKSP/ENTER bright blue
- Nav layer: all yellow, arrow keys bright yellow
- Underglow: off on all layers
- Tap OSM Shift: left outer thumb turns solid red
- Double-tap OSM Shift: left outer thumb pulses red
- Tap OSM Ctrl: right inner thumb turns solid red
- Double-tap OSM Ctrl: right inner thumb pulses red
- Idle 5 minutes: LEDs turn off
