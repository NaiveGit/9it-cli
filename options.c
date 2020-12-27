
#include "headers/options.h"
#include "headers/fileutils.h"
#include "headers/objutils.h"

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
            /* printf("File %s\n", arg); */
            add_index_item(arg);
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
            printf("Called commit with -m flag with message: %s\n", arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static error_t
parse_init_opt(int key, char* arg, ArgpState* state)
{
    char* working_dir;

    switch (key) {
        case 'b':
            printf("bare\n");
            // grab current path
            /* working_dir = getcwd(NULL, 0); */
            /* init_dot9it(working_dir); // bare is a bit broken rn */
            /* free(working_dir); */

            break;

        case ARGP_KEY_NO_ARGS: // bug rn, this is being called when --bare is passed too
            printf("not bare\n");
            /* working_dir = getcwd(NULL, 0); // maybe abstract this to a global var later */
            /* char* dot9it = "/.9it"; // abstract this out sometime */
            /* working_dir = realloc(working_dir, strlen(working_dir)+strlen(dot9it)+1); */
            /* strcat(working_dir, dot9it); */
            /* init_dot9it(working_dir); */
            /* free(working_dir); */
            init_aux(".9it/");
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
