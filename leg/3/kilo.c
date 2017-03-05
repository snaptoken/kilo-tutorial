/* # Press `q` to quit?
 */

#include <unistd.h>

int main() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}

/* Explain:
 *   - Why it doesn't quit immediately, and any input that came after the 'q'
 *     was read by the shell afterwards
 */

