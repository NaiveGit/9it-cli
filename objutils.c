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
    unsigned char* file_hash;
    char* hexstring; 
    char* out_path;

    /* open original file */
    file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Something went wrong opening %s\n", file_path);
        return NULL;
    }

    file_hash = hash_stream(file);
    hexstring = hash_to_string(file_hash);

    /* build the correct out dir */
    out_path = malloc(strlen(OBJ_DIR)+strlen(hexstring)+1);
    memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(out_path, hexstring);

    /* check to see if object already exists */
    if (access(out_path, F_OK) != 0) { // does not exist

        /* create the file */
        compress_file(file, out_path); // prob should error check this
    }

    /* clean up */
    free(out_path);
    fclose(file);

    return hexstring;
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
    if (bufstream == NULL) {
        perror(NULL);
        /* printf("Something went wrong opening the string buffer\n"); */
        return -1;
    }
    outstream = fopen(INDEX_FILE, "wb");
    if (outstream == NULL) {
        perror(NULL);
        /* printf("Something went wrong opening the index file\n"); */
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
    IndexItem* index_array;
    FILE* index_file;
    int entry_count;

    index_file = fopen(INDEX_FILE, "rb");
    if (index_file == NULL) { perror(NULL); return NULL; }

    /* prob check if index file has right format and such */
    /* perror("Index file may be corrupt"); */
    /* verify checksum and such */

    /* read the header */
    fseek(index_file, INDEX_HEADER_ENTRY_START, SEEK_SET);
    fread(&entry_count, INDEX_HEADER_ENTRY_LENGTH, 1, index_file);

    index_array = malloc(entry_count*sizeof(IndexItem));
    for (int i = 0; i < entry_count; i++) {
        IndexItem index_item;

        fread(&index_item.c_time, sizeof(time_t), 1, index_file);
        fread(&index_item.m_time, sizeof(time_t), 1, index_file);
        fread(&index_item.dev, sizeof(uint32_t), 1, index_file);
        fread(&index_item.ino, sizeof(uint32_t), 1, index_file);
        fread(&index_item.mode, sizeof(uint32_t), 1, index_file);
        fread(&index_item.uid, sizeof(uint32_t), 1, index_file);
        fread(&index_item.gid, sizeof(uint32_t), 1, index_file);
        fread(&index_item.file_size, sizeof(uint32_t), 1, index_file);

        unsigned char* hash = malloc(SHA_DIGEST_LENGTH*sizeof(unsigned char));        
        fread(hash, sizeof(unsigned char), SHA_DIGEST_LENGTH, index_file);
        index_item.hash = hash;

        index_item.file_path = read_until_null(index_file);       

        index_array[i] = index_item;
    }
    fclose(index_file);

    index.index_length = entry_count;
    index.index_items = index_array;
    /* make a copy of the index struct and return it */
    return_index = malloc(sizeof(Index));
    memcpy(return_index, &index, sizeof(Index));

    return return_index;
}

int
add_index_item(char* file_path)
{
    FILE* filepath_stream;
    unsigned char* hash;
    Stat file_stat;
    FILE* index_file;
    int index_header_entries;

    /* grab file stats */
    if (lstat(file_path, &file_stat) == -1) {
        perror(NULL);
        return -1; 
    }

    filepath_stream = fmemopen(file_path, strlen(file_path), "rb");
    hash = hash_stream(filepath_stream);

    /* build the binary */
    index_file = fopen(INDEX_FILE, "r+b");
    if (index_file == NULL) {
        perror(NULL);
        return -1;
    }

    /* increment number of index entries in header */
    fseek(index_file, INDEX_HEADER_ENTRY_START, SEEK_SET);
    fread(&index_header_entries, INDEX_HEADER_ENTRY_LENGTH, 1, index_file);
    index_header_entries += 1;
    fseek(index_file, INDEX_HEADER_ENTRY_START, SEEK_SET);
    fwrite(&index_header_entries, INDEX_HEADER_ENTRY_LENGTH, 1, index_file);
    
    /* append new binary to end of index */
    fseek(index_file, 0, SEEK_END);
    fwrite(&file_stat.st_ctime, sizeof(time_t), 1, index_file);
    fwrite(&file_stat.st_mtime, sizeof(time_t), 1, index_file);
    fwrite(&file_stat.st_dev, sizeof(uint32_t), 1, index_file);
    fwrite(&file_stat.st_ino, sizeof(uint32_t), 1, index_file);
    fwrite(&file_stat.st_mode, sizeof(uint32_t), 1, index_file);
    fwrite(&file_stat.st_uid, sizeof(uint32_t), 1, index_file);
    fwrite(&file_stat.st_gid, sizeof(uint32_t), 1, index_file);
    fwrite(&file_stat.st_size, sizeof(uint32_t), 1, index_file);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        fwrite(&hash[i], sizeof(unsigned char), 1, index_file);
    }
    fwrite(file_path, 1, strlen(file_path), index_file);

    char null = 0;
    fwrite(&null, 1, 1, index_file); // index entry ends with null character

    /* clean up */
    /* free(&index); */
    /* also free index array */

    fclose(index_file);

    return 0;
}


