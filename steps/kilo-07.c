/* # Display keypresses
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
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);

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
 *   - `printf()` comes from `<stdio.h>`
 *   - `isprint()` comes from `<ctype.h>`
 *   - Show that holding `Ctrl` while pressing a key just clears the sixth and
 *     seventh bits of the pressed key, resulting in `Ctrl-A` being 1, `Ctrl-B`
 *     being 2, `Ctrl-C` being 3, ..., `Ctrl-Z` being 26
 */

