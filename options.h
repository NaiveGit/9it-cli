#ifndef __PARSER_H__
#define __PARSER_H__

#include <argp.h>

typedef struct argp_option ArgpOption;
typedef struct argp Argp;
typedef struct argp_state ArgpState;

/* function declarations */
extern error_t parse_add_opt(int key, char* arg, ArgpState* state);
extern error_t parse_commit_opt(int key, char* arg, ArgpState* state);

/* global command */
static char doc[] = "totally not git";

/* ADD subcommand */
static char add_doc[] = "add";
static ArgpOption add_options[] = {
    {"all", 'a', 0, 0, "add all files in working directory to index"},
    {0}
};
Argp add_argp = {
    add_options,
    parse_add_opt,
    0,
    add_doc
};

/* COMMIT subcommand */
static char commit_doc[] = "commit";
static ArgpOption commit_options[] = {
    {"message", 'm', 0, 0, "commit message"},
    {0}
};
Argp commit_argp = {
    commit_options,
    parse_commit_opt,
    0,
    commit_doc
};

#endif


