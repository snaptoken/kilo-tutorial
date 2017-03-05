/* # Sections
 *
 * Our program is growing quickly. Now is a good time to divide it into logical
 * sections. Right now, only individual functions and structs can be hidden in
 * the diffs shown on these pages. Dividing the code into broad sections will
 * allow me to hide entire sections of code, which will make the diffs much
 * more manageable in the future, and will help you find your way around the
 * code.
 *
 * We will start with sections for includes, structs, terminal functions, and
 * program initialization. Sections are indicated by comments with three
 * asterisks at each end.
 */

/*** includes ***/

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*** structs ***/

struct editorConfig {
  struct termios orig_termios;
};

static struct editorConfig E;

/*** terminal ***/

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  struct termios raw;

  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/

int main() {
  enableRawMode();

  return 0;
}

