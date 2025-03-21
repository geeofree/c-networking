#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int handle_error(int return_val, char *error_message, int line_no);
#define check(return_val, error_message) (handle_error(return_val, error_message, __LINE__))

#endif
