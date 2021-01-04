#include "headers/objutils.h"
#include "headers/fileutils.h"
#include "headers/globals.h"

int was_file_deleted(char* file_path);

int
write_blob(char* file_path)
{
    FILE* file;
    unsigned char* file_hash;
    char* hexstring; 
    char* out_path;

    /* open original file */
    file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("write_blob > fopen");
        return -1;
    }

    file_hash = hash_stream(file);
    hexstring = hash_to_string(file_hash);
    free(file_hash);
    out_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
    free(hexstring);

    /* check to see if object already exists */
    if (access(out_path, F_OK) != 0) { // does not exist

        /* create the file */
        fseek(file, 0, SEEK_SET);
        /* write_to_file(out_path, BLOB_DEFAULT_HEADER, HEADER_LENGTH); */
        compress_file(file, out_path); // prob should error check this
    }

    /* clean up */
    free(out_path);
    fclose(file);

    return 0;
}

int
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
    out_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
    free(hexstring);

    /* check if tree exists */
    if (access(out_path, F_OK) == 0) { // already exists
        free(out_path);
        return 0;
    }

    /* write */
    tree_file = fopen(out_path, "wb"); 
    free(out_path);
    if (tree_file == NULL) {
        perror("write_tree > fopen");
        return -1;
    }

    /* write the number of entries first */
    fwrite(TREE_DEFAULT_HEADER, sizeof(char), HEADER_LENGTH, tree_file);
    fseek(tree_file, HEADER_ENTRY_NUM_START, SEEK_SET);
    fwrite(&tree->cnum, HEADER_ENTRY_NUM_LENGTH, 1, tree_file);

    for (int c = 0; c < tree->cnum; c++) {

        child = tree->children[c];

        fwrite(&child.nodeType, sizeof(NodeType), 1, tree_file);        
        write_hash(tree_file, child.hash);
        fwrite(child.name, 1, strlen(child.name), tree_file);
        write_null(tree_file);
    }

    /* clean up */
    fclose(tree_file);

    return 0;
}

int
write_commit(Commit* commit)
{ /* writes commit to objects  */
    
    char* hexstring;
    char* out_path;
    FILE* commit_file;

    hexstring = hash_to_string(commit->hash);
    out_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
    free(hexstring);

    /* check if it already exists */
    if (access(out_path, F_OK) == 0) { // already exists
        free(out_path);
        return 0;
    }

    /* write */
    commit_file = fopen(out_path, "wb"); 
    free(out_path);
    if (commit_file == NULL) {
        perror("write_commit > fopen");
        return -1;
    }

    /* write n shit */
    fwrite(COMMIT_DEFAULT_HEADER, sizeof(char), HEADER_LENGTH, commit_file);
    write_hash(commit_file, commit->root_tree_hash);

    fwrite(commit->committer, sizeof(char), strlen(commit->committer), commit_file);
    write_null(commit_file);
    fwrite(&commit->timestamp, sizeof(time_t), 1, commit_file);
    fwrite(commit->msg, sizeof(char), strlen(commit->msg), commit_file);
    write_null(commit_file);
    if (commit->parent_commit_hash != NULL) { // not the first commit
        write_hash(commit_file, commit->parent_commit_hash);
    }

    /* clean up */
    fclose(commit_file);

    return 0;
}

int
read_blob(unsigned char* hash, char* out_path)
{

    char* blob_path;
    char* hexstring;
    char* absolute_path;

    hexstring = hash_to_string(hash);
    blob_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
    free(hexstring);
    absolute_path = cat_str(2, get_repo_root(), out_path);
    
    uncompress_file(blob_path, absolute_path);

    free(blob_path);
    free(absolute_path);

    return 0;
}

