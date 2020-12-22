#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <argp.h>

typedef struct argp_option ArgpOption;
typedef struct argp Argp;
typedef struct argp_state ArgpState;

extern Argp global_argp;
extern Argp add_argp;
extern Argp commit_argp;

#endif
