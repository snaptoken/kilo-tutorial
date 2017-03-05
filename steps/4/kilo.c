/* # Turn off echoing
 */

#include <termios.h>
#include <unistd.h>

void enableRawMode() {
  struct termios raw;

  tcgetattr(STDIN_FILENO, &raw);

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
 *   - `tcgetattr()`, `tcsetattr()`, `TCSAFLUSH`, `struct termios`, `ECHO` come
 *     from `<termios.h>`
 *   - It's the exact same program, just doesn't show you what you're typing
 *   - How to `reset` your terminal (make sure to try it with bash, not zsh)
 *   - Bit flipping with `&=~`
 *   - Flags are categorized kind of randomly as c_iflag, c_oflag, c_cflag,
 *     c_lflag
 */

