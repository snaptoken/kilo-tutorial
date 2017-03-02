/* # Disable raw mode at exit
 *
 * To disable raw mode, we are going to save a copy of the terminal attributes
 * before we modify them. Then when the program exits, we'll restore the
 * original attributes using `tcsetattr()`.
 */

#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

static struct termios orig_termios;

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

  return 0;
}

/* `atexit()` registers a function to be called whenever the program exits,
 * whether by returning from `main()` or calling `exit()`. `atexit()` comes
 * from `<stdlib.h>`.
 *
 * We declare a global `termios` struct to store the original terminal
 * attributes. It is global because `disableRawMode()` needs access to it, and
 * we can't have arguments passed to functions registered with `atexit()`.
 *
 * You should be able to confirm that the program no longer messes up your
 * terminal after it exits.
 */