int
read_tree(Tree* root)
{
    /* NOTE, this function is not RECURSIVE */
    /* it is some else's responsibility to implement */

    char* hexstring;
    char* tree_path;
    FILE* tree_file;
    Tree* children_arr;
    Tree* child_tree;
    char* hash;

    /* build out dir */
    hexstring = hash_to_string(root->hash);
    tree_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
    free(hexstring);

    tree_file = fopen(tree_path, "rb");
    free(tree_path);
    if (tree_file == NULL) {
        perror("read_tree > fopen");
        return -1;
    }
    
    /* get number of entries first */
    fseek(tree_file, HEADER_ENTRY_NUM_START, SEEK_SET);
    fread(&root->cnum, HEADER_ENTRY_NUM_LENGTH, 1, tree_file);
    /* ROOT HASH IS NOT INITIALIZED!! */
    /* NAME IS NOT INITIALIZED BY THIS FUNCTION!! */
    root->nodeType = NodeType_tree;
    root->children = malloc((root->cnum)*sizeof(Tree));

    /* read from file */
    for (int i = 0; i < root->cnum; i ++) {

        /* create new children */
        child_tree = malloc(sizeof(Tree));

        fread(&child_tree->nodeType, sizeof(NodeType), 1, tree_file);

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

    return 0;

}

int
read_commit(Commit* commit)
{
    char* hexstring;
    char* out_path;
    FILE* commit_file;
    unsigned char* root_tree_hash;
    unsigned char* parent_commit_hash;

    hexstring = hash_to_string(commit->hash);
    out_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
    free(hexstring);

    commit_file = fopen(out_path, "rb");
    free(out_path);
    if (commit_file == NULL) {
        perror("read_commit > fopen");
        return -1;
    }

    /* skip header */
    fseek(commit_file, HEADER_LENGTH, SEEK_SET);

    root_tree_hash = malloc(SHA_DIGEST_LENGTH);
    fread(root_tree_hash, sizeof(unsigned char), SHA_DIGEST_LENGTH, commit_file);
    commit->root_tree_hash = root_tree_hash;

    commit->committer = read_until_null(commit_file);
    fread(&commit->timestamp, sizeof(time_t), 1, commit_file);
    commit->msg = read_until_null(commit_file);

    /* check if there is any more to read */
    parent_commit_hash = malloc(SHA_DIGEST_LENGTH);
    if (fread(parent_commit_hash, sizeof(unsigned char), SHA_DIGEST_LENGTH, commit_file) < SHA_DIGEST_LENGTH) {
        free(parent_commit_hash);
        commit->parent_commit_hash = NULL; 
    } else {
        commit->parent_commit_hash = parent_commit_hash;
    }

    fclose(commit_file);

    return 0;
}

int
hash_tree(Tree* tree)
{ 
    
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
        fwrite(&child_tree.name, sizeof(char), strlen(child_tree.name), temp_stream);

    }

    fseek(temp_stream, 0, SEEK_SET);
    tree->hash = hash_stream(temp_stream);

    fclose(temp_stream);

    return 0;

}

int
hash_commit(Commit* commit)
{
    int buf_size;
    FILE* temp_stream;
    
    buf_size = 
        SHA_DIGEST_LENGTH // tree hash
        + strlen(commit->committer)
        + sizeof(time_t)
        + strlen(commit->msg)
        + SHA_DIGEST_LENGTH; // previous commit hash

    temp_stream = fmemopen(NULL, buf_size, "r+b");

    write_hash(temp_stream, commit->root_tree_hash);
    fwrite(&commit->committer, 1, strlen(commit->committer), temp_stream);
    fwrite(&commit->timestamp, sizeof(time_t), 1, temp_stream);
    fwrite(&commit->msg, 1, strlen(commit->msg), temp_stream);
    if (commit->parent_commit_hash != NULL) { // only if its not first commit
        write_hash(temp_stream, commit->parent_commit_hash);
    }

    fseek(temp_stream, 0, SEEK_SET);
    commit->hash = hash_stream(temp_stream);

    fclose(temp_stream);

    return 0;
}

