# QMK Userspace — Halcyon Ferris

Custom firmware for a splitkb Halcyon Ferris 34-key split keyboard.

## Safety Rules

**Read before making ANY firmware changes.** See also: https://docs.qmk.fm/faq_misc#safety-considerations

- NEVER remove `QK_BOOT` from the keymap. Without it, entering DFU mode requires
  physical disassembly. Always ensure at least one layer contains QK_BOOT.
- NEVER modify files outside `keyboards/splitkb/halcyon/ferris/keymaps/nils/`.
  The `users/halcyon_modules/` directory is maintained by splitkb — do not edit it.
- Keep a known-good .uf2 backup before flashing experimental firmware.
- EEPROM has ~100,000 write cycles. Don't reflash in a tight loop during development.
- `qmk flash` is denied in Claude Code settings — flashing always requires explicit
  user confirmation. This is enforced via `.claude/settings.json` permissions.

## Project Structure

```
keyboards/splitkb/halcyon/ferris/keymaps/nils/   # <-- YOUR KEYMAP (edit here)
  keymap.c          # Layer definitions, tap-dances, combos, RGB callbacks
  config.h          # Timing (TAPPING_TERM, COMBO_TERM, ONESHOT_TIMEOUT, etc.)
  rules.mk          # Feature flags (TAP_DANCE_ENABLE, COMBO_ENABLE, etc.)
users/halcyon_modules/                            # Halcyon module code (DO NOT EDIT)
docs/final-keyboard-plan-revised.md               # Layout design document (authoritative)
qmk.json                                         # Build targets for CI
.github/workflows/build_binaries.yaml             # GitHub Actions CI
```

### Hardware
- **Board:** Halcyon Ferris rev1 — RP2040, 16MB flash, 34 Choc hot-swap keys
- **Modules:** TFT display (HLC_TFT_DISPLAY), Cirque trackpad (HLC_CIRQUE_TRACKPAD)
- **Layout macro:** `LAYOUT_ferris_hlc` — 34 physical keys + 10 module positions (44 total)
- **Connectivity:** Wired USB-C only

## Build & Flash

### GitHub Actions (primary build method)
Push to the `halcyon` branch. GitHub Actions compiles all targets in `qmk.json`.
Download .uf2 files from the Releases tab.

### Local build (requires qmk CLI)
One-time setup (already done if `qmk config user.overlay_dir` shows this repo):
```bash
qmk config user.overlay_dir="$(realpath .)"
```

Build both halves (two separate commands — one per module flag):
```bash
# Right half (trackpad, master)
qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_CIRQUE_TRACKPAD=1 -e TARGET=halcyon_ferris_nils_trackpad

# Left half (TFT display)
qmk compile -kb splitkb/halcyon/ferris/rev1 -km nils -e HLC_TFT_DISPLAY=1 -e TARGET=halcyon_ferris_nils_display
```
Output .uf2 files are copied to the userspace root directory.

Hardware setup: display on left half, trackpad on right half (master).
The module flag (`HLC_CIRQUE_TRACKPAD` or `HLC_TFT_DISPLAY`) selects which
module code to compile. Each half needs its own .uf2.

### Flashing (RP2040 UF2)
1. Enter DFU mode: press QK_BOOT key in keymap, OR hold BOOT + press RESET on PCB
2. A USB drive named "RPI-RP2" appears
3. Copy the .uf2 file to the drive
4. Keyboard reboots automatically with new firmware

## Architecture

### Design document
The authoritative layout spec is `docs/final-keyboard-plan-revised.md`. Read it before
implementing keymap changes.

### Core principles
- **Tap-centric, minimal holds** — inspired by Ben Vallack's 34-key layout
  (https://github.com/benvallack/34-QMK-Ferris-Sweep)
- **Dvorak alphas** — letter positions are non-negotiable
- **4 layers:** Base (Dvorak) → Symbols → Numbers → Nav/Mouse
- **TO() layer switching** — right outer thumb advances, left outer thumb returns to base
- **One-shot Shift and Ctrl** on thumb keys (OSM) — no hold timing ambiguity
- **Only 2 home row mods: Alt and GUI** — minimizes misfires
- **Combos for brackets** — symmetric left/right hand pairs
- **Tap-dances on punctuation** — dual-function comma, dot, slash
- **EurKey OS layout** — firmware handles Dvorak, OS handles Umlauts via AltGr

## Coding Conventions

### Keymap format
Use `keymap.c` (not `keymap.json`). The design requires C-only features: tap-dances,
combos, custom RGB callbacks, display integration. If a `keymap.json` exists alongside
`keymap.c`, QMK ignores the C file — remove the JSON first.

### C style
- Use QMK keycode names (`KC_A`, `OSM(MOD_LSFT)`, `TO(1)`), not raw HID codes
- Use `#define` for layer indices, HSV colors, LED index constants
- Use `enum` for tap-dance IDs and custom keycodes
- Organize keymap.c: includes → defines/enums → layer keymaps → tap-dance logic
  → combo definitions → RGB callbacks → display code

### Feature flags (rules.mk)
Enable only what's needed:
```makefile
TAP_DANCE_ENABLE = yes
COMBO_ENABLE = yes
MOUSEKEY_ENABLE = yes
POINTING_DEVICE_ENABLE = yes    # Cirque trackpad
RGB_MATRIX_ENABLE = yes
USER_NAME := halcyon_modules    # Required for Halcyon module support
```

### QMK documentation
- Primary: https://docs.qmk.fm
- One-shot keys: https://docs.qmk.fm/one_shot_keys
- Combos: https://docs.qmk.fm/features/combo
- Tap-dance: https://docs.qmk.fm/features/tap_dance
- RGB Matrix: https://docs.qmk.fm/features/rgb_matrix
- Pointing devices: https://docs.qmk.fm/features/pointing_device
