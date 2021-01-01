#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define NODETYPE_SIZE 1

typedef struct stat Stat;

typedef enum {
    NodeType_blob = 0,
    NodeType_tree = 1
} NodeType;

typedef struct Tree Tree;
struct Tree {
    unsigned char* hash;
    char* name;
    NodeType nodeType; // Type of object
    int cnum;
    Tree* children;
};

typedef struct Commit Commit;
struct Commit {
    unsigned char* hash;
    Tree* root_tree;
    /* char* author; /1* we dont need author prob *1/ */
    char* committer; // read from enviroment variables
    time_t timestamp;
    char* msg;
    unsigned char* parent_commit_hash;
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

/* write objects */
extern char* write_blob(char* file_path);
extern char* write_tree(Tree* tree);
extern char* write_commit(Commit* commit);

/* read objects */
extern char* read_tree(Tree* root);

/* hashing objects */
extern void hash_tree(Tree* tree); // writes hash to tree's hash field
extern void hash_commit(Commit* commit);

extern int add_index_item(char* file_path);
extern int add_index_dir(char* file_path);
extern Index* read_index(void);

extern unsigned char* get_head_commit(void);
extern unsigned char* read_ref(char* branch_name);

/* logging */
extern void log_tree(Tree* tree);
extern void log_commit(Commit* commit);

#endif