int
add_index_item(char* file_path)
{
    char* absolute_path;
    char* out_path;
    FILE* index_file;
    FILE* file_stream;
    int index_header_entries;
    unsigned char* hash;
    Stat file_stat;
    
    /* build the binary */
    out_path = cat_str(2, get_dot_dir(), INDEX_FILE);
    index_file = fopen(out_path, "r+b");
    if (index_file == NULL) {
        perror("add_index_item > fopen");
        return -1;
    }
    free(out_path);

    /* increment number of index entries in header */
    fseek(index_file, HEADER_ENTRY_NUM_START, SEEK_SET);
    fread(&index_header_entries, HEADER_ENTRY_NUM_LENGTH, 1, index_file);
    index_header_entries += 1;
    fseek(index_file, HEADER_ENTRY_NUM_START, SEEK_SET);
    fwrite(&index_header_entries, HEADER_ENTRY_NUM_LENGTH, 1, index_file);

    /* append new binary to end of index */
    fseek(index_file, 0, SEEK_END);

    if (was_file_deleted(file_path) == 0) {
        /* grab file stats */
        absolute_path = rcat_str(2, get_repo_root(), file_path);

        if (lstat(absolute_path, &file_stat) == -1) {
            perror("add_index_item > lstat");
            return -1; 
        }

        file_stream = fopen(file_path, "rb");
        hash = hash_stream(file_stream);
        fclose(file_stream);
        
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

        free(absolute_path);
        free(hash);

    } else {
        
        int nullblock = 2*sizeof(time_t)+6*sizeof(uint32_t);
        for (int i = 0; i < nullblock; i++) {
            write_null(index_file);
        }
        write_hash(index_file, DEL_HASH);
        fwrite(file_path, 1, strlen(file_path), index_file);
        write_null(index_file); // index entry ends with null character
        
    }

    fclose(index_file);

    return 0;
}

int
was_file_deleted(char* file_path)
{
    unsigned char* latest_commit_hash;
    char** previous_files;
    char* previous_file;
    int ind;

    /* check to see if file was deleted */
    /* latest_commit_hash = get_head_commit(); */
    /* previous_files = list_all_objects(latest_commit_hash); */
    /* free(latest_commit_hash); */

    /* ind = 0; */
    /* while ((previous_file = previous_files[ind]) != 0) { */
        
    /*     if (strcmp(previous_file, file_path) == 0) { */
    /*         return 1; */
    /*     } */

    /*     ind += 1; */             
    /* } */

    /* return 0; */
    return 1;

}

int // take in path relative to repo root
add_index_dir(char* dir_path)
{  // maybe check that dir_path is an acc dir
    char* absolute_path;
    DIR* dir;
    Dirent* dirent;
    char* relative_child_path;

    absolute_path = get_repo_root();
    absolute_path = realloc(absolute_path, strlen(absolute_path)+strlen(dir_path)+1);
    strcat(absolute_path, dir_path);
    
    /* prob need to handle dir_path = "", convert to "." */
    dir = opendir(absolute_path);
    if (dir == NULL) {
        perror(NULL);
        return -1; // need to clean up if add_dir fails halfway
    }
    free(absolute_path);

    while ((dirent = readdir(dir)) != NULL) {

        /* ignore . and .. and .9it */
        if (strcmp(dirent->d_name, ".9it") == 0) continue; 
        if (strcmp(dirent->d_name, "..") == 0) continue; 
        if (strcmp(dirent->d_name, ".") == 0) continue; 

        /* add files */
        relative_child_path = cat_str(2, dir_path, dirent->d_name);

        if (dirent->d_type == DT_DIR) { // recurse further
            /* append a slash */
            rcat_str(2, relative_child_path, "/");
            add_index_dir(relative_child_path);

        } else if (dirent->d_type == DT_REG) { // its a normal folder, add it
            add_index_item(relative_child_path);
            
        } // not handling smlinks and other file types

        free(relative_child_path);
    }

    closedir(dir);

    return 0;
}

int
clear_index(void)
{
    char* index_file = cat_str(2, get_dot_dir(), INDEX_FILE);
    if (write_to_file(index_file, INDEX_DEFAULT_HEADER, HEADER_LENGTH) == -1) {
        return -1;
    }
    free(index_file);

    return 0;
}

