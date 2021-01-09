#ifndef __OBJUTILS_H__
#define __OBJUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define NODETYPE_SIZE 1

typedef struct stat Stat;
typedef struct dirent Dirent; 

typedef enum {
    NodeType_blob = 0,
    NodeType_tree = 1
} NodeType;

typedef enum {
    ObjType_undefined,
    ObjType_blob,
    ObjType_tree,
    ObjType_commit
} ObjType;

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
    unsigned char* root_tree_hash;
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
extern int write_blob(char* file_path);
extern int write_tree(Tree* tree);
extern int write_commit(Commit* commit);

/* read objects */
extern int read_blob(unsigned char* hash, char* out_path);
extern int read_tree(Tree* root);
extern int read_commit(Commit* commit);

/* hashing objects */
extern int hash_tree(Tree* tree); // writes hash to tree's hash field
extern int hash_commit(Commit* commit);

/* index related */
extern int add_index_item(char* file_path);
extern int add_index_dir(char* file_path);
extern int append_index(char* file_path);
extern int remove_index(char* file_path);
extern int write_index(Index* index);
extern int clear_index(void);
extern int clear_temp(void);
extern Index* read_index(void);

/* ref / head related */
extern unsigned char* get_head_commit(void);
extern int write_head_commit(char* branch_name);
extern char* get_cur_branch(void);
extern unsigned char* read_ref(char* branch_name);
extern int write_ref(char* branch_name, unsigned char* hash);

/* logging */
extern ObjType obj_type(char* obj_path);
extern void log_tree(char* obj_path);
extern void log_commit(char* obj_path);


#endif
