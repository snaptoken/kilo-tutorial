# Search

Let's use `editorPrompt()` to implement a minimal search feature. When the user
types a search query and presses <kbd>Enter</kbd>, we'll loop through all the
rows of the file, and if a row contains their query string, we'll move the
cursor to the match.

{{basic-search}}

`strstr()` comes from `<string.h>`.

If they pressed <kbd>Escape</kbd> to cancel the input prompt, then
`editorPrompt()` returns `NULL` and we abort the search.

Otherwise, we loop through all the rows of the file. We use `strstr()` to check
if `query` is a substring of the current row. It returns `NULL` if there is no
match, otherwise it returns a pointer to the matching substring. To convert
that into an index that we can set `E.cx` to, we subtract the `row->render`
pointer from the `match` pointer, since `match` is a pointer into the
`row->render` string. Lastly, we set `E.rowoff` so that we are scrolled to the
very bottom of the file, which will cause `editorScroll()` to scroll upwards at
the next screen refresh so that the matching line will be at the very top of
the screen. This way, the user doesn't have to look all over their screen to
find where their cursor jumped to, and where the matching line is.

There's one problem here. Did you notice what we just did wrong? We assigned a
`render` index to `E.cx`, but `E.cx` is an index into `chars`. If there are
tabs to the left of the match, the cursor is going to be in the wrong position.
We need to convert the `render` index into a `chars` index before assigning it
to `E.cx`. Let's create an `editorRowRxToCx()` function, which is the opposite
of the `editorRowCxToRx()` function we wrote in
[chapter 4](04.aTextViewer.html#tabs-and-the-cursor), but contains a lot of the
same code.

{{rx-to-cx}}

To convert an `rx` into a `cx`, we do pretty much the same thing when
converting the other way: loop through the `chars` string, calculating the
current `rx` value (`cur_rx`) as we go. But instead of stopping when we hit a
particular `cx` value and returning `cur_rx`, we want to stop when `cur_rx`
hits the given `rx` value and return `cx`.

The `return` statement at the very end is just in case the caller provided an
`rx` that's out of range, which shouldn't happen. The `return` statement inside
the `for` loop should handle all `rx` values that are valid indexes into
`render`.

Now let's call `editorRowRxToCx()` to convert the matched index to a `chars`
index and assign that to `E.cx`.

{{use-rx-to-cx}}

Finally, let's map <kbd>Ctrl-F</kbd> to the `editorFind()` function, and add it
to the help message we set in `main()`.

{{ctrl-f}}

## Incremental search

Now, let's make our search feature fancy. We want to support incremental
search, meaning the file is searched after each keypress when the user is
typing in their search query.

To implement this, we're going to get `editorPrompt()` to take a callback
function as an argument. We'll have it call this function after each keypress,
passing the current search query inputted by the user and the last key they
pressed.

{{prompt-callback}}

The `if` statements allow the caller to pass `NULL` for the callback, in case
they don't want to use a callback. This is the case when we prompt the user
for a filename, so let's pass `NULL` to `editorPrompt()` when we do that. We'll
also pass `NULL` to `editorPrompt()` in `editorFind()` for now, to get the code
to compile.

{{null-callback}}

Now let's move the actual searching code from `editorFind()` into a function
called `editorFindCallback()`. Obviously this will be our callback function for
`editorPrompt()`.

{{incremental-search}}

In the callback, we check if the user pressed <kbd>Enter</kbd> or
<kbd>Escape</kbd>, in which case they are leaving search mode so we `return`
immediately instead of doing another search. Otherwise, after any other
keypress, we do another search for the current `query` string.

That's all there is to it. We now have incremental search.

## Restore cursor position when cancelling search

When the user presses <kbd>Escape</kbd> to cancel a search, we want the cursor
to go back to where it was when they started the search. To do that, we'll have
to save their cursor position and scroll position, and restore those values
after the search is cancelled.

{{restore-cursor}}

If `query` is `NULL`, that means they pressed <kbd>Escape</kbd>, so in that
case we restore the values we saved.

## Search forward and backward

The last feature we'd like to add is to allow the user to advance to the next
or previous match in the file using the arrow keys. The <kbd>&uarr;</kbd> and
<kbd>&larr;</kbd> keys will go to the previous match, and the <kbd>&darr;</kbd>
and <kbd>&rarr;</kbd> keys will go to the next match.

We'll implement this feature using two static variables in our callback.
`last_match` will contain the index of the row that the last match was on, or
`-1` if there was no last match. And `direction` will store the direction of
the search: `1` for searching forward, and `-1` for searching backward.

{{callback-statics}}

As you can see, we always reset `last_match` to `-1` unless an arrow key was
pressed. So we'll only advance to the next or previous match when an arrow key
is pressed. You can also see that we always set `direction` to `1` unless the
<kbd>&larr;</kbd> or <kbd>&uarr;</kbd> key was pressed. So we always search in
the forward direction unless the user specifically asks to search backwards
from the last match.

If `key` is `'\r'` (<kbd>Enter</kbd>) or `'\x1b'` (<kbd>Escape</kbd>), that
means we're about to leave search mode. So we reset `last_match` and
`direction` to their initial values to get ready for the next search operation.

Now that we have those variables all set up, let's put them to use.

{{search-arrows}}

`current` is the index of the current row we are searching. If there was a last
match, it starts on the line after (or before, if we're searching backwards).
If there wasn't a last match, it starts at the top of the file and searches in
the forward direction to find the first match.

The `if ... else if` causes `current` to go from the end of the file back to
the beginning of the file, or vice versa, to allow a search to "wrap around"
the end of a file and continue from the top (or bottom).

When we find a match, we set `last_match` to `current`, so that if the user
presses the arrow keys, we'll start the next search from that point.

Finally, let's not forget to update the prompt text to let the user know they
can use the arrow keys.

{{search-arrows-help}}

In the [next chapter](07.syntaxHighlighting.html), we'll implement syntax
highlighting and filetype detection, to complete our text editor.

