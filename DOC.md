# Notes

## The `main()` function

Explain:
  - `main()` is a special function
  - `return 0` exits the program with a successful exit status code
  - How to compile, run, and check the exit status

## Makefile

Explain:
  - The `vim kilo.c` -> `make` -> `./kilo` workflow
  - `kilo` depends on `kilo.c`
  - You need tabs in Makefile, not spaces
  - `$(CC)`
  - `-Wall -W -pedantic`
  - `-std=c99`

## Canonical mode

Explain:
  - `read()` comes from `<unistd.h>`
  - `STDIN_FILENO` comes from `<unistd.h>`
  - Canonical mode, and why we don't want it
  - What exactly happens when you hit `Ctrl-D` or `Ctrl-C`

## Press `q` to quit?

Explain:
  - Why it doesn't quit immediately, and any input that came after the 'q'
    was read by the shell afterwards

## Turn off echoing

Explain:
  - `tcgetattr()`, `tcsetattr()`, `TCSAFLUSH`, `struct termios`, `ECHO` come
    from `<termios.h>`
  - It's the exact same program, just doesn't show you what you're typing
  - How to `reset` your terminal (make sure to try it with bash, not zsh)
  - Bit flipping with `&=~`
  - Flags are categorized kind of randomly as c_iflag, c_oflag, c_cflag,
    c_lflag

## Disable raw mode at exit

Explain:
  - `atexit()` comes from `<stdlib.h>`
  - structure assignment
  - echoing is turned back on when it exits in bash, also the TCSAFLUSH on
    exit prevents extra input from going to the shell afterwards

## Turn off canonical mode

Explain:
  - `ICANON`, `IEXTEN` come from `<stdlib.h>`
  - `IEXTEN` is basically extended features for canonical mode? (It's turned
    off already on my terminal/shell)
  - Bitwise `|` operator
  - Now `read()` returns as soon as each key is pressed, and the program
    exits as soon as you press 'q'

## Display keypresses

Explain:
  - `printf()` comes from `<stdio.h>`
  - `isprint()` comes from `<ctype.h>`
  - Show that holding `Ctrl` while pressing a key just clears the sixth and
    seventh bits of the pressed key, resulting in `Ctrl-A` being 1, `Ctrl-B`
    being 2, `Ctrl-C` being 3, ..., `Ctrl-Z` being 26

## Turn off `Ctrl-Z` and `Ctrl-C` signals

Explain:
  - `ISIG` comes from `<termios.h>`
  - What `Ctrl-Z` and `Ctrl-C` do and why we don't want them

## Turn off `Ctrl-S` and `Ctrl-Q`

Explain:
  - `IXON` comes from `<termios.h>`
  - What `Ctrl-S` and `Ctrl-Q` are for
  - Now we can `Ctrl` the whole alphabet... *except* `Ctrl-M`! It should
    be read as a 13, but it's read as a 10. We must be translating carriage
    returns to newlines! We'll fix that next.

## Fix `Ctrl-M`

Explain:
  - `ICRNL` comes from `<termios.h>`
  - Now we have the whole `Ctrl` alphabet to work with
  - Notice that the enter key is now read as 13 instead of 10. (Carriage
    return instead of newline). It was also being affected by ICRNL.

## Turn off output processing

Explain:
  - `OPOST` comes from `<termios.h>`
  - It disables all other output processing flags in the `c_oflag` field
  - Generally the only thing this affects is `ONLCR`, which causes newlines
    to be translated into CR-NL's. From now on we'll have to output "\r\n"
    to get a newline with the cursor moved all the way to the left.

## Miscellaneous flags

Explain:
  - `BRKINT`, `INPCK`, `ISTRIP`, `CS8` come from `<termios.h>`
  - These flags are either probably already turned off, or don't have an
    effect on emulated terminals and are there for historical reasons, or I
    am not sure what they do or how to get a visible effect from them. But
    they seem to be generally accepted to be required settings in order for
    it to be considered "raw mode".

## Poll for keyboard input

Explain:
  - `VMIN`, `VTIME` come from `<termios.h>`
  - Why do we want `read()` to timeout after 100ms?

## Error handling

Explain:
  - `perror()` comes from `<stdio.h>`
  - `exit()` comes from `<stdlib.h>`
  - Importance of checking return values
  - How to make it fail (run `cat kilo.c | ./kilo`)

## Clear the screen on exit

Explain:
  - Why this is important

## Sections

Explain:
  - How the sections help shorten the diffs

## Press `Ctrl-Q` to quit

