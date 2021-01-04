#include "headers/nodeutils.h"

// Main functions:
Tree* commit_tree(void);
void add_to(Tree* root, IndexItem current);
void add_to_helper(Tree* root, Tree* new_node, char* nextFolder,char* path);
void delete_to(Tree* root, IndexItem current);
void delete_to_helper(Tree* root, char* nextFolder, char* path);
void delete(Tree* root,int num);
void revert_commit(unsigned char* t_hash);

// Making the Tree:
Tree* init_tree(Tree* root);
void write_full_tree(Tree* root);
Tree* duplicate_tree(unsigned char* hash, char* name, Tree* root);
void hash_all(Tree* root);

// Status
char** compare_index_commit(void);
char** compare_index_working(void);
char** get_untracked(void);

// Helper functions:
int find_child(Tree* root, char* path);
int file_exists(char* path);
void print_tree(Tree* root);
void erase_tree(Tree* root);
void repopulate(Tree* root);
void print_whole_tree(Tree* root);
int clean_folders(Tree* root);
char** list_all_objects(unsigned char* commit_hash);
void list_all_objects_helper(Tree* root,char** objects, int* size);
Commit* get_previous_commit(char* recent_commit_ptr);

// Commit the tree object
// What if the commit is EMPTY?? 
Tree*
commit_tree(void)
{
    Tree* root; 
    root = malloc(sizeof(Tree)); 
    //First check if head exists
    unsigned char* recent_commit_ptr = get_head_commit();
    if (NULL != recent_commit_ptr){
        // File exists
        printf("Commiting with an existing commit! \n");
        Commit* c;
        c = get_previous_commit(recent_commit_ptr);
        duplicate_tree(c->root_tree_hash, "", root);
        // When adding, there is a possibility that the hash will be different. In that case, 
        // Find it, and perform logic to check if hash is the same.
        // Don't worry about folders because those will be rehashed. 
        // If the file does not exist irl, then delete it.

    }
    else {
        init_tree(root);
        /*
        Index index;
        index.index_length = 3;
        IndexItem* items = malloc(3*sizeof(IndexItem));
        IndexItem item1;
        item1.hash = "1";
        item1.file_path = "one/two/file1.txt";
        IndexItem item2;
        item2.hash = "2";
        item2.file_path = "one/two/file2.txt";

        IndexItem item3;
        item3.hash = "3";
        item3.file_path = "three/another.txt";
        items[0] = item1;
        items[1] = item2;
        items[2] = item3;
        index.index_items = items;
        root = init_tree(&index);
        */
    }
    Index* index;
    index = read_index();
    // LOGIC
    // Loop Thru index. Check if file exists irl. If it does, try adding it.
    for (int i = 0; i < index->index_length; i++) {
        IndexItem current = index->index_items[i];
        printf("Current Index: Name: %s, Hash: %s \n",current.file_path,hash_to_string(current.hash)); 
        if (memcmp(DEL_HASH,current.hash,SHA_DIGEST_LENGTH) == 0) {
            delete_to(root,current);
        }
        else {
            add_to(root,current);
        }
    }
    clean_folders(root);
    hash_all(root);
    write_full_tree(root);
    free(recent_commit_ptr);
    return root;
}

void revert_commit(unsigned char* t_hash) {
    printf("Commence reverting commit\n");
    Tree* tree_to_delete;
    tree_to_delete = malloc(sizeof(Tree));
    unsigned char* recent_commit_ptr = get_head_commit();
    if (NULL != recent_commit_ptr){
        // File exists
        Commit* c = malloc(sizeof(Commit));
        c->hash = recent_commit_ptr;
        read_commit(c);
        // Commit* curcommit = read_commit(recent_commit_ptr);
        duplicate_tree(c->root_tree_hash, "", tree_to_delete);
    }
    erase_tree(tree_to_delete);
    Tree* root;
    root = malloc(sizeof(Tree));
    duplicate_tree(t_hash,"",root);

    printf("Let there be life initiate\n");
    repopulate(root);  
}

// For adding files
void
add_to(Tree* root, IndexItem current)
{
    char* filep = malloc((strlen(current.file_path) + 2)*sizeof(char));
    strcpy(filep,current.file_path);
    char* path = malloc((strlen(current.file_path) + 2)*sizeof(char));
    path[0] = 0; 
    char* nextFolder = strtok(filep,"/");
    // Making the node
    Tree* new_node;
    new_node = malloc(sizeof(Tree));
    new_node->hash = current.hash;
    new_node->nodeType = NodeType_blob;
    new_node->name = current.file_path;
    new_node->children = malloc(sizeof(Tree));
    new_node->cnum = 0;
    add_to_helper(root, new_node, nextFolder,path);
    free(path);
}

