#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MESSAGE "hello this is the server! how are you?"
#define BUF_SIZE 500

// for debugging
const int af_inet = AF_INET;
const int af_inet6 = AF_INET6;
const int af_unix = AF_UNIX;
const int af_unspec = AF_UNSPEC;

int sock;

int
main() {
  const struct addrinfo hints = {
    .ai_flags = AI_PASSIVE,
    .ai_family = AF_INET6,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = getprotobyname("tcp")->p_proto,
  };

  printf("proto num: %i\n", hints.ai_protocol);

  struct addrinfo* ai;

  int err = getaddrinfo(NULL, "63036", &hints, &ai);
  if (err != 0) {
    printf("error: '%s' (%i); errno: %i\n", gai_strerror(err), err, errno);
    return err;
  }

  const struct sockaddr_in sin = {
    .sin_family = ai->ai_family,
    .sin_port = 63036,
    .sin_addr = *(struct in_addr*)(ai->ai_addr),
  };

  struct addrinfo* ai_ptr;
  for (ai_ptr = ai; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
    sock = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
    if (sock == -1) continue;

    err = bind(sock, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
    if (err == 0) break;

    close(sock);
  }

  printf("sock: %i\n", sock);

  freeaddrinfo(ai);

  if (ai_ptr == NULL) {
    printf("could not bind. errno: %i\n", errno);
    return errno;
  }

  listen(sock, SOMAXCONN);

  struct sockaddr_storage client_addr;
  socklen_t client_addr_len;

  int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_len);

  if (client_sock == -1) {
    printf("could not accept client connection. errno: %i\n", errno);
    return errno;
  }

  char buf[BUF_SIZE];
  ssize_t readlen = read(client_sock, buf, BUF_SIZE);

  if (readlen == -1) {
    printf("could not read from client. errno: %i\n", errno);
    return errno;
  }

  printf("received %zd bytes from client: '%s'\n", readlen, buf);

  write(client_sock, MESSAGE, sizeof(MESSAGE));

  shutdown(sock, SHUT_RDWR);
  close(sock);

  puts("Done");
}
