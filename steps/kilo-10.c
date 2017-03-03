/* # Fix `Ctrl-M`
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
  raw.c_iflag &= ~(ICRNL | IXON);
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
 *   - `ICRNL` comes from `<termios.h>`
 *   - Now we have the whole `Ctrl` alphabet to work with
 *   - Notice that the enter key is now read as 13 instead of 10. (Carriage
 *     return instead of newline). It was also being affected by ICRNL.
 */

