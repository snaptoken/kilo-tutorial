/* # Poll for keyboard input
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
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  while (1) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    if (isprint(c)) {
      printf("%d ('%c')\r\n", c, c);
    } else {
      printf("%d\r\n", c);
    }
    if (c == 'q') break;
  }

  return 0;
}

/* Explain:
 *   - `VMIN`, `VTIME` come from `<termios.h>`
 *   - Why do we want `read()` to timeout after 100ms?
 */

