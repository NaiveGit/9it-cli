#include "headers/nodeutils.h"


void add_to(Tree* root, Tree* new_node, char* nextFolder,char* path);
int find_folder(Tree* root, char* path);

Tree*
make_tree(Index* index)
{
    Tree* root;
    root = malloc(sizeof(Tree));
    root->nodeType = NodeType_tree;
    root->name = "Root folder lmao";
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
            add_to(&root->children[root->cnum-1],new_node,nextFolder,path);


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

