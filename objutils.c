#include "headers/objutils.h"
#include "headers/fileutils.h"
#include "headers/globals.h"

int
write_blob(char* file_path)
{
    FILE* file;
    char* hashed_filename;
    int hash_length;
    char* out_path;

    /* open original file */
    file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Something went wrong opening %s\n", file_path);
        return -1;
    }

    hashed_filename = hash_stream(file, &hash_length);

    /* build the correct out dir */
    out_path = malloc(strlen(OBJ_DIR)+strlen(hashed_filename)+1);
    memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(out_path, hashed_filename);

    compress_file(file, out_path); // prob should error check this

    /* clean up */
    free(hashed_filename);
    free(out_path);
    free(file);

    return 0;
}

int
write_tree(Tree* tree)
{
    /* shove everything into hash */

    return 0;
}

int
write_commit(Commit* commit)
{

    return 0;
}

int
init_index(void)
{
    return 0;
}

int
read_index(void)
{
    return 0;
}
