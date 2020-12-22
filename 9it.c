
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/options.h"
#include "headers/objutils.h"

int
main(int argc, char** argv)
{
    
    argp_parse(&global_argp, argc, argv, ARGP_IN_ORDER, 0, 0);

    int size;
    char* hashed = hash_file("README.md", &size);
    printf("%s\n", hashed);

    /* compress_file("9it", "test.gz"); */

    free(hashed);

    return 0;
}
