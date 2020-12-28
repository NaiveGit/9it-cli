#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// add sm for .9it dir instead of this mess
#define DOT_DIR ".9it/"
#define OBJ_DIR DOT_DIR "obj/"
#define INDEX_FILE DOT_DIR "index"
#define HEAD_FILE DOT_DIR "HEAD"

/* index stuff */
#define INDEX_DEFAULT_HEADER "DIRC\x00\x00\x00\x01\x00\x00\x00\x00"
#define INDEX_HEADER_START 0
#define INDEX_HEADER_ENTRY_START 8
#define INDEX_HEADER_ENTRY_LENGTH 4
#define INDEX_HEADER_LENGTH 12

#define INDEX_ENTRY_START INDEX_HEADER_LENGTH
#define INDEX_ENTRY_LENGTH 0

/* head stuff */
#define HEAD_DEFAULT "refs/heads/senpai"

#endif
