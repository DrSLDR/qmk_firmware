# A split-layout Workman layout for xd75

Also includes some other fun quirks. It is very much a work in progress and a learning experience.

## Layers

**Primary layer**: 0 (_\_WM_)

The Workman layout with a number row and various function keys on the first, fifteenth, and three middle columns. Escape is in the lower left, LCTRL is between Tab and LShift. Tapping either Shift key twice toggles Caps Lock. Arrow keys in the bottom right are ordered like Vim keys, except up and down are flipped, because I prefer having Up under the middle finger.

Middle three columns are still largely unedited. Work in progress, as said...

**Function layer**: 1 (*_FN*)

Function layer accessible while holding the FN key (bottom, second column). Contains the F-keys, some useful programmer symbols under the right hand, a numpad, PG UP/DN and HOME/END under the arrow keys, a numpad in the center, media keys, and LED controls. Also has RESET under the top middle key (top, eighth column), as well as PAUSE (useful for halting VS compiling) at bottom, ninth column.

## Blinkenlights

LED is supposed to be managed depending on layer and active function.

Base color is currently Orange and will switch to White while FN is held down. While in Reset mode, color switches to Red.

I intend for everything to blink or something while Caps Lock is active, but haven't gotten that to work just yet.