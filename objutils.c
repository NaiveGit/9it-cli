#include "headers/objutils.h"
#include "headers/fileutils.h"
#include "headers/globals.h"

#define INDEX_DEFAULT_HEADER "DIRC\x00\x00\x00\x01\x00\x00\x00\x00"
#define INDEX_HEADER_START 0
#define INDEX_HEADER_ENTRY_START 8
#define INDEX_HEADER_ENTRY_LENGTH 4
#define INDEX_HEADER_LENGTH 12

#define INDEX_ENTRY_START INDEX_HEADER_LENGTH
#define INDEX_ENTRY_LENGTH 0

char*
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
        return NULL;
    }

    hashed_filename = hash_stream(file, &hash_length);

    /* check to see if object already exists */
    if (access(hashed_filename, F_OK) != 0) { // does not exist

        /* create the file */
        /* build the correct out dir */
        out_path = malloc(strlen(OBJ_DIR)+strlen(hashed_filename)+1);
        memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
        strcat(out_path, hashed_filename);

        compress_file(file, out_path); // prob should error check this

        free(out_path);
    }

    /* clean up */
    free(file);

    return hashed_filename;
}

char*
write_tree(Tree* tree)
{
    /* shove everything into hash */

    return NULL;
}

char*
write_commit(Commit* commit)
{
    return NULL;
}

int
init_index(void)
{ 
    FILE* bufstream;
    FILE* outstream;

    bufstream = fmemopen(INDEX_DEFAULT_HEADER, INDEX_HEADER_LENGTH, "rb");
    outstream = fopen(INDEX_FILE, "wb");
    if (bufstream == NULL) {
        printf("Something went wrong opening the string buffer\n");
        return -1;
    }
    if (outstream == NULL) {
        printf("Something went wrong opening the index file\n");
        return -1;
    }
    
    copy_stream(bufstream, outstream); // error check this?

    fclose(bufstream);
    fclose(outstream);

    return 0;
}

Index*
read_index(void)
{
    Index index;
    Index* return_index;
    IndexItem index_item;
    FILE* index_file;
    int entry_count;

    index_file = fopen(INDEX_FILE, "rb");
    if (index_file == NULL) {
        printf("Cannot open index file\n");
        return NULL;
    }

    /* prob check if index file has right format and such */

    /* read the header */
    fseek(index_file, INDEX_HEADER_ENTRY_START, SEEK_SET);
    fread(&entry_count, INDEX_HEADER_ENTRY_LENGTH, 4, index_file);
    index.index_length = entry_count;

    /* for (int i = 0; i < entry_count; i++) { */

    /* } */

    /* make a copy of the index struct and return it */
    return_index = malloc(sizeof(Index));
    memcpy(return_index, &index, sizeof(Index));
    // may also need to memcpy the indexitem array inside

    return return_index;
}

Index*
write_index(Index* index)
{
    return NULL;
}

int
add_index_item(char* file_path)
{
    Index index;
    char* hashed_filename;
    Stat file_stat;
    IndexItem new_indexitem;

    /* read the old index */
    index = *(read_index());

    /* grab file stats */
    lstat(file_path, &file_stat); // error check!!

    /* build new indexitem struct */


    /* append to end of index_items in index struct */


    /* build the binary */


    /* write to index */


    /* write it to blobs */
    hashed_filename = write_blob(file_path); // error check!!

    /* clean up */
    free(index);

    return 0;

}
