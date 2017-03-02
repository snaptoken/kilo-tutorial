#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct editorConfig {
  int screenrows;
  int screencols;
  int rawmode;
};

static struct editorConfig E;

enum KEY_ACTION {
  ESC = 27
};

void fatal(const char *s) {
  perror(s);
  exit(1);
}

static struct termios orig_termios;

void disableRawMode() {
  if (E.rawmode) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    E.rawmode = 0;
  }
}

void enableRawMode() {
  struct termios raw;

  if (E.rawmode) return;
  if (!isatty(STDIN_FILENO)) { errno = ENOTTY; fatal("enableRawMode"); }
  atexit(disableRawMode);
  if (tcgetattr(STDIN_FILENO, &orig_termios) < 0) fatal("tcgetattr");

  raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) fatal("tcsetattr");
  E.rawmode = 1;
}

int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) == 0);
  if (nread == -1) fatal("read");
  return c;
}

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;

  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, buf + i, 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';

  if (buf[0] != ESC || buf[1] != '[') return -1;
  if (sscanf(buf + 2, "%d;%d", rows, cols) != 2) return -1;
  return 0;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    int orig_row, orig_col, retval;

    retval = getCursorPosition(&orig_row, &orig_col);
    if (retval == -1) return -1;

    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    retval = getCursorPosition(rows, cols);
    if (retval == -1) return -1;

    char seq[32];
    snprintf(seq, 32, "\x1b[%d;%dH", orig_row, orig_col);
    if (write(STDOUT_FILENO, seq, strlen(seq)) == -1) {
      /* Can't recover... */
    }
    return 0;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL) return;
  memcpy(new + ab->len, s, len);
  ab->b = new;
  ab->len += len;
}

void abFree(struct abuf *ab) {
  free(ab->b);
}

void editorRefreshScreen() {
  int y;
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[H", 3);
  for (y = 0; y < E.screenrows; y++) {
    abAppend(&ab, "~\x1b[0K", 5);
    if (y < E.screenrows - 1) {
      abAppend(&ab, "\r\n", 2);
    }
  }
  abAppend(&ab, "\x1b[H", 3);
  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorProcessKeypress() {
  int c = editorReadKey();

  switch (c) {
    case 'q':
      exit(0);
      break;
  }
}

void initEditor() {
  E.rawmode = 0;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
    fatal("Unable to query the screen for size (columns / rows)");
  }
}

int main() {
  initEditor();
  enableRawMode();

  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }

  return 0;
}

