#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>

typedef struct argp_option ArgpOption;
typedef struct argp Argp;
typedef struct argp_state ArgpState;

static error_t parse_opt(int key, char* arg, ArgpState* state);

static char args_doc[] = "status add";
static char doc[] = "totally not git";

static ArgpOption options[] = {
    {"add", 'a', 0, 0, "stage for commit"},
    {0}
};

static Argp argp = {
    options,
    parse_opt,
    args_doc,
    doc
};

int
main(int argc, char** argv)
{
    argp_parse(&argp, argc, argv, 0, 0, 0);
    
    return 0;
}

static error_t 
parse_opt(int key, char* arg, ArgpState* state)
{
    switch(key) {
        case 'a':
            printf("Add!\n");
            break;
        defualt:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

