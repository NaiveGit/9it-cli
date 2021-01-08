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
#define NON_BARE_DIR ".9it/"

#define OBJ_DIR "obj/"
#define REFS_DIR "refs/"
#define TEMP_DIR "temp/"
#define HEADS_DIR REFS_DIR "heads/"

#define INDEX_FILE "index"
#define HEAD_FILE "HEAD"
#define IDENT_FILE "9IT"

/* file stuff */
#define INDEX_TYPE "DIRC"
#define BLOB_TYPE "BLOB"
#define TREE_TYPE "TREE"
#define COMMIT_TYPE "CMIT"

#define INDEX_DEFAULT_HEADER INDEX_TYPE "\x00\x00\x00\x01\x00\x00\x00\x00"
#define BLOB_DEFAULT_HEADER BLOB_TYPE "\x00\x00\x00\x01\x00\x00\x00\x00"
#define TREE_DEFAULT_HEADER TREE_TYPE "\x00\x00\x00\x01\x00\x00\x00\x00"
#define COMMIT_DEFAULT_HEADER COMMIT_TYPE "\x00\x00\x00\x01\x00\x00\x00\x00"

#define HEADER_LENGTH 12

#define HEADER_START 0

#define HEADER_TYPE_START 0
#define HEADER_TYPE_LENGTH 4

#define HEADER_ENTRY_NUM_START 8
#define HEADER_ENTRY_NUM_LENGTH 4

#define DEL_HASH "\x70\x6f\x6a\x64\x69\x62\x6f\x6a\x20\x69\x62\x77\x66\x20\x62\x20\x65\x6a\x64\x6c"

/* head stuff */
#define HEAD_DEFAULT "senpai"

/* Enviros */
#define NINEIT_USERNAME "NINEIT_USERNAME"

extern const char* get_dot_dir(void);
extern char* get_repo_root(void);
extern void set_dot_dir(char* dir);

#endif
