# Changelog

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

