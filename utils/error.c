#include "error.h"

int handle_error(int return_val, char *error_message, char *file, int line_no) {
  if (return_val < 0) {
    fprintf(
      stderr,
      "%s\nfile: %s\nline number: %d\nerror: %s (%d)\n",
      error_message,
      file,
      line_no,
      strerror(errno),
      errno
    );
    exit(EXIT_FAILURE);
  }
  return return_val;
}
