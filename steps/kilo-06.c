/* # Turn off canonical mode
 */

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
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}

/* Explain:
 *   - `ICANON`, `IEXTEN` come from `<stdlib.h>`
 *   - `IEXTEN` is basically extended features for canonical mode? (It's turned
 *     off already on my terminal/shell)
 *   - Bitwise `|` operator
 *   - Now `read()` returns as soon as each key is pressed, and the program
 *     exits as soon as you press 'q'
 */

