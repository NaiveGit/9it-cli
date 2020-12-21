
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"

int
main(int argc, char** argv)
{
    
    argp_parse(&global_argp, argc, argv, 0, 0, 0);

    return 0;
}
