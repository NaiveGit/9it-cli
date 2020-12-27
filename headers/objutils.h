#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct stat Stat;

typedef enum {
    blob,
    tree
} NodeType;

typedef struct Tree Tree;
struct Tree {
    // Folder: Folder hash
    // File: Blob hash
    char* hash;
    NodeType nodeType; // Type of object
    char* name;

    // Pointers to children 
    Tree* children; // Nodes

    // For malloc n stuff
    int cnum;
};

typedef struct Commit Commit;
struct Commit {
    unsigned char* hash;
    Tree* root_tree;
    Commit* parent_commit;
    char* author;
    char* committer;
    char* msg;
    char* timestamp;
};

typedef struct IndexItem {
    time_t c_time;
    time_t m_time;
    uint32_t dev;
    uint32_t ino; // truncate the overflow
    uint32_t mode;
    uint32_t uid;
    uint32_t gid;
    uint32_t file_size;
    unsigned char* hash; // length of SHA_DIGEST_LENGTH (not null terminated)
    char* file_path; // should be null terminated
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
extern int add_index_item(char* file_path);

#endif
