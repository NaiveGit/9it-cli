#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct stat Stat;

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

typedef struct IndexItem {
    char* hash;
    char* file_name;
} IndexItem;

typedef struct Index {
    int index_length;
    IndexItem* index_items;
    char* checksum; // prob dont need?
} Index;

/* objects */
extern char* write_blob(char* file_path);
extern char* write_tree(Tree* tree);
extern char* write_commit(Commit* commit);

/* index */
extern int init_index(void);
extern Index* read_index(void);
extern Index* write_index(Index* index);
extern int add_index_item(char* file_path);

#endif
