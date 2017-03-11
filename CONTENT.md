# Build Your Own Text Editor

## Setup

### The `main()` function

You start by writing a simple `main()` function.

{{main}}

### Makefile

{{makefile}}

## Entering raw mode

### Canonical mode

{{canonical}}

### Press `q` to quit?

{{press-q}}

### Turn off echoing

{{echo}}

### Disable raw mode at exit

{{atexit}}

### Turn off canonical mode

{{canonical-off}}

### Display keypresses

{{keypresses}}

### Turn off `Ctrl-Z` and `Ctrl-C` signals

{{isig}}

### Turn off `Ctrl-S` and `Ctrl-Q`

{{ixon}}

### Fix `Ctrl-M`

{{ctrl-m}}

### Turn off output processing

{{opost}}

### Miscellaneous flags

{{misc-flags}}

### Poll for keyboard input

{{vmin-vtime}}

### Error handling

{{error-handling}}

### Clear the screen on exit

{{clear-screen-atexit}}

### Sections

{{sections}}

## Raw input and output

### Press `Ctrl-Q` to quit

{{ctrl-q}}

### Refactor keyboard input

{{refactor-input}}

### Clear the screen

{{clear-screen}}

### Reposition the cursor

{{cursor-home}}

### Tildes

{{tildes}}

### Global state

{{global-state}}

### Window size, the easy way

{{window-size}}

### Window size, the hard way (part 1)

{{bottom-right}}

### Window size, the hard way (part 2)

{{cursor-query}}

### Window size, the hard way (part 3)

{{response-buffer}}

### Window size, the hard way (part 4)

{{parse-response}}

### Window size, the hard way (part 5)

{{cleanup-window-size}}

### The last line

{{last-line}}

### Append buffer

{{abuf}}

### Hide the cursor when repainting

{{hide-cursor}}

### Welcome message

{{welcome}}

### Move the cursor

{{move-cursor}}

### Arrow keys

{{arrow-keys}}

### Prevent moving the cursor off screen

## A text viewer

{{off-screen}}

### A line viewer

{{single-line}}

### Multiple lines

{{multiple-lines}}

### Vertical scrolling

{{vertical-scroll}}

### Horizontal scrolling

{{horizontal-scroll}}

### Limit scrolling to the right

{{scroll-limits}}

### Snap cursor to end of line

{{snap-cursor}}

### Moving left at the start of a line

{{moving-left}}

### Moving right at the end of a line

{{moving-right}}

### Rendering tabs

{{render-tabs}}

### Tabs and the cursor

{{rx}}

### Page Up and Page Down

{{page-up-down}}

### Home and End keys

{{home-end-keys}}

### Status bar

{{status-bar}}

### Status message

{{status-message}}

## A text editor

### Insert ordinary characters

{{insert-chars}}

### Prevent inserting special characters

{{block-special-chars}}

### Save to disk

{{save}}

### Dirty flag

{{dirty-flag}}

### Quit confirmation

{{quit-confirmation}}

### Simple backspace

{{simple-backspace}}

### Backspacing at the start of a line

{{backspacing-lines}}

### Deleting forward with the delete key

{{delete-forward}}

### Enter key

{{enter-key}}

### Save as...

{{save-as}}

## Search

### Basic search

{{basic-search}}

### Incremental search

{{incremental-search}}

### Restore cursor position when escaping out of search

{{restore-cursor}}

### Search forward and backward

{{search-arrows}}

## Syntax highlighting

### Colorful digits

{{syntax-digits}}

### Refactor syntax highlighting

{{syntax-refactoring}}

### Colorful search results

{{search-highlighting}}

### Restore syntax highlighting after search

{{restore-search-highlight}}

### Colorful numbers

{{syntax-numbers}}

### Detect filetype

{{filetype}}

### Colorful strings

{{syntax-strings}}

### Colorful single-line comments

{{syntax-comments}}

### Colorful keywords

{{syntax-keywords}}

### Not so colorful non-printable characters

{{nonprintables}}

### Colorful multiline comments

{{multiline-comments}}

