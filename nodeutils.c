#include "headers/nodeutils.h"


void add_to(Tree node, char* ptr, Tree* root);

Tree*
make_tree(Index* index)
{
    Tree root;
    root.nodeType = tree;
    root.name = "Root folder lmao";
    root.children = malloc(0);
    root.cnum = 0;

    // Making the node
    Tree new_node;

    

    // Loop thru the index and make folders n shit.
    
    for (i = 0; i<index.index_length;++i){
        IndexItem current = index.index_items[i];
        char *ptr = strtok(current.file_path,"/");

        // Node we're adding to tree
        new_node = *(malloc(sizeof(Tree)));
        new_node.hash = current.hash;
        new_node.oType = blob;
        new_node.name = current.file_path;
        new_node.children = malloc(0);
        new_node.cnum = 0;

        add_to(new_node, ptr, &root);
    }

    return root;
}

void
add_to(Tree node, char* ptr, Tree* root)
{
    if (ptr!=NULL){ // If still going to path
        // Check if folder exists. If it does, then go in.

        // Otherwise, make the folder and go in




        ptr = strtok(NULL,"/");
    }
    else{//We've reached the end, add the object file here. 
        root->cnum+=1;
        root->children = realloc(root->children,root->cnum);
        root->children[root->cnum-1] = &node;//Set it equal

    }
}
/*
int
find_folder(,Tree root)
*/