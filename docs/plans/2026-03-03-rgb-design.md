# RGB Per-Key Layer Indication

Date: 2026-03-03

## Goal

Use the 34 per-key RGB LEDs as functional indicators: show which layer
is active, highlight important keys on each layer, and display one-shot
modifier state on the thumb keys.

## Hardware

- 46 LEDs total: 12 underglow (6 per half) + 34 per-key (SK6812MINI-E)
- LED matrix mapping already defined in `halcyon.c` (`g_led_config`)
- Underglow LEDs have flag `LED_FLAG_UNDERGLOW` (2); per-key have
  flag `LED_FLAG_KEYLIGHT` (4)

## Approach

Full per-key control via `rgb_matrix_indicators_advanced_user()`.
No runtime effect cycling. LEDs serve as indicators, not decoration.

## Color scheme

| Context              | H   | S   | V        | Visual             |
|----------------------|-----|-----|----------|--------------------|
| Base layer (all)     | 0   | 0   | 40       | Dim white          |
| Symbols layer (all)  | 85  | 255 | 120      | Green              |
| Numbers layer (all)  | 170 | 255 | 120      | Blue               |
| Nav layer (all)      | 43  | 255 | 120      | Yellow             |
| Functional highlight | layer hue | 255 | 255 | Bright layer color |
| OSM armed            | 0   | 255 | 150      | Solid red          |
| OSM locked           | 0   | 255 | 80–200   | Pulsing red ~2 Hz  |
| Underglow (all)      | 0   | 0   | 0        | Off                |

Functional highlights use the same hue as the layer at full brightness.

## Functional key highlights per layer

| Layer        | Highlighted keys                       |
|--------------|----------------------------------------|
| 0 (Base)     | Home row mods: a/Alt, o/GUI, n/GUI, s/Alt |
| 1 (Symbols)  | ESC, TAB, ENTER, BKSP                  |
| 2 (Numbers)  | ESC, TAB, ENTER, BKSP                  |
| 3 (Nav)      | Arrow keys: Left, Down, Up, Right      |

One-shot Shift and Ctrl thumb keys show red when armed (solid) or
locked (pulsing), regardless of active layer.

## Implementation

### rules.mk

```makefile
RGB_MATRIX_ENABLE = yes
```

Driver comes from the board-level info.json in QMK firmware.

### config.h

```c
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SAT 0
#define RGB_MATRIX_DEFAULT_VAL 40
#define RGB_MATRIX_TIMEOUT 300000   // 5 min idle auto-off
```

### keymap.c

Add `rgb_matrix_indicators_advanced_user()` callback:

1. For each LED in `[led_min, led_max)`:
   - If underglow flag: set black (off)
   - Otherwise: set layer background color (HSV converted to RGB)
2. Override functional highlight keys with bright layer color
3. Check `get_oneshot_mods()` and `get_oneshot_locked_mods()`:
   - Armed: set thumb LED to solid red
   - Locked: set thumb LED to pulsing red (triangle wave via `timer_read()`)
4. Return `false`

### LED index constants

Derived from `g_led_config` in `halcyon.c`. Need `#define` for:
- Home row mod positions (a, o, n, s)
- ESC, TAB, ENTER, BKSP positions (per layer if positions differ)
- Arrow key positions (nav layer)
- OSM Shift and Ctrl thumb positions

## Files modified

- `keyboards/splitkb/halcyon/ferris/keymaps/nils/keymap.c` — RGB callback
- `keyboards/splitkb/halcyon/ferris/keymaps/nils/config.h` — RGB defaults
- `keyboards/splitkb/halcyon/ferris/keymaps/nils/rules.mk` — enable flag
