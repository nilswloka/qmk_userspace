// Halcyon Ferris — nils keymap configuration
#pragma once

// Home row mod timing
#define TAPPING_TERM 240

// If held past TAPPING_TERM without pressing another key, register tap on release
#define RETRO_TAPPING

// One-shot modifier behavior
#define ONESHOT_TAP_TOGGLE 2   // Double-tap to lock (sticky)
#define ONESHOT_TIMEOUT 3000   // Expire after 3 seconds if unused

// Encoder (Halcyon module)
#define ENCODER_RESOLUTION 2

// Combos
#define COMBO_TERM 40   // Max ms between combo key presses

// RGB Matrix defaults
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SAT 0
#define RGB_MATRIX_DEFAULT_VAL 40
#define RGB_MATRIX_TIMEOUT 300000  // 5 min idle auto-off

// TFT Display (secondary/left half)
// Pin definitions match the Halcyon TFT display module on VIK connector
#define LCD_RST_PIN GP26
#define LCD_CS_PIN GP13
#define LCD_DC_PIN GP16
#define LCD_SPI_DIVISOR 0
#define LCD_SPI_MODE 3
#define LCD_WIDTH 135
#define LCD_HEIGHT 240
#define LCD_ROTATION QP_ROTATION_0
#define LCD_OFFSET_X 52
#define LCD_OFFSET_Y 40
#define QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS TRUE
#define ST7789_NO_AUTOMATIC_VIEWPORT_OFFSETS
#define ST7789_NUM_DEVICES 1
#define SURFACE_NUM_DEVICES 1

// Display backlight on GP27 — controlled via direct GPIO in keymap.c
// (cannot use BACKLIGHT_PIN here; splitkb/config.h redefines it later)
