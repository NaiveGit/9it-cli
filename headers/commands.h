#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <time.h>

extern int init(char* root);
extern int add(char* local_path);
extern int cat(char* obj_path);
extern int commit(char* commit_msg);
extern int status(void);

#endif
