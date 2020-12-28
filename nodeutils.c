#include "headers/nodeutils.h"


void add_to(Tree* root, Tree* new_node, char* nextFolder,char* path);
int find_folder(Tree* root, char* path);
Tree* commit_tree(void);
void hash_all(Tree* root);
Tree* duplicate_tree(unsigned char* hash, char* name);
void delete(Tree* root, char* nextFolder, char* path);



Tree*
init_tree(Index* index)
{
    Tree* root;
    root = malloc(sizeof(Tree));
    root->nodeType = NodeType_tree;
    root->name = "";
    root->children = malloc(0);
    root->cnum = 0;

    struct stat sb;

    // Making the node
    Tree* new_node;

    // Loop thru the index and make folders n shit.
    
    for (int i = 0; i < index->index_length; i++) {
        IndexItem current = index->index_items[i];
        char *nextFolder = strtok(current.file_path,"/");
        char *path = malloc(strlen(current.file_path)*sizeof(char));

        // Node we're adding to tree
        new_node = malloc(sizeof(Tree));
        new_node->hash = current.hash;
        new_node->nodeType = NodeType_blob;
        new_node->name = current.file_path;
        new_node->children = malloc(0);
        new_node->cnum = 0;

        add_to(root, new_node, nextFolder,path);
        free(path);
    }

    return root;
}

// Commit the tree object
Tree*
commit_tree(void)
{
    //First check if head exists
    if (access(HEAD_FILE,R_OK)){
        // File exists
        Tree* root;
        root = duplicate_tree(get_head_commit(), "");
        // LOGIC

        // LOGIC
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
//   else if (root->nodeType == NodeType_blob) {
       // Dunno what to put here.
//   }
}

// Duplicates the tree
Tree*
duplicate_tree(unsigned char* hash, char* name)
{
    Tree* root;
    // Node we're adding to tree
    root = malloc(sizeof(Tree));
    root->hash = hash;
    root->name = name;
    read_tree(root);
    for (int i = 0; i < root->cnum; ++i) {
        if (root->children[i].nodeType == NodeType_tree) {
            duplicate_tree(root->children[i].hash, root->children[i].name);
        }
    }
    return root;
}

// For deleting files
void
delete(Tree* root, char* nextFolder, char* path)
{
    
}

// For adding files
void
add_to(Tree* root, Tree* new_node, char* nextFolder,char* path)
{
    if (nextFolder!=NULL) { // If still going to path 
        strcat(path,nextFolder);
        strcat(path,"/");
        // If folder doesn't exist, make it
        int folderpos = find_folder(root,path);
        nextFolder = strtok(NULL,"/");
        if (folderpos == -1) {
            // Make folder
            Tree* new_folder;
            new_folder = malloc(sizeof(Tree));
            new_folder->nodeType = NodeType_tree;
            new_folder->name = path;
            new_folder->children = malloc(0);
            new_folder->cnum = 0;

            // Add folder and recur
            root->cnum+=1;
            root->children = realloc(root->children,root->cnum);
            root->children[root->cnum-1] = *(new_folder);
            add_to(&(root->children[root->cnum-1]),new_node,nextFolder,path);
        }
        //Go into folder
        else {
            add_to(&root->children[folderpos],new_node,nextFolder,path);
        }
    }
    else {//We've reached the end, add the object file here. 
        root->cnum+=1;
        root->children = realloc(root->children,root->cnum);
        root->children[root->cnum-1] = *(new_node);//Set it equal
    }
}

int
find_folder(Tree* root, char* path)
{
    Tree* children = root->children;
    for (int i = 0; i<root->cnum; ++i) {
        if (children[i].name == path) {
            return i;
        }
    }
    return -1;
}

/*
// Clean folder: Run this after delete to remove empty folders.
int
clean_folders(Tree* root)
{
    //Return 1 if folder empty, return 0 if folder is not empty.
    if (root->cnum == 0 && root->nodeType == NodeType_tree) {
        return 1;
    }
    else if (root->nodeType == NodeType_blob) {
        return 0;
    }
    else { // If folder is not empty, keep on recurring
        for (int i = 0; i < root->cnum; ++i) {
            if 
        }
    }
}
*/