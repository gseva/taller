#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define P_CHECKSUM_START 1
#define P_NO_MORE_CHECKSUMS 2

#define P_NEW_FILE_CHUNK 3
#define P_BLOCK_FOUND 4
#define P_END_OF_FILE 5


int checksum(char* x, int bytes);

#endif
