#include "tcp_select_client.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <str:address> <int:port_number>\n", argv[0]);
    return EXIT_FAILURE;
  }
  int socketfd = setup_client(argv[1], argv[2]);
  handle_connection(socketfd);
  close_connection(socketfd);
}

int setup_client(char *address, char *port) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));

  struct addrinfo *peer_address;
  check(getaddrinfo(address, port, &hints, &peer_address), "getaddrinfo() failed.");

  int socketfd = check(
    socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol),
    "socket() failed."
  );

  check(
    connect(socketfd, peer_address->ai_addr, peer_address->ai_addrlen),
    "connect() failed."
  );
  freeaddrinfo(peer_address);

  return socketfd;
}

void handle_connection(int socketfd) {
  while (1) {
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(socketfd, &reads);
    FD_SET(STDIN_FILENO, &reads);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    check(select(socketfd + 1, &reads, 0, 0, &timeout), "select() failed.");

    if (FD_ISSET(socketfd, &reads)) {
      char read[READ_BUFFER];
      ssize_t bytes_received = check(recv(socketfd, read, sizeof(read), 0), "recv() failed.");

      if (bytes_received < 1) {
        printf("Connection closed by peer.\n");
        break;
      }

      printf("Bytes received: %zd\n", bytes_received);
      printf("%.*s", (int)bytes_received, read);
    }

    if (FD_ISSET(STDIN_FILENO, &reads)) {
      char sent[READ_BUFFER];
      if (!fgets(sent, READ_BUFFER, stdin)) break;
      ssize_t bytes_sent = check(send(socketfd, sent, strlen(sent), 0), "send() failed.");
      printf("Sent: %zd bytes\n", bytes_sent);
    }
  }
}

void close_connection(int socketfd) {
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd);
}
