#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

int
main(int argc, char** argv) {
  if (argc < 2 || argc > 3) {
    printf("Usage: %s <ip> [port]\n", argv[0]);
    return 1;
  }

  const char* ip = argv[1];
  const char* port;

  if (argc >= 3) {
    port = argv[3];
  } else {
    port = "63036";
  }

  const struct addrinfo hints = {
    .ai_flags = 0,
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = getprotobyname("tcp")->p_proto,
  };

  printf("proto num: %i\n", hints.ai_protocol);

  struct addrinfo* ai;

  int err = getaddrinfo(ip, port, &hints, &ai);
  if (err != 0) {
    printf("error: %i; errno: %i\n", err, errno);
    return err;
  }

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
    printf("could not connect. errno: %i\n", errno);
    return errno;
  }

  puts("Done");
}
