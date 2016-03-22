
#include <strings.h>
#include <stdlib.h>

#include "server.h"
#include "protocol.h"


int server_create(server_t* s, int port) {
  socket_t skt;
  socket_create(&skt);
  s->skt = skt;
  s->port = port;
  bag_create(&(s->checksums));
  return 0;
}


int server_receive_filename(server_t* s) {
  int ret;
  char *data = (char*) &ret;
  socket_read(&(s->c_skt), data, 4);
  printf("El tamaño es %d\n", ret);

  ret = ntohl(ret);
  printf("El tamaño host es %d\n", ret);
  s->new_file = malloc(ret);
  socket_read(&(s->c_skt), s->new_file, ret);

  printf("Recibo filename: %s\n", s->new_file);
  return 0;
}

int server_receive_block_size(server_t* s) {
  int block_size;
  char *data = (char*) &block_size;
  socket_read(&(s->c_skt), data, 4);

  s->block_size = ntohl(block_size);
  printf("Mi block size es: %d\n", s->block_size);
  return 0;
}

int server_receive_checksums(server_t* s) {
  char flag;
  int checksum;
  do
  {
    socket_read(&(s->c_skt), &flag, 1);
    printf("Recibo flag %d\n", flag);
    if (flag == P_CHECKSUM_START) {
      socket_read(&(s->c_skt), (char*) &checksum, 4);
      bag_add(&(s->checksums), ntohl(checksum));
    }
  } while (flag != P_NO_MORE_CHECKSUMS);
  return 0;
}

int server_send_chunk(server_t* s, char* chunk, int bytes) {
  char flag = P_NEW_FILE_CHUNK;
  socket_write(&(s->skt), &flag, 1);
  bytes = htonl(bytes);
  socket_write(&(s->skt), (char*) &bytes, 4);
  return socket_write(&(s->skt), chunk, bytes);
}

int server_sync_file(server_t* s) {
  FILE *new_file;
  size_t bytes_read;
  int block_index = 0;
  int chunk_size = 0;
  char flag;
  char chunk[256];

  new_file = fopen(s->new_file, "rb");
  if (!new_file) return -1;

  char* block = malloc(s->block_size);
  bzero(chunk, 256);

  while (!feof(new_file)) {
    bytes_read = fread(block, 1, s->block_size, new_file);

    // Si lei menos bytes que el block_size, los agrego al chunk y corto el ciclo
    if (bytes_read < s->block_size) {
      strncat(chunk, block, bytes_read);
      chunk_size += bytes_read;
      break;
    }

    int cs = checksum(block, s->block_size);

    if ((block_index = bag_search(&(s->checksums), cs)) > 0) {
      // Si encuentro un checksum, primero tengo que vaciar el chunk que tengo
      if (chunk_size > 0) {
        server_send_chunk(s, chunk, chunk_size);
        bzero(chunk, 256);
        chunk_size = 0;
      }
      // Luego envio el numero de bloque encontrado
      // Estoy confiando en que el cliente me mando los checksums en orden
      flag = P_BLOCK_FOUND;
      socket_write(&(s->skt), &flag, 1);
      block_index = htonl(block_index);
      socket_write(&(s->skt), (char*) &block_index, 4);
    } else {
      // Si no encuentro checksum, agrego el primer byte del bloque al chunk
      // y muevo el puntero del archivo (1 - el tamaño del bloque).
      strncat(chunk, block, 1);
      fseek(new_file, 1 - s->block_size, SEEK_CUR);
      chunk_size++;
    }
  }

  if (chunk_size > 0) {
    server_send_chunk(s, chunk, chunk_size);
  }

  free(block);
  return 0;
}


int server_run(server_t* s) {
  socket_bind_and_listen(&(s->skt), s->port);
  s->c_skt = socket_accept(&(s->skt));

  server_receive_filename(s);
  server_receive_block_size(s);
  server_receive_checksums(s);

  server_sync_file(s);

  return 0;
}

int server_destroy(server_t* s) {
  socket_destroy(&(s->skt));
  socket_destroy(&(s->c_skt));
  free(s->new_file);
  bag_destroy(&(s->checksums));
  return 0;
}
