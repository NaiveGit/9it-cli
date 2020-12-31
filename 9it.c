#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/fileutils.h"
#include "headers/options.h"
#include "headers/objutils.h"
#include "headers/globals.h"
#include "headers/nodeutils.h"

int
main(int argc, char** argv)
{

    /* check if dot dir exists */
    /* set some global variable to location of dot dir */
    
    argp_parse(&global_argp, argc, argv, ARGP_IN_ORDER, 0, 0);


    /* printf("%d\n", read_index()->index_length); */

    return 0;
}
