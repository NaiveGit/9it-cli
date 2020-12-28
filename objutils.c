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
{ // TODO: make return clean up less cancer
    char* hexstring;
    char* out_path;
    FILE* tree_file;
    Tree child;

    /* NOTE THIS FUNCTION SHOULD BE CALLED ONLY
     * ON NODES OF TYPE TREE WITH AN NON ZERO 
     * NUMBER OF CHILDREN, ADD AN ASSERRTION */

    hexstring = hash_to_string(tree->hash);

    /* build out dir */
    out_path = malloc(strlen(OBJ_DIR)+strlen(hexstring)+1);
    memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(out_path, hexstring);

    /* check if tree exists */
    if (access(out_path, F_OK) == 0) { // already exists
        free(out_path);
        return hexstring;
    }

    /* write */
    tree_file = fopen(out_path, "wb"); 
    if (tree_file == NULL) {
        free(hexstring);
        free(out_path);
        perror(NULL);
        return NULL;
    }

    /* write the number of entries first */
    fwrite(&tree->cnum, sizeof(uint32_t), 1, tree_file);

    for (int c = 0; c < tree->cnum; c++) {

        child = tree->children[c];

        fwrite(&child.nodeType, NODETYPE_SIZE, 1, tree_file);        
        write_hash(tree_file, child.hash);
        fwrite(child.name, 1, strlen(child.name), tree_file);
        write_null(tree_file);
    }

    /* clean up */
    fclose(tree_file);
    free(out_path);

    return hexstring;
}

char*
write_commit(Commit* commit)
{ /* writes commit to objects  */
    
    char* hexstring;
    char* out_path;
    FILE* commit_file;

    hexstring = hash_to_string(commit->hash);

    /* build out dir - this is duplicate code, prob abstract */
    out_path = malloc(strlen(OBJ_DIR)+strlen(hexstring)+1);
    memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(out_path, hexstring);

    /* check if it already exists */
    if (access(out_path, F_OK) == 0) { // already exists
        free(out_path);
        return hexstring;
    }

    /* write */
    commit_file = fopen(out_path, "wb"); 
    if (commit_file == NULL) {
        free(hexstring);
        free(out_path);
        perror(NULL);
        return NULL;
    }

    /* write n shit */
    write_hash(commit_file, commit->hash);
    write_hash(commit_file, commit->root_tree->hash);

    fwrite(&commit->committer, 1, strlen(commit->committer), commit_file);
    fwrite(&commit->timestamp, sizeof(time_t), 1, commit_file);
    fwrite(&commit->msg, 1, strlen(commit->msg), commit_file);
    if (commit->parent_commit != NULL) { // not the first commit
        fwrite(&commit->parent_commit->hash, 1, strlen(commit->parent_commit->hash), commit_file);
    }

    /* clean up */
    fclose(commit_file);
    free(out_path);

    return hexstring;
}

char*
read_tree(unsigned char* tree_hash, Tree* root)
{
    /* NOTE, this function is not RECURSIVE */
    /* it is some else's responsibility to implement */

    char* hexstring;
    char* tree_path;
    FILE* tree_file;
    int entry_num;
    Tree* child_tree;
    char* hash;

    /* build out dir */
    hexstring = hash_to_string(tree_hash);
    tree_path = malloc(strlen(OBJ_DIR)+strlen(hexstring)+1);
    memcpy(tree_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(tree_path, hexstring);

    tree_file = fopen(tree_path, "rb");
    if (tree_file == NULL) {
        perror(NULL);
        return NULL;
    }
    
    /* get number of entries first */
    fread(&root->cnum, sizeof(uint32_t), 1, tree_file);

    root->hash = hexstring; 
    /* NAME IS NOT INITIALIZED BY THIS FUNCTION!! */
    root->nodeType = NodeType_tree;
    root->children = malloc((root->cnum)*sizeof(Tree));

    /* read from file */
    for (int i = 0; i < root->cnum; i ++) {

        /* create new children */
        child_tree = malloc(sizeof(Tree));

        fread(&child_tree->nodeType, NODETYPE_SIZE, 1, tree_file);

        hash = malloc(SHA_DIGEST_LENGTH);
        fread(hash, 1, SHA_DIGEST_LENGTH, tree_file);
        child_tree->hash = hash;

        child_tree->name = read_until_null(tree_file);

        child_tree->cnum = 0;
        child_tree->children = malloc(0);

        /* write to array */
        root->children[i] = *child_tree;

    }

    /* clean */
    fclose(tree_file);
    free(tree_path);

    return hexstring;

}

void
hash_tree(Tree* tree)
{ // careful not to pass empty tree, add assert
    
    int buf_size;
    FILE* temp_stream;

    buf_size = 0;
    for (int i = 0; i < tree->cnum; i++) {
        buf_size += NODETYPE_SIZE;    
        buf_size += SHA_DIGEST_LENGTH;
        buf_size += strlen(tree->children[i].name);
    }

    temp_stream = fmemopen(NULL, buf_size, "r+b");
    for (int i = 0; i < tree->cnum; i++) {
        Tree child_tree = tree->children[i];

        fwrite(&child_tree.nodeType, NODETYPE_SIZE, 1, temp_stream);
        write_hash(temp_stream, child_tree.hash);
        fwrite(&child_tree.name, 1, strlen(child_tree.name), temp_stream);

    }

    tree->hash = hash_stream(temp_stream);

    fclose(temp_stream);

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
    write_hash(index_file, hash);
    fwrite(file_path, 1, strlen(file_path), index_file);
    write_null(index_file); // index entry ends with null character

    /* clean up */
    /* free(&index); */
    /* also free index array */

    fclose(index_file);

    return 0;
}


