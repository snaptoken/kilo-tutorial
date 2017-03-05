/* # Error handling
 *
 * It is bad practice to not check for errors when calling C library functions
 * such as `tcgetattr()` and `tcsetattr()`. These kinds of functions can fail
 * in many ways at various points in your program, and not handling the errors
 * immediately can make them very hard to debug when they do happen.
 *
 * We will define a `die()` function that prints out fatal errors and exits
 * the program immediately. We will use it to handle any errors that we can't
 * recover from.
 */

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void die(const char *s) {
  perror(s);
  exit(1);
}

static struct termios orig_termios;

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  struct termios raw;

  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int main() {
  enableRawMode();

  return 0;
}

/* `tcgetattr()` and `tcsetattr()` both return `0` on success, and `-1` on
 * failure. Like many C library functions, on failure they set the global
 * `errno` variable to indicate what the error was. `perror()` (which comes
 * from `<stdio.h>`) looks at the `errno` code and prints out a somewhat
 * human-readable error message to let you know what went wrong. We have also
 * arranged for the name of the function that failed to be passed to `die()`,
 * which passes it to `perror()`, which prints it as a prefix to the error
 * message, which helps you find the source of the error in your code.
 *
 * When `die()` is called, it exits the program using `exit()`, which comes
 * from `<stdlib.h>`. It returns an exit status of `1` to the operating system
 * which indicates failure (as would any non-zero value). As noted previously,
 * `disableRawMode()` will get called as the program exits, since it was
 * registered by `atexit()`.
 *
 * To test this out, try running `cat kilo.c | ./kilo`. This replaces the
 * standard input of your program with a pipe instead of your terminal.
 * `tcgetattr()` will fail as a result and you should see an error message.
 */

