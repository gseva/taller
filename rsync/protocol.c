
#include "protocol.h"

#define M 2^16


int checksum(char* x, int bytes) {
  int lower = 0, higher = 0;

  for (int i = 0; i < bytes; i++)
  {
      lower = (lower + x[i]) % M;
      higher = ((bytes - i) * higher) % M;
  }

  return lower + (higher * M);
}
