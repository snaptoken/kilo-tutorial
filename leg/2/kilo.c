/* # Canonical mode
 */

#include <unistd.h>

int main() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1);
  return 0;
}

/* Explain:
 *   - `read()` comes from `<unistd.h>`
 *   - `STDIN_FILENO` comes from `<unistd.h>`
 *   - Canonical mode, and why we don't want it
 *   - What exactly happens when you hit `Ctrl-D` or `Ctrl-C`
 */

