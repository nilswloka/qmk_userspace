# Numbers Layer Design

Date: 2026-03-03

## Layout

```
+------+------+------+------+------+   +------+------+------+------+------+
| Esc  |  F1  |  F2  |  F3  |  F4  |   |  -   |  7   |  8   |  9   | Bksp |
+------+------+------+------+------+   +------+------+------+------+------+
| Tab  |  F5  |  F6  |  F7  |  F8  |   |  +   |  4   |  5   |  6   |Enter |
+------+------+------+------+------+   +------+------+------+------+------+
|      |  F9  | F10  |  .   |  =   |   |  0   |  1   |  2   |  3   |  *   |
+------+------+------+------+------+   +------+------+------+------+------+
                     +------+------+   +------+------+
                     |TO(0) | Gui  |   | RAlt |TO(3) |
                     +------+------+   +------+------+
```

## Design decisions

- **Right hand: numpad cluster** -- 7-8-9 / 4-5-6 / 1-2-3 with 0 on the home
  row inner position. Matches the design doc.
- **Arithmetic operators adjacent to numpad** -- minus and plus on the inner
  column, asterisk on the pinky. Self-contained numeric expressions without
  layer switching.
- **ESC/TAB/Enter/Backspace mirror Symbols layer** -- same positions as Layer 1
  for muscle memory transfer.
- **Dot and equals on left hand** -- decimal numbers and assignment available
  without switching.
- **F1-F10 fill remaining left-hand positions** -- F1-F4 top row, F5-F8 home
  row, F9-F10 bottom row (columns 2-3, aligned under F2-F3).
- **No media keys** -- user does not use keyboard media controls.

## Deviations from design doc

The design doc specified media keys (play, prev, next, vol, brightness) on the
left hand. Replaced with F1-F10. The right hand numpad and thumb cluster match
the design doc. The arithmetic operator positions differ slightly (design doc had
`_` and `-` on top row, `+` and `=` on middle; implemented with `-`/`+` on the
inner column for numpad adjacency).
