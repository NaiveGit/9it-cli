#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <time.h>

extern int init(char* root);
extern int add(char* local_path);
extern int branch(char* branch_name);
extern int branch_delete(char* branch_name);
extern int branch_info(void);
extern int cat(char* obj_path);
extern int checkout(char* branch_name);
extern int commit(char* commit_msg);
extern int log_horizon(void);
extern int revert(char* hash);
extern int status(void);

#endif
