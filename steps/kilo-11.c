/* # Turn off output processing
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
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (isprint(c)) {
      printf("%d ('%c')\r\n", c, c);
    } else {
      printf("%d\r\n", c);
    }
  }

  return 0;
}

/* Explain:
 *   - `OPOST` comes from `<termios.h>`
 *   - It disables all other output processing flags in the `c_oflag` field
 *   - Generally the only thing this affects is `ONLCR`, which causes newlines
 *     to be translated into CR-NL's. From now on we'll have to output "\r\n"
 *     to get a newline with the cursor moved all the way to the left.
 */

