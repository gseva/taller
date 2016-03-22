
#include "protocol.h"

#define M 0x00010000


int checksum(char* x, int bytes) {
  int lower = 0, higher = 1;

  for (int i = 0; i < bytes; ++i)
  {
      lower = (lower + x[i]) % M;
      higher = ((bytes - i) * higher) % M;
  }

  return lower + (higher * M);
}
