#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <zlib.h>

extern char* hash_file(char* file_path, int* hash_length);
extern int compress_file(char* file_path, char* outname);

#endif
