/* # Miscellaneous flags
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
 *   - `BRKINT`, `INPCK`, `ISTRIP`, `CS8` come from `<termios.h>`
 *   - These flags are either probably already turned off, or don't have an
 *     effect on emulated terminals and are there for historical reasons, or I
 *     am not sure what they do or how to get a visible effect from them. But
 *     they seem to be generally accepted to be required settings in order for
 *     it to be considered "raw mode".
 */

