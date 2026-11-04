#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define MESSAGE                                                              \
  "Hello there my name is Colin Melican and I am transmitting this message " \
  "via a socket. "

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

  const char* errstr = "OK";
  switch (err) {
  case EAI_BADFLAGS:
    errstr = "bad flags";
  case EAI_FAMILY:
    errstr = "family";
  case EAI_NONAME:
    errstr = "no name";
  case EAI_SERVICE:
    errstr = "service";
  case EAI_SOCKTYPE:
    errstr = "sock type";
  }

  if (err == 0) {
    puts("OK");
  } else {
    printf("error: %s (%i); errno: %i\n", errstr, err, errno);
  }

  freeaddrinfo(ai);

  puts("Done");
}
