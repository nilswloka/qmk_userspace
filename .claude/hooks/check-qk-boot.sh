#!/bin/bash
# Safety hook: verify QK_BOOT exists in keymap after edits
# QK_BOOT is the recovery key — without it, the keyboard cannot enter DFU mode
# and would require physical disassembly to reflash.

INPUT=$(cat)
FILE_PATH=$(echo "$INPUT" | jq -r '.tool_input.file_path // empty')

KEYMAP_DIR="$CLAUDE_PROJECT_DIR/keyboards/splitkb/halcyon/ferris/keymaps/nils"

# Only check when editing files in the keymap directory
if [[ ! "$FILE_PATH" =~ keymaps/nils/ ]]; then
  exit 0
fi

# Verify QK_BOOT exists somewhere in the keymap directory
if grep -rq "QK_BOOT" "$KEYMAP_DIR/" 2>/dev/null; then
  exit 0
fi

echo "SAFETY WARNING: QK_BOOT not found in keymap directory!" >&2
echo "The keyboard will be unrecoverable without QK_BOOT." >&2
echo "Add QK_BOOT to at least one layer before flashing." >&2
exit 2
