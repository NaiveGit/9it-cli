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
/*

tree
-tree (folder)
-pointer to obj

*/

extern Tree* make_tree(Index* index);


#endif
