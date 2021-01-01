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
#define HEADS_DIR REFS_DIR "heads/"

#define INDEX_FILE "index"
#define HEAD_FILE "HEAD"
#define IDENT_FILE "9IT"

/* file stuff */
#define INDEX_DEFAULT_HEADER "DIRC\x00\x00\x00\x01\x00\x00\x00\x00"
#define TREE_DEFAULT_HEADER "TREE\x00\x00\x00\x01\x00\x00\x00\x00"
#define COMMIT_DEFAULT_HEADER "CMIT\x00\x00\x00\x01\x00\x00\x00\x00"

#define HEADER_LENGTH 12

#define HEADER_START 0
#define HEADER_ENTRY_NUM_START 8
#define HEADER_ENTRY_NUM_LENGTH 4


/* head stuff */
#define HEAD_DEFAULT "senpai"

/* Enviros */
#define NINEIT_USERNAME "NINEIT_USERNAME"

extern const char* get_dot_dir(void);
extern char* get_repo_root(void);
extern void set_dot_dir(char* dir);

#endif
