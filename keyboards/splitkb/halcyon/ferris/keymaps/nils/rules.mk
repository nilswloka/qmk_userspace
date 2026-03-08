TAP_DANCE_ENABLE = yes
MOUSEKEY_ENABLE = yes
ENCODER_ENABLE = yes
ENCODER_MAP_ENABLE = yes
RGB_MATRIX_ENABLE = yes
COMBO_ENABLE = yes

# This adds module functionality to your keyboard (files found in users/halcyon_modules)
USER_NAME := halcyon_modules

# TFT display fonts and number images
# With HLC_TFT_DISPLAY the module rules.mk already adds these assets.
# Only add them manually for the trackpad build (which has no display module).
ifndef HLC_TFT_DISPLAY
HLC_DISPLAY_PATH = $(USER_PATH)/splitkb/hlc_tft_display
SRC += $(HLC_DISPLAY_PATH)/graphics/fonts/Retron2000-27.qff.c \
       $(HLC_DISPLAY_PATH)/graphics/fonts/Retron2000-underline-27.qff.c \
       $(HLC_DISPLAY_PATH)/graphics/numbers/0.qgf.c \
       $(HLC_DISPLAY_PATH)/graphics/numbers/1.qgf.c \
       $(HLC_DISPLAY_PATH)/graphics/numbers/2.qgf.c \
       $(HLC_DISPLAY_PATH)/graphics/numbers/3.qgf.c \
       $(HLC_DISPLAY_PATH)/graphics/numbers/undef.qgf.c
endif

# Custom fonts for OSM indicators
SRC += keyboards/splitkb/halcyon/ferris/keymaps/nils/fonts/Hack-16.qff.c
