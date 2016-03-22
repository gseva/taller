
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "client.h"
#include "protocol.h"


int client_create(client_t* c, const char* hostname, int port,
                  const char* old_local_file, const char* new_local_file,
                  const char* remote_file, int block_size) {
  socket_t skt;
  socket_create(&skt);
  c->skt = skt;
  c->hostname = hostname;
  c->port = port;
  c->old_local_file = old_local_file;
  c->new_local_file = new_local_file;
  c->remote_file = remote_file;
  c->block_size = block_size;
  return 0;
}


int client_send_remote_filename(client_t* c) {
  int32_t size = htonl(strlen(c->remote_file));

  socket_write(&(c->skt), (char*) &size, 4);

  socket_write(&(c->skt), (char*) c->remote_file, strlen(c->remote_file));

  return 0;
}

int client_send_block_size(client_t* c) {
  int32_t size = htonl(c->block_size);

  socket_write(&(c->skt), (char*) &size, 4);

  return 0;
}


int client_send_checksums(client_t* c) {
  FILE *old_file;
  size_t bytes_read;
  char flag;

  printf("Trato de abrir %s\n", c->old_local_file);
  old_file = fopen(c->old_local_file, "rb");
  if (!old_file) return -1;  // TODO: manage error
  printf("Abri %s\n",  c->old_local_file);
  char* buffer = malloc(c->block_size);
  while ((bytes_read = fread(buffer, 1, c->block_size, old_file)) >= c->block_size) {
    printf("Leo %s\n", buffer);
    int cs = htonl(checksum(buffer, c->block_size));
    printf("Envio checksum: %d\n", cs);
    flag = P_CHECKSUM_START;
    socket_write(&(c->skt), &flag, 1);

    socket_write(&(c->skt), (char*) &cs, 4);
  }
  flag = P_NO_MORE_CHECKSUMS;
  socket_write(&(c->skt), &flag, 1);

  fclose(old_file);
  free(buffer);
  return 0;
}


int client_sync_file(client_t* c) {
  char flag;
  int bytes_to_write, ;
  FILE *old_file, *new_file;

  old_file = fopen(c->old_local_file, "rb");
  if (!old_file) return -1;
  new_file = fopen(c->new_local_file, "wb");
  if (!new_file) return -1;

  socket_read(&(c->c_skt), &flag, 1);
  while (flag != P_END_OF_FILE) {

    if (flag == P_NEW_FILE_CHUNK) {

      socket_read((&(c->c_skt), (char*) &bytes_to_write, 4);

    } else if (flag == P_BLOCK_FOUND) {

    } else {
      return -1;  // Incorrect flag
    }

    socket_read(&(c->c_skt), &flag, 1);

  }

  printf("RECV End of file\n");

  fclose(old_file);
  fclose(new_file)

}


int client_run(client_t* c) {
  socket_connect(&(c->skt), c->hostname, c->port);

  client_send_remote_filename(c);
  client_send_block_size(c);
  client_send_checksums(c);

  client_sync_file(c);

  return 0;
}



int client_destroy(client_t* c) {
  socket_destroy(&(c->skt));
  return 0;
}
