
#include "headers/options.h"
#include "headers/fileutils.h"
#include "headers/objutils.h"
#include "headers/commands.h"
#include "headers/nodeutils.h"
static void parse_command(char* cmd_name, Argp* argp, ArgpState* state);

static error_t parse_global_opt(int key, char* arg, ArgpState* state);
static error_t parse_add_opt(int key, char* arg, ArgpState* state);
static error_t parse_commit_opt(int key, char* arg, ArgpState* state);
static error_t parse_init_opt(int key, char* arg, ArgpState* state);

static void log_state(ArgpState* state);
static char* append_argv0(char* str, char* append);

/* global command */
static char global_doc[] = "\
9it - The version control system that is totally not git\v\
Commands:\n\
    add\n\
    commit\n\
    init\n\
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
    {"message", 'm', "msg", 0, "commit message"},
    {0}
};
Argp commit_argp = {
    commit_options,
    parse_commit_opt,
    0,
    commit_doc
};

/* INIT subcommand */
static char init_doc[] = "initialize the 9it repo";
static ArgpOption init_options[] = {
    {"bare", 'b', 0, 0, "bare grylls"},
    {0}
};
Argp init_argp = {
    init_options,
    parse_init_opt,
    0,
    init_doc
};

static void
parse_command(char* cmd_name, Argp* argp, ArgpState* state)
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
    argv[0] = append_argv0(argv0, cmd_name);

    argp_parse(argp, argc, argv, ARGP_IN_ORDER, 0, 0);

    /* restore argv[0] */
    free(argv[0]);
    argv[0] = argv0;
    
    /* empty the rest of args */
    /* state->next += argc - 1; */
    state->next = state->argc;

    return;
}

static error_t
parse_global_opt(int key, char* arg, ArgpState* state)
{
    switch (key) {

        case ARGP_KEY_ARG:
            if (strcmp(arg, "add") == 0 || strcmp(arg, "stage") == 0) {
                /* printf("Add command!\n"); */
                parse_command("add", &add_argp, state);
            } else if (strcmp(arg, "commit") == 0) {
                /* printf("Commit command!\n"); */
                parse_command("commit", &commit_argp, state);
            } else if (strcmp(arg, "init") == 0) {
                /* printf("Init command!\n"); */
                parse_command("init", &init_argp, state);
            } else {
                /* argp_error(state, "%s is not a valid command", arg); */    
                argp_usage(state);
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
            /* add_index_item(arg); */
            add(arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static error_t
parse_commit_opt(int key, char* arg, ArgpState* state)
{
    switch (key) {
        case 'm':
            /* printf("Called commit with -m flag with message: %s\n", arg); */
            commit(arg);
            break;

        case ARGP_KEY_NO_ARGS: 
            
            if (state->argc == 1) {
                char* editor;

                editor = getenv("EDITOR");
                if (editor == NULL) {
                    printf("Please set the EDITOR environment variable or run 9it commit -m [message]");
                    exit(1);
                }

                /* open buffer with editor */

            }
            
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static error_t
parse_init_opt(int key, char* arg, ArgpState* state)
{

    switch (key) {

        case 'b':
            init("");
            break;

        case ARGP_KEY_NO_ARGS: 
            
            // handle no args
            if (state->argc == 1) {
                init(NON_BARE_DIR);
            }
            
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
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
