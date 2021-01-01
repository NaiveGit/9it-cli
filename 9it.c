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

    /* printf("%s\n", get_dot_dir()); */
    /* printf("%s\n", get_repo_root()); */
    /* printf("%s\n", get_cwd()); */
    /* printf("%s\n", get_local_path()); */

    return 0;
}
