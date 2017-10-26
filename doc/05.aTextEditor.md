# A text editor

## Insert ordinary characters

Let's begin by writing a function that inserts a single character into an
`erow`, at a given position.

{{row-insert-char}}

`memmove()` comes from `<string.h>`. It is like `memcpy()`, but is safe to use
when the source and destination arrays overlap.

First we validate `at`, which is the index we want to insert the character
into. Notice that `at` is allowed to go one character past the end of the
string, in which case the character should be inserted at the end of the
string.

Then we allocate one more byte for the `chars` of the `erow` (we add `2`
because we also have to make room for the null byte), and use `memmove()` to
make room for the new character. We increment the `size` of the `chars` array,
and then actually assign the character to its position in the array. Finally,
we call `editorUpdateRow()` so that the `render` and `rsize` fields get updated
with the new row content.

Now we'll create a new section called `/*** editor operations ***/`. This
section will contain functions that we'll call from `editorProcessKeypress()`
when we're mapping keypresses to various text editing operations. We'll add a
function to this section called `editorInsertChar()` which will take a
character and use `editorRowInsertChar()` to insert that character into the
position that the cursor is at.

{{editor-insert-char}}

If `E.cy == E.numrows`, then the cursor is on the tilde line after the end of
the file, so we need to append a new row to the file before inserting a
character there. After inserting a character, we move the cursor forward so
that the next character the user inserts will go after the character just
inserted.

Notice that `editorInsertChar()` doesn't have to worry about the details of
modifying an `erow`, and `editorRowInsertChar()` doesn't have to worry about
where the cursor is. That is the difference between functions in the
`/*** editor operations ***/` section and functions in the
`/*** row operations ***/` section.

Let's call `editorInsertChar()` in the `default:` case of the `switch`
statement in `editorProcessKeypress()`. This will allow any keypress that isn't
mapped to another editor function to be inserted directly into the text being
edited.

{{key-insert-char}}

We've now officially upgraded our text viewer to a text editor.

## Prevent inserting special characters

Currently, if you press keys like <kbd>Backspace</kbd> or <kbd>Enter</kbd>,
those characters will be inserted directly into the text, which we certainly
don't want. Let's handle a bunch of these special keys in
`editorProcessKeypress()`, so that they don't fall through to the `default`
case of calling `editorInsertChar()`.

{{block-special-chars}}

<kbd>Backspace</kbd> doesn't have a human-readable backslash-escape
representation in C (like `\n`, `\r`, and so on), so we make it part of the
`editorKey` enum and assign it its ASCII value of `127`.

In `editorProcessKeypress()`, the first new key we add to the `switch`
statement is `'\r'`, which is the <kbd>Enter</kbd> key. For now we want to
ignore it, but obviously we'll be making it do something later, so we mark it
with a `TODO` comment.