Index*
read_index(void)
{
    Index index;
    Index* return_index;
    char* index_path;
    IndexItem* index_array;
    FILE* index_file;
    int entry_count;

    index_path = cat_str(2, get_dot_dir(), INDEX_FILE);
    index_file = fopen(index_path, "rb");
    free(index_path);
    if (index_file == NULL) { perror(NULL); return NULL; }

    /* prob check if index file has right format and such */
    /* perror("Index file may be corrupt"); */
    /* verify checksum and such */

    /* read the header */
    fseek(index_file, HEADER_ENTRY_NUM_START, SEEK_SET);
    fread(&entry_count, HEADER_ENTRY_NUM_LENGTH, 1, index_file);

    index_array = malloc(entry_count*sizeof(IndexItem));
    for (int i = 0; i < entry_count; i++) {
        IndexItem* index_item;
        index_item = malloc(sizeof(IndexItem));

        fread(&index_item->c_time, sizeof(time_t), 1, index_file);
        fread(&index_item->m_time, sizeof(time_t), 1, index_file);
        fread(&index_item->dev, sizeof(uint32_t), 1, index_file);
        fread(&index_item->ino, sizeof(uint32_t), 1, index_file);
        fread(&index_item->mode, sizeof(uint32_t), 1, index_file);
        fread(&index_item->uid, sizeof(uint32_t), 1, index_file);
        fread(&index_item->gid, sizeof(uint32_t), 1, index_file);
        fread(&index_item->file_size, sizeof(uint32_t), 1, index_file);

        unsigned char* hash = malloc(SHA_DIGEST_LENGTH*sizeof(unsigned char));        
        fread(hash, sizeof(unsigned char), SHA_DIGEST_LENGTH, index_file);
        index_item->hash = hash;

        index_item->file_path = read_until_null(index_file);       

        index_array[i] = *index_item;
    }
    fclose(index_file);

    index.index_length = entry_count;
    index.index_items = index_array;
    /* make a copy of the index struct and return it */
    return_index = malloc(sizeof(Index));
    if (return_index == NULL) {
        printf("Malloc failed \n");
    }
    memcpy(return_index, &index, sizeof(Index));

    return return_index;
}

unsigned char*
get_head_commit(void)
{
    return read_ref(get_cur_branch());
}

char*
get_cur_branch(void)
{
    char* out_path;
    FILE* head_file;
    char* cur_branch;

    out_path = cat_str(2, get_dot_dir(), HEAD_FILE);
    head_file = fopen(out_path, "rb");
    if (head_file == NULL) {
        perror(NULL);
        return NULL;
    }
    free(out_path);

    cur_branch = read_until_null(head_file);
    fclose(head_file);
    
    return cur_branch;
}

unsigned char*
read_ref(char* branch_name)
{
    FILE* ref_file;
    char* ref_path;
    unsigned char* hash;

    ref_path = cat_str(3, get_dot_dir(), HEADS_DIR, branch_name);
    ref_file = fopen(ref_path, "rb");
    if (ref_file == NULL) { // no commits
        return NULL;
    }

    hash = malloc(SHA_DIGEST_LENGTH);
    fread(hash, sizeof(unsigned char), SHA_DIGEST_LENGTH, ref_file);

    free(ref_path);
    fclose(ref_file);
   
    return hash; 
}

int
write_ref(char* branch_name, unsigned char* hash)
{ // creates the file if it's not already created
     
    FILE* ref_file;
    char* ref_path;

    ref_path = cat_str(3, get_dot_dir(), HEADS_DIR, branch_name);
    ref_file = fopen(ref_path, "wb");
    if (ref_file == NULL) { // no commits
        return -1;
    }
    
    write_hash(ref_file, hash);

    fclose(ref_file);
    free(ref_path);

    return 0;
}


ObjType
obj_type(char* obj_path)
{
    return ObjType_undefined;
}

void
log_tree(char* obj_path)
{

}

void
log_commit(char* obj_path)
{

}
