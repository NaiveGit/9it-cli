#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "options.h"

static void add_command(ArgpState* state);

static error_t parse_global_opt(int key, char* arg, ArgpState* state);
static error_t parse_add_opt(int key, char* arg, ArgpState* state);
static error_t parse_commit_opt(int key, char* arg, ArgpState* state);

static void log_state(ArgpState* state);
static char* append_argv0(char* str, char* append);

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
static char add_doc[] = "stages files for commit";
static ArgpOption add_options[] = {
    {"all", 'a', 0, 0, "add all files in working directory to index"},
    {"update", 'u', 0, 0, "stages all tracked files"},
    {0}
};
Argp add_argp = {
    add_options,
    parse_add_opt,
    0,
    add_doc
};

/* COMMIT subcommand */
static char commit_doc[] = "create a commit";
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

static void
add_command(ArgpState* state)
{
    char* argv0;
    int next;
    int argc;
    char** argv;

    argv0 = state->argv[0]; // save argv0 value
    next = state->next;
    argc = state->argc-next+1; // keep argv[0]
    argv = &state->argv[next-1];
    
    /* change argv[0] to name of subcommand */
    argv[0] = append_argv0(argv0, "add");

    argp_parse(&add_argp, argc, argv, ARGP_IN_ORDER, 0, 0);

    /* restore argv[0] */
    free(argv[0]);
    argv[0] = argv0;
    
    /* empty the rest of args */
    state->next = state->argc;

    return;
}

static error_t
parse_global_opt(int key, char* arg, ArgpState* state)
{
    switch (key) {
        case ARGP_KEY_ARG:

            if (strcmp(arg, "add") == 0) {
                printf("Add command!\n");
                add_command(state);
            } else if (strcmp(arg, "commit") == 0) {
                printf("Commit command!\n");
            } else {
                argp_error(state, "%s is not a valid command", arg);    
            }

            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static error_t
parse_add_opt(int key, char* arg, ArgpState* state)
{
    switch (key) {
        case 'a':
            printf("Add all option\n");
            break;
        case 'u':
            printf("Add update option\n");
            break;
        case ARGP_KEY_ARG:
            printf("File %s\n", arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static error_t
parse_commit_opt(int key, char* arg, ArgpState* state)
{
    return 0;
}

static void
log_state(ArgpState* state)
{
    printf("next: %d\n", state->next);
}

static char* 
append_argv0(char* str, char* append)
{   
    char* dest;    

    // account for space and null character
    dest = malloc(strlen(str)+1+strlen(append)+1); 
    sprintf(dest, "%s %s", str, append);

    return dest;
}
