#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#endif

extern int init_dot9it(char* root);
extern int mkfolder(char* root, char* dir_name);

#endif
