#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define READ_CHUNK_SIZE 1024

extern unsigned char* hash_file(char* file_path, int* hash_length);

#endif
