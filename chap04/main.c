#include "udp_client.h"
#include "udp_server.h"
#include <ctype.h>

#define CHAR_BUFFER 1024

void handle_server(char *address, char *port);
void handle_client(char *address, char *port);

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("Usage: %s <str:client|server> <str:address> <int:port>\n", argv[0]);
    return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "server") == 0) {
    handle_server(argv[2], argv[3]);
    return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "client") == 0) {
    handle_client(argv[2], argv[3]);
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Invalid host type. Required 'client' or 'server', received '%s'\n", argv[1]);
  fprintf(stderr, "Usage: %s <str:client|server> <str:address> <int:port>\n", argv[0]);
  return EXIT_FAILURE;
}

void handle_server(char *address, char *port) {
  struct addrinfo *bind_address;
  int socketfd = udp_setup_server(address, port, &bind_address);

  printf("Server address is: ");
  char address_buffer[100];
  char service_buffer[100];
  getnameinfo(
    bind_address->ai_addr,
    bind_address->ai_addrlen,
    address_buffer, sizeof(address_buffer),
    service_buffer, sizeof(service_buffer),
    NI_NUMERICHOST | NI_NUMERICSERV
  );
  printf("%s %s\n", address_buffer, service_buffer);

  fd_set socket_set;
  FD_ZERO(&socket_set);
  FD_SET(socketfd, &socket_set);

  while (1) {
    fd_set read_sockets;
    read_sockets = socket_set;

    HANDLE_ERROR(select(socketfd + 1, &read_sockets, NULL, NULL, NULL), "select() failed.");

    if (FD_ISSET(socketfd, &read_sockets)) {
      struct sockaddr_storage client_address;
      socklen_t client_address_len = sizeof(client_address_len);

      char received[CHAR_BUFFER];
      ssize_t bytes_received = HANDLE_ERROR(recvfrom(socketfd, received, CHAR_BUFFER, 0, (struct sockaddr*)&client_address, &client_address_len), "recvfrom() failed.");
      printf("(Received bytes: %zd) message: %.*s", bytes_received, (int)bytes_received, received);

      char sent[CHAR_BUFFER];
      int i;
      for (i = 0; i < bytes_received; i++) {
        sent[i] = toupper(received[i]);
      }
      ssize_t bytes_sent = HANDLE_ERROR(sendto(socketfd, sent, strlen(sent), 0, (struct sockaddr*)&client_address, client_address_len), "");
    }
  }

  freeaddrinfo(bind_address);
  close_connection(socketfd);
}

void handle_client(char *address, char *port) {
  struct addrinfo *client_address;
  int socketfd = udp_setup_client(address, port, &client_address);

  printf("Client address is: ");
  char address_buffer[100];
  char service_buffer[100];
  getnameinfo(
    client_address->ai_addr,
    client_address->ai_addrlen,
    address_buffer, sizeof(address_buffer),
    service_buffer, sizeof(service_buffer),
    NI_NUMERICHOST | NI_NUMERICSERV
  );
  printf("%s %s\n", address_buffer, service_buffer);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 100000;

  while (1) {
    fd_set socket_set;
    FD_ZERO(&socket_set);
    FD_SET(socketfd, &socket_set);
    FD_SET(STDIN_FILENO, &socket_set);

    HANDLE_ERROR(select(socketfd + 1, &socket_set, NULL, NULL, &timeout), "select() failed.");

    if (FD_ISSET(socketfd, &socket_set)) {
      char received[CHAR_BUFFER];
      ssize_t bytes_received = HANDLE_ERROR(recvfrom(socketfd, received, CHAR_BUFFER, 0, client_address->ai_addr, &client_address->ai_addrlen), "recvfrom() failed");
      printf("(Received bytes: %zd) message: %.*s", bytes_received, (int)bytes_received, received);
    }

    if (FD_ISSET(STDIN_FILENO, &socket_set)) {
      char sent[CHAR_BUFFER];
      if (!fgets(sent, CHAR_BUFFER, stdin)) break;
      ssize_t bytes_sent = HANDLE_ERROR(sendto(socketfd, sent, strlen(sent), 0, client_address->ai_addr, client_address->ai_addrlen), "sendto() failed.");
      printf("(Sent bytes: %zd) %.*s", bytes_sent, (int)bytes_sent, sent);
    }
  }

  freeaddrinfo(client_address);
  close_connection(socketfd);
}