Explain:
  - How `x & 0x1f` clears bits 6 & 7, and why we're doing that
  - #define, and why (k) is in parens

## Refactor keyboard input

Explain:
  - `editorReadKey()` is for low level input handling,
    `editorProcessKeypress()` is for binding keys to actions at a higher
    level, `main()` and the main loop should be kept trim
  - `editorReadKey()` basically blocks for input, so for now we'll only be
    updating the screen when a key is pressed

## Clear the screen

Explain:
  - `write()`, `STDOUT_FILENO` come from `<unistd.h>`
  - `'\x1b'` is the ESC character, we are outputting an escape sequence which
    the terminal interprets as "clear the entire screen"

## Reposition the cursor

Explain:
  - `ESC [ H` is short for `ESC [ 1 ; 1 H`, positioning the cursor at the
    first row and first column (mnemonic: H for Home)

## Tildes

Explain:
  - What the tildes are for
  - Why we don't know the number of rows in the terminal window yet

## Global state

Explain:
  - What kind of global state to expect and why we're putting it all in one
    struct called `E`.

## Window size, the easy way

Explain:
  - `ioctl()`, `winsize`, `TIOCGWINSZ` come from `<sys/ioctl.h>`
  - Passing input arguments by reference
  - May notice that the last row is blank, this will be fixed soon

## Window size, the hard way (part 1)

Explain:
  - The C and B escape sequences move the cursor way to the right and way
    to the bottom, and the output shows that this moved it to the bottom-
    right corner of the screen. Now we will just have to ask for the
    position of the cursor to find out the size of the window.
  - Why we add `1 || ...` to the if condition

## Window size, the hard way (part 2)

Explain:
  - The "6n" sequence asks the terminal to report the cursor position. We
    then `read()` the response and print it out character by character to
    show what the response format looks like. We'll parse the response next.

## Window size, the hard way (part 3)

Explain:
  - We have to read into a buf[] to parse it later with sscanf. We stop
    reading into buf[] when we hit the 'R' character of the response. We
    print out the contents of buf[], except for the first character which
    should be an ESC which would mess things up if we printed it out.

## Window size, the hard way (part 4)

Explain:
  - `sscanf()` comes from `<stdio.h>`

## Window size, the hard way (part 5)

Explain:
  - We're done implementing the fallback branch, so the last step is to
    remove the `1 ||` that allowed us to test that branch.

## The last line

Explain:
  - Why does ending the last line with "\r\n" make that last line invisible?

## Append buffer

Explain:
  - `memcpy()` comes from `<string.h>`.
  - `realloc()`, `free()` come from `<stdlib.h>`.
  - The append buffer is used to prevent flickering.
  - Memory management
  - Literal struct values

## Hide the cursor when repainting

Explain:
  - Escape sequences `[?25l` and `[?25h`
  - Try commenting out the `[?25h` part to see if the cursor is actually
    being hidden

## Welcome message

Explain:
  - `snprintf()` comes from `<stdio.h>`
  - Centering calculations

## Move the cursor

Explain:
  - `[%d;%dH` sequence
  - 1-indexing and 0-indexing
  - What happens when the cursor goes past the screen border
  - Suggest using 'WASD' or the numpad instead

## Arrow keys

Explain:
  - enums
  - Changing `char` to `int`
  - How special keys like arrows are read as escape sequences
  - Why seq is 3 chars instead of 2

## Prevent moving the cursor off screen

Explain:
  - Reminder that cx and cy use 0-indexing

## A line viewer

Explain:
  - `fopen()`, `fclose()`, `FILE` come from `<stdio.h>`
  - `getline()` comes from `<stdio.h>` (but may require defining
  - `size_t`, `ssize_t` come from `<sys/types.h>`
    `_DEFAULT_SOURCE` and/or `_GNU_SOURCE`
  - `typedef struct`
  - We'll store lines without newlines or even NUL terminators at the end,
    storing the length of each line instead
  - Replacing `\x1b[2J` with `\x1b[K`
  - Display welcome message only if new file
  - `argc` and `argv`

## Multiple lines

Explain:
  - How a single `*` turned one row into a dynamic array of rows (and how we
    use `realloc()` to do it)
  - What `filerow` is for

## Vertical scrolling

Explain:
  - What `E.rowoff` means
  - Think of `cx` and `cy` as being the cursor position in the file, not the
    cursor position on the screen
  - `editorScroll()` checks if the cursor is in the visible window, and
    corrects the window position if it's not
  - Another reminder about 0-indexing, off by one errors, etc.

## Horizontal scrolling

Explain:
  - It's all parallel to vertical scrolling code, for now
  - Except we don't limit scrolling to the right

## Limit scrolling to the right

Explain:
  - ternary operator

## Snap cursor to end of line

Explain:

## Moving left at the start of a line

Explain:

## Moving right at the end of a line

Explain:

## Rendering tabs

Explain:
  - Problem: tabs don't erase anything, you may notice when you start up
    ./kilo
  - How tab stops work
  - What `render` is for

## Tabs and the cursor

Explain:
  - What `rx` (render x) is for
  - Why we don't need `ry` (although it could be useful for implementing
    word wrapping...)
  - Arithmetic

