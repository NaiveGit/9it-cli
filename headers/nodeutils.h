#ifndef __NODEUTILS_H__
#define __NODEUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*

tree
-tree (folder)
-pointer to obj

*/

typedef enum {
    blob,
    tree
} oType;

typedef struct Tree {

    // Folder: Folder hash
    // File: Blob hash
    char* hash;
    oType nodeType; // Type of object
    char* filename;

    // Pointers to children 
    Tree* children; // Nodes

    // For malloc n stuff
    int cnum;

} Tree;

extern Tree* commit(Index* index);


#endif
