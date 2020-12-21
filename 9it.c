
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/options.h"

int
main(int argc, char** argv)
{
    
    argp_parse(&global_argp, argc, argv, ARGP_IN_ORDER, 0, 0);

    return 0;
}