## Page Up and Page Down

Explain:
  - Their escape sequences
  - How we implement them in terms of arrow key movements

## Home and End keys

Explain:
  - Their escape sequences (why are there so many different ways of encoding
    them??)

## Status bar

Explain:
  - `strdup()` comes from `<string.h>`
  - `snprintf()` comes from `<stdio.h>`
  - `[7m` inverts colors, `[0m` resets styles
  - How we right-align the rstatus

## Status message

Explain:
  - `time_t`, `time()` come from `<time.h>`
  - `va_list`, `va_start()`, `va_end()` come from `<stdarg.h>`
  - `vsnprintf()` comes from `<stdio.h>`

## Insert ordinary characters

Explain:
  - `memmove()` comes from `<string.h>`

## Prevent inserting special characters

Explain:
  - `\r` is for the enter key
  - `\x1b` is escape, we ignore it as well as Ctrl-L (refresh screen) and
    Ctrl-C
  - BACKSPACE, Ctrl-H, and DEL_KEY are all ways of deleting the last char

## Save to disk

Explain:
  - `open()`, `O_RDWR`, `O_CREAT` come from `<fnctl.h>`
  - `ftruncate()`, `close()` come from `<unistd.h>`
  - `strerror()` comes from `<stdio.h>`
  - `errno` comes from `<errno.h>`
  - Function prototypes
  - Why ftruncate()?
  - Octal 0644 permissions

## Dirty flag

Explain:
  - Why increment?
  - Why does it go in row operations but not in editor operations?

## Quit confirmation

Explain:
  - Static variables in functions
  - Concatenating string literals

## Simple backspace

Explain:

## Backspacing at the start of a line

Explain:
  - Backspacing at the start of the line joins the two lines, and deletes
    the one you were on

## Deleting forward with the delete key

Explain:
  - How moving the cursor to the right solves each case

## Enter key

Explain:
  - Adding `at` argument to `editorAppendRow()`, making it more of an
    `editorInsertRow()`
  - When calling `editorInsertRow()`, it reallocs `E.row`, so any pointers
    to rows become invalid, which is why we set the `row` variable again in
    `editorInsertNewline()`

## Save as...

Explain:
  - Dynamic buffer with `realloc()`

## Basic search

Explain:
  - `strstr()` comes from `<string.h>`
  - We position the window below the entire file so it scrolls up, and the
    result row goes to the top of the screen

## Incremental search

Explain:
  - Function pointers and callbacks

## Restore cursor position when escaping out of search

Explain:

## Search forward and backward

Explain:
  - What `last_match` and `find_next` are for, why they need to be statics
  - How `current` lets us wrap around the file when searching forwards or
    backwards

## Syntax highlighting: digits

Explain:
  - How color works in the terminal

## Syntax highlighting: refactoring

Explain:
  - Why `row->hl` is needed
  - Using `current_color` to minimize formatting sequences

## Syntax highlighting: search results

Explain:

## Syntax highlighting: restore highlighting after search

Explain:

## Syntax highlighting: numbers

Explain:
  - `isspace()` comes from `<ctype.h>`
  - `strchr()` comes from `<string.h>`
  - Purpose of `is_separator()` and `prev_sep`
  - Why we increment `i` and use `continue` in the while loop
  - Complex if condition

## Syntax highlighting: detect filetype

Explain:
  - bitflags

## Syntax highlighting: strings

Explain:

## Syntax highlighting: single-line comments

Explain:

## Syntax highlighting: keywords

Explain:
  - Why we both `break` and `continue` to escape the loops

## Syntax highlighting: non-printable characters

Explain:
  - Why the chars might be negative
  - Why we add '@' to chars 0-26 (hint: there's 26 letters in the alphabet)

## Syntax highlighting: multiline comments

Explain:
  - Why multiline comments are complicated
