/* # Disable raw mode at exit
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
  raw.c_lflag &= ~(ECHO);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}

/* Explain:
 *   - `atexit()` comes from `<stdlib.h>`
 *   - structure assignment
 *   - echoing is turned back on when it exits in bash, also the TCSAFLUSH on
 *     exit prevents extra input from going to the shell afterwards
 */

