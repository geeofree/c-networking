#ifndef UTILS_ERROR_H
#define UTILS_ERROR_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int handle_error(int return_val, char *error_message, int line_no);
#define HANDLE_ERROR(return_val, error_message) (handle_error(return_val, error_message, __LINE__))

#endif
