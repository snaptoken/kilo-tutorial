/* # Turn off `Ctrl-S` and `Ctrl-Q`
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  struct termios raw;

  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  raw = orig_termios;
  raw.c_iflag &= ~(IXON);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (isprint(c)) {
      printf("%d ('%c')\n", c, c);
    } else {
      printf("%d\n", c);
    }
  }

  return 0;
}

/* Explain:
 *   - `IXON` comes from `<termios.h>`
 *   - What `Ctrl-S` and `Ctrl-Q` are for
 *   - Now we can `Ctrl` the whole alphabet... *except* `Ctrl-M`! It should
 *     be read as a 13, but it's read as a 10. We must be translating carriage
 *     returns to newlines! We'll fix that next.
 */