void
add_to_helper(Tree* root, Tree* new_node, char* nextFolder,char* path)
{
    strcat(path,nextFolder);
    if (strcmp(path,new_node->name) != 0) { // If still going to path 
        strcat(path,"/");
        // If folder doesn't exist, make it
        int folderpos = find_child(root,path);
        nextFolder = strtok(NULL,"/");
        if (folderpos == -1) {
            // Make folder
            Tree* new_folder = malloc(sizeof(Tree));
            new_folder->nodeType = NodeType_tree;
            char* fpath = malloc((strlen(path)+1)*sizeof(char));
            strcpy(fpath,path);
            new_folder->name = fpath;
            new_folder->children = malloc(0);
            new_folder->cnum = 0;
            // Add folder and recur
            root->cnum+=1;
            root->children = realloc(root->children,root->cnum*sizeof(Tree));
            root->children[root->cnum-1] = *(new_folder);
            
            add_to_helper(&root->children[root->cnum-1],new_node,nextFolder,path);
        }
        //Go into folder
        else {
            add_to_helper(&root->children[folderpos],new_node,nextFolder,path);
        }
    }
    else {//We've reached the end, add the object file here. 
        int objectpos = find_child(root,path);
        if (objectpos == -1) {
            printf("Object inserted for the first time! Path: %s\n",path);
            root->cnum+=1;
            root->children = realloc(root->children,root->cnum*sizeof(Tree));
            root->children[root->cnum-1] = *(new_node);//Set it equal
        }
        else { // If the object does exist, check the hashes.
            if (strcmp(new_node->hash, root->children[objectpos].hash) != 0) {
                printf("Object is in tree, but is updated! \n");
                root->children[objectpos] = *new_node;
            }
            else {
                printf("Object has not changed, not changing tree.\n");
                free(new_node);
            }
        }
   }
}

void
delete_to(Tree* root, IndexItem current)
{
    char* filep = malloc((strlen(current.file_path) + 2)*sizeof(char));
    strcpy(filep,current.file_path);
    char* path = malloc((strlen(current.file_path) + 2)*sizeof(char));
    path[0] = 0; 
    char* nextFolder = strtok(filep,"/");
    delete_to_helper(root,nextFolder,path);
    free(path);
}

// For deleting files
void
delete_to_helper(Tree* root, char* nextFolder, char* path)
{
    strcat(path,nextFolder);
    nextFolder = strtok(NULL,"/");
    if (nextFolder != NULL) {
        strcat(path,"/");
    }
    int pos = find_child(root,path);
    if (pos == -1) { // Folder or file doesn't exist, means nothing will happen
        printf("File/Folder doesn't exist so deleted.\n");
    }
    else {
        // If it's a blob, then disconnect.
        if (root->children[pos].nodeType == NodeType_blob) {
            printf("Disconnecting node.\n");
            delete(root,pos);
        }
        // Otherwise, continue traversing the tree.
        else {
            printf("Traversing tree to delete\n");
            delete_to_helper(&root->children[pos],nextFolder,path);
        }
    }
}

void
delete(Tree* root, int num)
{
    for (int i = num; i < (root->cnum-1); i++) {
        root->children[i] = root->children[i+1];
    }
    root->children = realloc(root->children,(root->cnum-1)*sizeof(Tree));
    root->cnum-=1;
}

void
erase_tree(Tree* root)
{
    if (root->nodeType == NodeType_blob) {
       char* filepath;
       filepath = get_repo_root();
       filepath = realloc(filepath,(strlen(filepath)+strlen(root->name)+1));
       strcat(filepath,root->name);
       remove(filepath); 
       printf("Removing file: %s\n",filepath);
    }
    else {
        for (int i = 0; i< root->cnum; i++) {
            erase_tree(&root->children[i]);
        } 
    }
}

void
repopulate(Tree* root)
{
    if (root->nodeType == NodeType_blob) {
       printf("Adding file: %s\n",root->name);
       read_blob(root->hash,root->name);
    }
    else {
        for (int i = 0; i < root->cnum; i++) {
            repopulate(&root->children[i]);
        }
    }
}

Tree*
init_tree(Tree* root)
{
    printf("Init Tree \n");
    root->nodeType = NodeType_tree;
    root->name = "";
    root->children = malloc(sizeof(Tree));
    root->cnum = 0;
    return root;
}

void 
write_full_tree(Tree* root) {
    write_tree(root);
    for (int i = 0; i < root->cnum; i++) {
        if (root->children[i].nodeType == NodeType_blob) {
            write_blob(root->children[i].name);
        } else {
            write_full_tree(&root->children[i]);
        }
    }
}

