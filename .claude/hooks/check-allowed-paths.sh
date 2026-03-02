#!/bin/bash
# Safety hook: block edits to files outside allowed directories
# Prevents accidental modification of core QMK or halcyon_modules files.

INPUT=$(cat)
FILE_PATH=$(echo "$INPUT" | jq -r '.tool_input.file_path // empty')

if [[ -z "$FILE_PATH" ]]; then
  exit 0
fi

PROJECT_DIR="$CLAUDE_PROJECT_DIR"

# Allowed paths (relative to project root)
# - keyboards/splitkb/halcyon/ferris/keymaps/nils/ — the custom keymap
# - .claude/ — Claude Code settings and hooks
# - CLAUDE.md — project guidance
# - qmk.json — build targets
# - .github/workflows/ — CI configuration
# - .gitignore — git configuration
# - docs/plans/ — design documents

case "$FILE_PATH" in
  "$PROJECT_DIR"/keyboards/splitkb/halcyon/ferris/keymaps/nils/*)  exit 0 ;;
  "$PROJECT_DIR"/.claude/*)                                         exit 0 ;;
  "$PROJECT_DIR"/CLAUDE.md)                                         exit 0 ;;
  "$PROJECT_DIR"/qmk.json)                                         exit 0 ;;
  "$PROJECT_DIR"/.github/workflows/*)                               exit 0 ;;
  "$PROJECT_DIR"/.gitignore)                                        exit 0 ;;
  "$PROJECT_DIR"/docs/plans/*)                                      exit 0 ;;
esac

# Block everything else
RELATIVE_PATH="${FILE_PATH#"$PROJECT_DIR"/}"
echo "BLOCKED: Editing '$RELATIVE_PATH' is not allowed." >&2
echo "Allowed paths: keymaps/nils/, .claude/, CLAUDE.md, qmk.json, .github/workflows/, .gitignore, docs/plans/" >&2
exit 2
