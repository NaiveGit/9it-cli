#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* objects */
extern int write_blob(char* file_path);
extern int write_tree(Tree* tree);
extern int write_commit(Commit* commit);

/* index */
extern int init_index(void);
extern int read_index(void);

#endif
