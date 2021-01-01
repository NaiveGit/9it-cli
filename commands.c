#include "headers/fileutils.h"
#include "headers/objutils.h"
#include "headers/globals.h"
#include "headers/commands.h"
#include "headers/nodeutils.h"

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

int
add(char* local_path)
{
    char* relative_path;
    int relative_length;

    /* make local_path relative to repo root */
    relative_path = get_local_path();
    relative_length = strlen(relative_path)+strlen(local_path);
    relative_path = realloc(relative_path, relative_length+1);
    strcat(relative_path, local_path);

    /* check if dir or file */
    printf("Adding file with path %s\n", relative_path);
    add_index_item(relative_path);

    free(relative_path);

    return 0;
}

int
commit(char* commit_msg)
{ // tree must be hashed and ready
    Commit new_commit;
    Tree* root_tree;
    char* username;
    unsigned char* parent_commit_hash;

    /* grab the index tree */
    root_tree = commit_tree();

    /* get user name */
    username = getenv(NINEIT_USERNAME);
    if (username == NULL) {
        printf("Please set the NINEIT_USERNAME environment variable.\n");
        exit(1);
    }
    
    /* make commit */
    new_commit.root_tree = root_tree;
    new_commit.committer = username;
    new_commit.timestamp = time(0);
    new_commit.msg = commit_msg;
    new_commit.parent_commit_hash = get_head_commit();

    /* write commit */
    hash_commit(&new_commit); 
    write_commit(&new_commit);

    /* clear index */

    return 0;
}
