#include "headers/fileutils.h"
#include "headers/globals.h"
#include "headers/commands.h"

int 
init(char* root) 
{
    char* path;
    int ind;
    char* dirs[] = {
        "obj/",
        "refs/tags/",
        "refs/heads/",
        0
    };

    ind = 0;
    while ((path = dirs[ind]) != 0) {

        if (mkfolder(root, path) == -1) {
            // prob also clean up half created dir
            return -1;
        }
         
        ind++;
    }

    /* error check these ma nibba */
    char* index_file = cat_str(2, root, INDEX_FILE);
    char* head_file = cat_str(2, root, HEAD_FILE);
    char* ident_file = cat_str(2, root, IDENT_FILE);

    write_to_file(index_file, INDEX_DEFAULT_HEADER, INDEX_HEADER_LENGTH); //index file
    write_to_file(head_file, HEAD_DEFAULT, strlen(HEAD_DEFAULT)+1);
    write_to_file(ident_file, "", 0);

    free(index_file);
    free(head_file);
    free(ident_file);

    printf("Successfully initialized 9it in current working directory.\n");

    return 0;
}
