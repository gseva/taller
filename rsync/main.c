
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "client.h"


int main(int argc, char const *argv[])
{
  if (argc < 3) {
    return 1;
  } else if (!strncmp(argv[1], "server", 6)) {
    server_t s;
    server_create(&s, atoi(argv[2]));
    server_run(&s);
    server_destroy(&s);

  } else if (!strncmp(argv[1], "client", 6)) {
    client_t c;
    // hostname port old_local new_local new_remote block_size
    client_create(&c, argv[2], argv[3], argv[4],
                      argv[5], argv[6], atoi(argv[7]));
    client_run(&c);
    client_destroy(&c);
  }

  return 0;
}
