#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define MESSAGE "hello, I am sending a message!"
#define BUF_SIZE 500

// for debugging
const int af_inet = AF_INET;
const int af_inet6 = AF_INET6;
const int af_unix = AF_UNIX;
const int af_unspec = AF_UNSPEC;

int
main(int argc, char** argv) {
  if (argc > 3) {
    printf("Usage: %s [ip] [port]\n", argv[0]);
    return 1;
  }

  const char* ip;
  const char* port;

  if (argc >= 2) {
    ip = argv[1];
  } else {
    ip = "localhost";
  }

  if (argc >= 3) {
    port = argv[3];
  } else {
    port = "63036";
  }

  const struct addrinfo hints = {
    .ai_flags = 0,
    .ai_family = AF_INET6,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = getprotobyname("tcp")->p_proto,
  };

  printf("proto num: %i\n", hints.ai_protocol);

  struct addrinfo* ai;

  int err = getaddrinfo(ip, port, &hints, &ai);
  if (err != 0) {
    printf("error: '%s' (%i); errno: %i\n", gai_strerror(err), err, errno);
    return err;
  }

  int sock;
  struct addrinfo* ai_ptr;
  for (ai_ptr = ai; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
    sock = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
    if (sock == -1) continue;

    err = connect(sock, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
    if (err == 0) break;

    close(sock);
  }

  printf("sock: %i\n", sock);

  freeaddrinfo(ai);

  if (ai_ptr == NULL) {
    printf("could not connect. errno: %i\n", errno);
    return errno;
  }

  ssize_t write_size = write(sock, MESSAGE, sizeof(MESSAGE));
  if (write_size == -1 || write_size != sizeof(MESSAGE)) {
    printf("error writing to socket. errno: %i\n", errno);
    return errno;
  }

  char buf[BUF_SIZE];
  ssize_t read_size = read(sock, buf, BUF_SIZE);
  if (read_size == -1) {
    printf("error reading from socket. errno: %i\n", errno);
    return errno;
  }

  printf("received %zd bytes: '%s'\n", read_size, buf);

  shutdown(sock, SHUT_RDWR);
  close(sock);

  puts("Done");
}
