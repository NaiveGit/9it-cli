#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <openssl/sha.h>
#include <zlib.h>

#ifdef _WIN32
#include <conio.h>
#endif

#define HASH_LENGTH SHA_DIGEST_LENGTH*2+1

/* init related stuff */
extern int init_aux(char* root);
extern int mkfolder(char* root, char* dir_name);

/* utilities - STREAM MUST BE IN BINARY MODE! */
extern int copy_stream(FILE* instream, FILE* outstream);
extern char* hash_stream(FILE* stream);
extern int compress_file(FILE* stream, char* outname);

extern char* read_until_null(FILE* stream);

#endif
