#ifndef __NODEUTILS_H__
#define __NODEUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "objutils.h"
#include "globals.h"
#include "fileutils.h"
/*

tree
-tree (folder)
-pointer to obj

*/

extern Tree* commit_tree(void);
extern void print_tree(Tree* root);
extern void revert_commit(unsigned char* t_hash);
#endif
