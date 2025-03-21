#ifndef TCP_CLIENT
#define TCP_CLIENT
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include "../utils/error.h"

/** @returns socketfd - Socket file description **/
int connect_client(char *address, char *port);
#endif
