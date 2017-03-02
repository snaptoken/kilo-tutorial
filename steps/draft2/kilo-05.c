/* # Global state
 *
 * Our text editor is going to need to keep track of a lot of variables, such
 * as cursor position, scroll position, the text itself, and so on. Many
 * functions need access to these variables. Instead of passing around a struct
 * to all these functions, we're going to have a single global struct which
 * contains all of these state variables. We will create it now, and relocate
 * the `orig_termios` global variable into the struct.
 */

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct editorConfig {
  struct termios orig_termios;
};

static struct editorConfig E;

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

int main() {
  enableRawMode();

  return 0;
}

/* The global state variable is named `E`. We will use it all over our program,
 * so it's good that it is both easy to type and easy to spot.
 */

