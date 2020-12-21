#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"

static error_t parse_global_opt(int key, char* arg, ArgpState* state);
static error_t parse_add_opt(int key, char* arg, ArgpState* state);
static error_t parse_commit_opt(int key, char* arg, ArgpState* state);

/* global command */
static char global_doc[] = "\
9it - The version control system that is totally not git\v\
Commands:\n\
    add\n\
    commit\n\
";
static ArgpOption global_options[] = {
    {0}
};
Argp global_argp = {
    global_options,
    parse_global_opt,
    0,
    global_doc
};

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


static error_t
parse_global_opt(int key, char* arg, ArgpState* state)
{
    switch (key) {
        case ARGP_KEY_ARG:

            if (strcmp(arg, "add") == 0) {
                printf("Add command!\n");
            } else if (strcmp(arg, "commit") == 0) {
                printf("Commit command!\n");
            } else {
                argp_error(state, "%s is not a valid command", arg);    
            }

            break;

        /* default: */
    }
    return 0;
}

static error_t
parse_add_opt(int key, char* arg, ArgpState* state)
{
    return 0;
}

static error_t
parse_commit_opt(int key, char* arg, ArgpState* state)
{
    return 0;
}

