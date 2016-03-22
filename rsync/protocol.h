#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define P_CHECKSUM_START '\x01'
#define P_NO_MORE_CHECKSUMS '\x02'

#define P_NEW_FILE_CHUNK '\x03'
#define P_BLOCK_FOUND '\x04'
#define P_END_OF_FILE '\x05'


int checksum(char* x, int bytes);

#endif
