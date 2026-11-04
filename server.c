#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 500

int
main() {
  const struct addrinfo hints = {
    .ai_flags = AI_PASSIVE,
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = getprotobyname("tcp")->p_proto,
  };

  printf("proto num: %i\n", hints.ai_protocol);

  struct addrinfo* ai;

  int err = getaddrinfo(NULL, "63036", &hints, &ai);
  if (err != 0) {
    printf("error: %i; errno: %i\n", err, errno);
    return err;
  }

  const struct sockaddr_in sin = {
    .sin_family = ai->ai_family,
    .sin_port = 63036,
    .sin_addr = *(struct in_addr*)(ai->ai_addr),
  };

  int sock;
  struct addrinfo* ai_ptr;
  for (ai_ptr = ai; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
    sock = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
    if (sock < 0) continue;

    err = connect(sock, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
    if (err == 0) break;
  }

  printf("sock: %i\n", sock);

  freeaddrinfo(ai);

  if (ai_ptr == NULL) {
    printf("could not bind. errno: %i\n", errno);
    return errno;
  }

  while (true) {
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    struct sockaddr_storage peer_addr;
  }

  freeaddrinfo(ai);
  puts("Done");
}