// Hash all tree
void
hash_all(Tree* root)
{
   if (root->nodeType == NodeType_tree) { // Note: Does hash all work with an empty folder?
       for (int i = 0; i < root->cnum; ++i) {
           hash_all(&root->children[i]);
       }
       hash_tree(root);
   } 
}

// Status
char** 
compare_index_commit(void)
{
    Tree* root; 
    root = malloc(sizeof(Tree)); 
    root->cnum = 0;
    //First check if head exists
    unsigned char* recent_commit_ptr = get_head_commit();
    if (NULL != recent_commit_ptr){
        // File exists
        printf("Comparing index with an existing commit! \n");
        Commit* c;
        c = get_previous_commit(recent_commit_ptr);
        duplicate_tree(c->root_tree_hash, "", root);
    }
    else {
        printf("There is no previous commit, so list everything in index.\n");
    }
    Index* index;
    index = read_index();
    for (int i = 0; i < index->index_length; i++) {
        IndexItem current = index->index_items[i];
        printf("Current Index: Name: %s, Hash: %s \n",current.file_path,hash_to_string(current.hash)); 
        if (strcmp(DEL_HASH,current.hash) == 0) {
            printf("delete check\n");
        }
        else {
            printf("add check\n");
        }
    }
}
char** 
compare_index_working(void)
{

}
char** 
get_untracked(void)
{

}


// Duplicates the tree
Tree*
duplicate_tree(unsigned char* hash, char* name, Tree* root)
{
    root->hash = hash;
    root->name = name;
    read_tree(root);
    for (int i = 0; i < root->cnum; ++i) {
        if (root->children[i].nodeType == NodeType_tree) {
            duplicate_tree(root->children[i].hash, root->children[i].name, &root->children[i]);
        }
    }
    return root;
}

void
print_tree(Tree* root)
{
    char* hash = hash_to_string(root->hash);
    printf("Hash: %s \n Name: %s \n Nodetype: %d \n Num of children: %d \n",hash,root->name,root->nodeType,root->cnum);
    free(hash);
}

void
print_whole_tree(Tree* root)
{
    print_tree(root);
    printf("Inside of it:\n");
    for (int i = 0; i < root->cnum; i++) {
        print_whole_tree(&root->children[i]);
    }
    printf("End of Node \n\n");
}

int
file_exists(char* path)
{
    struct stat sb;
    char* filepath;
    filepath = get_repo_root();
    filepath = realloc(filepath,(strlen(filepath)+strlen(path)+1));
    strcat(filepath,path);
    printf("This is the filepath: %s \n",filepath);
    if (stat(filepath,&sb) == 0) { // File exists
        return 1;
    }
    return 0;
}

int
find_child(Tree* root, char* path)
{
    Tree* children = root->children;
    for (int i = 0; i<root->cnum; ++i) {
        if (strcmp(children[i].name,path) == 0) {
            return i;
        }
    }
    return -1;
}

// Clean folder: Run this after delete to remove empty folders.
int
clean_folders(Tree* root)
{
    //Return 0 if folder empty, 
    if (root->cnum == 0 && root->nodeType == NodeType_tree) {
        return 0;
    }
    // return 1 if folder is not empty.
    else if (root->nodeType == NodeType_blob) {
        return 1;
    }
    else { // If folder is not empty, keep on recurring
        for (int i = 0; i < root->cnum; ++i) {
            if (clean_folders(&root->children[i]) != 1) { // If folder is empty 
                delete(root,i);
                i-=1; // For folder
            }  
        }
        //If the root folder is empty, then its empty.
        if (root->cnum == 0) {
            return 0;
        }
        return 1;
    }
}

Commit*
get_previous_commit(char* recent_commit_ptr)
{
    Commit* c = malloc(sizeof(Commit));
    c->hash = recent_commit_ptr;
    read_commit(c);
    return c;
}


//List All the objects in the tree
// Imma need to get size. Also, i should also have a int pointer. 
char**
list_all_objects(unsigned char* commit_hash)
{
    char** objects;
    objects = malloc(0);
    int num = 0;
    int* size = &num;
    if (NULL != commit_hash) {
        Commit* c = get_previous_commit(commit_hash);
        Tree* root;
        root = malloc(sizeof(Tree));
        duplicate_tree(c->root_tree_hash,"",root);
        list_all_objects_helper(root,objects,size);
    }
    objects = realloc(objects,((*size)+1)*sizeof(char*));
    objects[*size] = 0;
    *size+=1;
    return objects;
}

void
list_all_objects_helper(Tree* root,char** objects, int* size) {
    if (root->nodeType == NodeType_blob) {
        objects = realloc(objects,((*size)+1)*sizeof(char*));
        objects[*size] = root->name;
        *size +=1;
    }
    else {
        for (int i = 0; i < root->cnum; i++) {
            list_all_objects_helper(&root->children[i],objects,size);
        }
    }

}
