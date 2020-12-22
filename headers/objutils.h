#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <zlib.h>

typedef enum {
    blob,
    tree,
    commit
} ObjType;

typedef struct TreeItem {
    ObjType type;
    char* hash;
    char* file_name;
} TreeItem;

typedef struct Tree {
    TreeItem* items;
} Tree;

typedef struct Commit {
    char* tree;
    char* parent_commit;
    char* author;
    char* committer;
    char* msg;
    char* timestamp;
} Commit;

extern char* hash_file(char* file_path, int* hash_length);
extern int compress_file(char* file_path, char* outname);

extern int write_blob(char* org_file_path);
extern int write_tree(Tree* tree);
extern int write_commit(Commit* commit);

#endif
