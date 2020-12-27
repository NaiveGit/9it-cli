
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/fileutils.h"
#include "headers/options.h"
#include "headers/objutils.h"
#include "headers/globals.h"

int
main(int argc, char** argv)
{

    /* check if dot dir exists */
    /* set some global variable to location of dot dir */
    
    argp_parse(&global_argp, argc, argv, ARGP_IN_ORDER, 0, 0);

    /* init_dot9it(DOT_DIR); */ 
    /* write_blob("9it"); */
    /* Index index = *read_index(); */
    /* printf("the index has %d items in it\n", index.index_length); */
    /* add_index_item("README.md"); */
    /* read_index(); */

    return 0;
}
