#include "error.h"

int handle_error(int return_val, char *error_message, int line_no) {
  if (return_val < 0) {
    fprintf(
      stderr,
      "%s (line: %d, error: %s, errno: %d)\n",
      error_message,
      line_no,
      strerror(errno),
      errno
    );
    exit(EXIT_FAILURE);
  }
  return return_val;
}
