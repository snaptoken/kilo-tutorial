# Changelog

## 1.0.0beta11 (October 3, 2017)

* Fix number of bits in binary example. (Thanks @osiyuk)

## 1.0.0beta10 (July 12, 2017)

* Make bit numbering clearer in the explanation of step 20. (Thanks @fyce)

## 1.0.0beta9 (May 22, 2017)

* Make explanation of step 30 clearer by reordering paragraphs. (Thanks @mtn)
* Update [How to contribute](http://viewsourcecode.org/snaptoken/kilo/08.appendices.html#how-to-contribute)
  section. (Thanks @mtn)

## 1.0.0beta8 (April 28, 2017)

* `strchr()` was described incorrectly in
  [chapter 7](http://viewsourcecode.org/snaptoken/kilo/07.syntaxHighlighting.html#colorful-numbers).
  (Thanks Rudi)

## 1.0.0beta7 (April 20, 2017)

* In `editorSelectSyntaxHighlight()`, change the logic for filename pattern
  matching, so that filenames like `kilo.c.c` will work. (Thanks Ivandro)

## 1.0.0beta6 (April 10, 2017)

* Near the end of chapter 6, add `if (last_match == -1) direction = 1;` to
  `editorFindCallback()`, to fix a segfault. (Thanks @agacek)
* In the [Scrolling with Page Up and Page Down](http://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html#scrolling-with-page-up-and-page-down)
  section in chapter 4, add `if (E.cy > E.numrows) E.cy = E.numrows;` to
  `editorProcessKeypress()`, so the cursor stays within the file. (Thanks
  @agacek)
* At the beginning of chapter 3, remove the keypress printing code during the
  `refactor-input` step instead of the `ctrl-q` step, and make it clear that
  you're supposed to remove that code. (Thanks @wonthegame)

## 1.0.0beta5 (April 9, 2017)

* Remove *all* newlines and carriage returns from the end of each line read by
  `editorOpen()`, by changing `if (linelen > 0 && ...` to
  `while (linelen > 0 && ...`. This allows text files with DOS line endings
  (`\r\n`) to be opened properly.

## 1.0.0beta4 (April 8, 2017)

* In the [Multiple lines](http://viewsourcecode.org/snaptoken/kilo/04.aTextViewer.html#multiple-lines)
  section, make it clearer that the program doesn't compile for those steps.
  (Thanks @mapleray and @agentultra)

## 1.0.0beta3 (April 7, 2017)

* In `editorPrompt()`, change `!iscntrl(c)` to `!iscntrl(c) && c < 128`, so
  that we don't try to append special keys like the arrow keys to the prompt
  input. (Thanks @fmdkdd)

## 1.0.0beta2 (April 6, 2017)

* Replace all instances of `isprint()` with `!iscntrl()`, so that extended
  ASCII characters can be inserted and displayed in the editor.
* Include font files in offline version of tutorial.

## 1.0.0beta1 (April 6, 2017)

* Add changelog.
* Fix landing page typo. (Thanks @mtr)

## 1.0.0beta0 (April 4, 2017)

* First public release.

