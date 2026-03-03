# Nav Layer Design

Date: 2026-03-03

## Layout

```
+------+------+------+------+------+   +------+------+------+------+------+
|      | ScLf | MsUp | ScRt |      |   | Back | PTab | NTab | Fwd  | Bksp |
+------+------+------+------+------+   +------+------+------+------+------+
|      | MsLf | MsDn | MsRt | Del  |   | Left | Down |  Up  |Right |Enter |
+------+------+------+------+------+   +------+------+------+------+------+
|      | Btn2 | ScUp | ScDn |      |   | Btn1 | Home | PgUp | PgDn | BOOT |
+------+------+------+------+------+   +------+------+------+------+------+
                     +------+------+   +------+------+
                     |TO(0) |      |   | RAlt |TO(1) |
                     +------+------+   +------+------+
```

## Design decisions

- **Left hand: full mouse keys** -- movement (up/down/left/right), horizontal
  scroll (left/right), vertical scroll (up/down), and both mouse buttons.
  Serves as fallback to the Cirque trackpad.
- **Right hand home row: arrow keys** -- Left/Down/Up/Right in the standard
  hjkl-equivalent positions. These emit arrow keycodes (not vim motions),
  useful in terminals, minibuffers, and non-evil contexts.
- **Right hand top row: browser navigation** -- Back/Forward and Prev Tab/Next
  Tab. Common workflow keys for web browsing and documentation lookup.
- **Right hand bottom row: Home/PgUp/PgDn** -- text navigation for terminals,
  file managers, and long documents. Mouse Button 1 on inner position for
  trackpad click coordination.
- **ESC/TAB omitted** -- unlike Symbols and Numbers layers, Nav doesn't need
  these since you typically navigate then return to Base.
- **QK_BOOT stays bottom-right** -- safe corner position, only reachable via
  layer cycling (Base -> Symbols -> Numbers -> Nav).
- **Backspace/Enter/Delete available** -- mirrors their positions from other
  layers for muscle memory.

## Deviations from design doc

- Added Home/PgUp/PgDn on the bottom row (design doc had empty positions).
- Horizontal scroll (ScLf/ScRt) on top row instead of the design doc's
  MSWHL/MSWHR naming (same function, clearer labels).
- Left inner thumb left empty (design doc had transparent/GUI). With TO()
  switching there's no lower layer to inherit from.
