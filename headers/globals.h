#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>


// add sm for .9it dir instead of this mess
/* #define DOT_DIR ".9it/" */

#define OBJ_DIR "obj/"
#define REFS_DIR "refs/"
#define HEADS_DIR REFS_DIR "heads/"

#define INDEX_FILE "index"
#define HEAD_FILE "HEAD"
#define IDENT_FILE "9IT"

/* index stuff */
#define INDEX_DEFAULT_HEADER "DIRC\x00\x00\x00\x01\x00\x00\x00\x00"
#define INDEX_HEADER_START 0
#define INDEX_HEADER_ENTRY_START 8
#define INDEX_HEADER_ENTRY_LENGTH 4
#define INDEX_HEADER_LENGTH 12

#define INDEX_ENTRY_START INDEX_HEADER_LENGTH
#define INDEX_ENTRY_LENGTH 0

/* head stuff */
#define HEAD_DEFAULT "senpai"

extern const char* get_dot_dir(void);
extern void set_dot_dir(char* dir);

#endif
