#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdarg.h>
#include <openssl/sha.h>
#include <zlib.h>

#ifdef _WIN32
#include <conio.h>
#endif

/* init related stuff */
extern int mkfolder(char* root, char* dir_name);

/* utilities - STREAM MUST BE IN BINARY MODE! */
extern int write_to_file(char* file_path, char* contents, int length);
extern int copy_stream(FILE* instream, FILE* outstream);
extern unsigned char* hash_stream(FILE* stream);
extern char* hash_to_string(unsigned char* hash);
extern int compress_file(FILE* stream, char* outname);

extern char* read_until_null(FILE* stream);
extern void write_hash(FILE* stream, unsigned char* hash);
extern void write_null(FILE* stream);

/* string stuff */
extern char* cat_str(int num, ...);
extern int strend(char* s, char* t);

/* directory stuff */
extern char* get_cwd(void);
extern char* get_local_path(void);


#endif