We handle <kbd>Backspace</kbd> and <kbd>Delete</kbd> in a similar way, marking
them with a `TODO`. We also handle the <kbd>Ctrl-H</kbd> key combination, which
sends the control code `8`, which is originally what the <kbd>Backspace</kbd>
character would send back in the day. If you look at the
[ASCII table](http://www.asciitable.com/), you'll see that ASCII code `8` is
named `BS` for "backspace", and ASCII code `127` is named `DEL` for "delete".
But for whatever reason, in modern computers the <kbd>Backspace</kbd> key is
mapped to `127` and the <kbd>Delete</kbd> key is mapped to the escape sequence
`<esc>[3~`, as we saw at the end of
[chapter 3](03.rawInputAndOutput.html#the-delete-key).

Lastly, we handle <kbd>Ctrl-L</kbd> and <kbd>Escape</kbd> by not doing anything
when those keys are pressed. <kbd>Ctrl-L</kbd> is traditionally used to refresh
the screen in terminal programs. In our text editor, the screen refreshes after
*any* keypress, so we don't have to do anything else to implement that feature.
We ignore the <kbd>Escape</kbd> key because there are many key escape sequences
that we aren't handling (such as the <kbd>F1</kbd>&ndash;<kbd>F12</kbd> keys),
and the way we wrote `editorReadKey()`, pressing those keys will be equivalent
to pressing the <kbd>Escape</kbd> key. We don't want the user to unwittingly
insert the escape character `27` into their text, so we ignore those
keypresses.

## Save to disk

Now that we've finally made text editable, let's implement saving to disk.
First we'll write a function that converts our array of `erow` structs into a
single string that is ready to be written out to a file.

{{rows-to-string}}

First we add up the lengths of each row of text, adding `1` to each one for the
newline character we'll add to the end of each line. We save the total length
into `buflen`, to tell the caller how long the string is.

Then, after allocating the required memory, we loop through the rows, and
`memcpy()` the contents of each row to the end of the buffer, appending a
newline character after each row.

We return `buf`, expecting the caller to `free()` the memory.

Now we'll implement the `editorSave()` function, which will actually write the
string returned by `editorRowsToString()` to disk.

{{save}}

`open()`, `O_RDWR`, and `O_CREAT` come from `<fcntl.h>`. `ftruncate()` and
`close()` come from `<unistd.h>`.

If it's a new file, then `E.filename` will be `NULL`, and we won't know where
to save the file, so we just `return` without doing anything for now. Later,
we'll figure out how to prompt the user for a filename.

Otherwise, we call `editorRowsToString()`, and `write()` the string to the path
in `E.filename`. We tell `open()` we want to create a new file if it
doesn't already exist (`O_CREAT`), and we want to open it for reading and
writing (`O_RDWR`). Because we used the `O_CREAT` flag, we have to pass an
extra argument containing the mode (the permissions) the new file should have.
`0644` is the standard permissions you usually want for text files. It gives
the owner of the file permission to read and write the file, and everyone else
only gets permission to read the file.

`ftruncate()` sets the file's size to the specified length. If the file is
larger than that, it will cut off any data at the end of the file to make it
that length. If the file is shorter, it will add `0` bytes at the end to make
it that length.

The normal way to overwrite a file is to pass the `O_TRUNC` flag to `open()`,
which truncates the file completely, making it an empty file, before writing
the new data into it. By truncating the file ourselves to the same length as
the data we are planning to write into it, we are making the whole overwriting
operation a little bit safer in case the `ftruncate()` call succeeds but the
`write()` call fails. In that case, the file would still contain most of the
data it had before. But if the file was truncated completely by the `open()`
call and then the `write()` failed, you'd end up with all of your data lost.

More advanced editors will write to a new, temporary file, and then rename that
file to the actual file the user wants to overwrite, and they'll carefully
check for errors through the whole process.

Anyways, all we have to do now is map a key to `editorSave()`, so let's do it!
We'll use <kbd>Ctrl-S</kbd>.

{{ctrl-s}}

You should be able to open a file in the editor, insert some characters, press
<kbd>Ctrl-S</kbd>, and reopen the file to confirm that the changes you made
were saved.

Let's add error handling to `editorSave()`.

{{save-errors}}

`open()` and `ftruncate()` both return `-1` on error. We expect `write()` to
return the number of bytes we told it to write. Whether or not an error
occurred, we ensure that the file is closed and the memory that `buf` points to
is freed.

Let's use `editorSetStatusMessage()` to notify the user whether the save
succeeded or not. While we're at it, we'll add the <kbd>Ctrl-S</kbd> key
binding to the help message that's set in `main()`.

{{save-status-message}}

`strerror()` comes from `<string.h>`.

`strerror()` is like `perror()` (which we use in `die()`), but it takes the
`errno` value as an argument and returns the human-readable string for that
error code, so that we can make the error a part of the status message we
display to the user.

The above code doesn't actually compile, because we are trying to call
`editorSetStatusMessage()` before it is defined in the file. You can't do that
in C, because C was made to be a language that can be compiled in a [single
pass](https://en.wikipedia.org/wiki/One-pass_compiler), meaning it should be
possible to compile each part of a program without knowing what comes later in
the program.

When we call a function in C, the compiler needs to know the arguments and
return value of that function. We can tell the compiler this information about
`editorSetStatusMessage()` by declaring a function prototype for it near the
top of the file. This allows us to call the function before it is defined.
We'll add a new `/*** prototypes ***/` section and put the declaration under
it.

{{prototypes}}

## Dirty flag

We'd like to keep track of whether the text loaded in our editor differs from
what's in the file. Then we can warn the user that they might lose unsaved
changes when they try to quit.

We call a text buffer "dirty" if it has been modified since opening or saving
the file. Let's add a `dirty` variable to the global editor state, and
initialize it to `0`.

{{dirty}}

Let's show the state of `E.dirty` in the status bar, by displaying `(modified)`
after the filename if the file has been modified.

{{show-dirty}}

Now let's increment `E.dirty` in each row operation that makes a change to the
text.

{{increment-dirty}}

We could have used `E.dirty = 1` instead of `E.dirty++`, but by incrementing it
we can have a sense of "how dirty" the file is, which could be useful. (We'll
just be treating `E.dirty` as a boolean value in this tutorial, so it doesn't
really matter.)

If you open a file at this point, you'll see that `(modified)` appears right
away, before you make any changes. That's because `editorOpen()` calls
`editorAppendRow()`, which increments `E.dirty`. To fix that, let's reset
`E.dirty` to `0` at the end of `editorOpen()`, and also in `editorSave()`.

{{reset-dirty}}

Now you should see `(modified)` appear in the status bar when you first insert
a character, and you should see it disappear when you save the file to disk.

## Quit confirmation

Now we're ready to warn the user about unsaved changes when they try to quit.
If `E.dirty` is set, we will display a warning in the status bar, and require
the user to press <kbd>Ctrl-Q</kbd> three more times in order to quit without
saving.

{{quit-confirmation}}

We use a static variable in `editorProcessKeypress()` to keep track of how many
more times the user must press <kbd>Ctrl-Q</kbd> to quit. Each time they press
<kbd>Ctrl-Q</kbd> with unsaved changes, we set the status message and decrement
`quit_times`. When `quit_times` gets to `0`, we finally allow the program to
exit. When they press any key other than <kbd>Ctrl-Q</kbd>, then `quit_times`
gets reset back to `3` at the end of the `editorProcessKeypress()` function.

## Simple backspacing

Let's implement backspacing next. First we'll create an `editorRowDelChar()`
function, which deletes a character in an `erow`.

{{row-del-char}}

As you can see, it's very similar to `editorRowInsertChar()`, except we don't
have any memory management to do. We just use `memmove()` to overwrite the
deleted character with the characters that come after it (notice that the null
byte at the end gets included in the move). Then we decrement the row's `size`,
call `editorUpdateRow()`, and increment `E.dirty`.

Now let's implement `editorDelChar()`, which uses `editorRowDelChar()` to
delete the character that is to the left of the cursor.

{{editor-del-char}}

If the cursor's past the end of the file, then there is nothing to delete, and
we `return` immediately. Otherwise, we get the `erow` the cursor is on, and if
there is a character to the left of the cursor, we delete it and move the
cursor one to the left.

Let's map the <kbd>Backspace</kbd>, <kbd>Ctrl-H</kbd>, and <kbd>Delete</kbd>
keys to `editorDelChar()`.

{{key-del-char}}

It so happens that in our editor, pressing the <kbd>&rarr;</kbd> key and then
<kbd>Backspace</kbd> is equivalent to what you would expect from pressing the
<kbd>Delete</kbd> key in a text editor: it deletes the character to the right
of the cursor. So that is how we implement the <kbd>Delete</kbd> key above.

## Backspacing at the start of a line

Currently, `editorDelChar()` doesn't do anything when the cursor is at the
beginning of a line. When the user backspaces at the beginning of a line, we
want to append the contents of that line to the previous line, and then delete
the current line. This effectively backspaces the implicit `\n` character in
between the two lines to join them into one line.

So we need two new row operations: one for appending a string to a row, and one
for deleting a row. Let's start by implementing `editorDelRow()`, which will
also require an `editorFreeRow()` function for freeing the memory owned by the
`erow` we are deleting.

{{del-row}}

`editorDelRow()` looks a lot like `editorRowDelChar()`, because in both cases
we are deleting a single element from an array of elements by its index.

First we validate the `at` index. Then we free the memory owned by the row
using `editorFreeRow()`. We then use `memmove()` to overwrite the deleted row
struct with the rest of the rows that come after it, and decrement `numrows`.
Finally, we increment `E.dirty`.

Now let's implement `editorRowAppendString()`, which appends a string to the
end of a row.

{{row-append-string}}

The row's new size is `row->size + len + 1` (including the null byte), so first
we allocate that much memory for `row->chars`. Then we simply `memcpy()` the
given string to the end of the contents of `row->chars`. We update `row->size`,
call `editorUpdateRow()` as usual, and increment `E.dirty` as usual.

Now we're ready to get `editorDelChar()` to handle the case where the cursor is
at the beginning of a line.

{{del-char-row}}

If the cursor is at the beginning of the _first_ line, then there's nothing to
do, so we `return` immediately. Otherwise, if we find that `E.cx == 0`, we call
`editorRowAppendString()` and then `editorDelRow()` as we planned. `row` points
to the row we are deleting, so we append `row->chars` to the previous row, and
then delete the row that `E.cy` is on. We set `E.cx` to the end of the contents
of the previous row before appending to that row. That way, the cursor will end
up at the point where the two lines joined.

Notice that pressing the <kbd>Delete</kbd> key at the end of a line works as
the user would expect, joining the current line with the next line. This is
because moving the cursor to the right at the end of a line moves it to the
beginning of the next line. So making the <kbd>Delete</kbd> key an alias for
the <kbd>&rarr;</kbd> key followed by the <kbd>Backspace</kbd> key still works.

## The <kbd>Enter</kbd> key

The last editor operation we have to implement is the <kbd>Enter</kbd> key. The
<kbd>Enter</kbd> key allows the user to insert new lines into the text, or
split a line into two lines. The first thing we need to do is rename the
`editorAppendRow(...)` function to `editorInsertRow(int at, ...)`. It will now
be able to insert a row at the index specified by the new `at` argument.

{{append-to-insert}}

Much like `editorRowInsertChar()`, we first validate `at`, then allocate memory
for one more `erow`, and use `memmove()` to make room at the specified index
for the new row.

We also delete the old `int at = ...` line, since `at` is now being passed in
as an argument.

We now have to replace all calls to `editorAppendRow(...)` with calls to
`editorInsertRow(E.numrows, ...)`.

{{use-insert-row}}

Now that we have `editorInsertRow()`, we're ready to implement
`editorInsertNewline()`, which handles an <kbd>Enter</kbd> keypress.

{{insert-newline}}

If we're at the beginning of a line, all we have to do is insert a new blank
row before the line we're on.

Otherwise, we have to split the line we're on into two rows. First we call
`editorInsertRow()` and pass it the characters on the current row that are to
the right of the cursor. That creates a new row after the current one, with the
correct contents. Then we reassign the `row` pointer, because
`editorInsertRow()` calls `realloc()`, which might move memory around on us and
invalidate the pointer (yikes). Then we truncate the current row's contents by
setting its size to the position of the cursor, and we call
`editorUpdateRow()` on the truncated row. (`editorInsertRow()` already calls
`editorUpdateRow()` for the new row.)

In both cases, we increment `E.cy`, and set `E.cx` to `0` to move the cursor to
the beginning of the row.

Finally, let's actually map the <kbd>Enter</kbd> key to the
`editorInsertNewline()` operation.

{{enter-key}}

That concludes all of the text editing operations we are going to implement. If
you wish, and if you are brave enough, you may now start using the editor to
modify its own code for the rest of the tutorial. If you do, I suggest making
regular backups of your work (using `git` or similar) in case you run into bugs
in the editor.

## Save as...

Currently, when the user runs `./kilo` with no arguments, they get a blank file
to edit but have no way of saving. We need a way of prompting the user to input
a filename when saving a new file. Let's make an `editorPrompt()` function that
displays a prompt in the status bar, and lets the user input a line of text
after the prompt.

{{prompt}}

The user's input is stored in `buf`, which is a dynamically allocated string
that we initalize to the empty string. We then enter an infinite loop that
repeatedly sets the status message, refreshes the screen, and waits for a
keypress to handle. The `prompt` is expected to be a format string containing a
`%s`, which is where the user's input will be displayed.

When the user presses <kbd>Enter</kbd>, and their input is not empty, the
status message is cleared and their input is returned. Otherwise, when they
input a printable character, we append it to `buf`. If `buflen` has reached the
maximum capacity we allocated (stored in `bufsize`), then we double `bufsize`
and allocate that amount of memory before appending to `buf`. We also make sure
that `buf` ends with a `\0` character, because both `editorSetStatusMessage()`
and the caller of `editorPrompt()` will use it to know where the string ends.

Notice that we have to make sure the input key isn't one of the special keys in
the `editorKey` enum, which have high integer values. To do that, we test
whether the input key is in the range of a `char` by making sure it is less
than `128`.

Now let's prompt the user for a filename in `editorSave()`, when `E.filename`
is `NULL`.

{{save-as}}

Great, we now have basic "Save as..." functionality. Next, let's allow the user 
to press <kbd>Escape</kbd> to cancel the input prompt.

{{prompt-escape}}

When an input prompt is cancelled, we `free()` the `buf` ourselves and return
`NULL`. So let's handle a return value of `NULL` in `editorSave()` by aborting
the save operation and displaying a "Save aborted" message to the user.

{{abort-save}}

(Note: If you're using **Bash on Windows**, you will have to press
<kbd>Escape</kbd> 3 times to get one <kbd>Escape</kbd> keypress to register in
our program, because the `read()` calls in `editorReadKey()` that look for an
escape sequence never time out.)

Now let's allow the user to press <kbd>Backspace</kbd> (or <kbd>Ctrl-H</kbd>,
or <kbd>Delete</kbd>) in the input prompt.

{{prompt-backspace}}

In the [next chapter](06.search.html), we'll make use of `editorPrompt()` to
implement an incremental search feature in our editor.

