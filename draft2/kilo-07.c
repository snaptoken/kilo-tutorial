/* # Press `q` to quit
 *
 * Now that we're in raw mode, let's make the program interactive by having it
 * run indefinitely until the user presses `q`.
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

int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) == 0);
  if (nread == -1) die("read");
  return c;
}

/*** input ***/

void editorProcessKeypress() {
  int c = editorReadKey();

  switch (c) {
    case 'q':
      exit(0);
      break;
  }
}

/*** init ***/

int main() {
  enableRawMode();

  while (1) {
    editorProcessKeypress();
  }

  return 0;
}

/* `editorReadKey()` repeatedly calls `read()` until 1 byte is read into the
 * `c` variable. When there is no keyboard input, `read()` returns `0`, meaning
 * "0 bytes read", after waiting 0.1 seconds and then timing out. So we are
 * calling `read()` 10 times a second until it gives us some keyboard input. We
 * also check to see if `read()` returned `-1`, which indicates an error
 * occurred. (`read()` comes from `<unistd.h>`.)
 *
 * `editorProcessKeypress()` waits for a keypress and handles it. For now, it
 * exits when `q` is pressed, and ignores all other keypresses.
 *
 * In `main()`, we've added an infinite loop that repeatedly handles
 * keypresses.
 */

