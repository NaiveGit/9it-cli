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

/* init related stuff */
extern int init_aux(char* root);
extern int mkfolder(char* root, char* dir_name);

/* utilities */
extern char* hash_stream(FILE* stream, int* hash_length);
extern int compress_file(FILE* stream, char* outname);

#endif
