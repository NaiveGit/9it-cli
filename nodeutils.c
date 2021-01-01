#include "headers/nodeutils.h"

void add_to(Tree* root, Tree* new_node, char* nextFolder,char* path);
int find_folder(Tree* root, char* path);
Tree* commit_tree(void);
void print_tree(Tree* root);
void hash_all(Tree* root);
Tree* duplicate_tree(unsigned char* hash, char* name);
void delete(Tree* root, char* nextFolder, char* path);
void write_full_tree(Tree* root);

Tree*
init_tree(Index* index)
{
    printf("Init Tree \n");
    Tree* root;
    root = malloc(sizeof(Tree));
    root->nodeType = NodeType_tree;
    root->name = "";
    root->children = malloc(sizeof(Tree));
    root->cnum = 0;
    struct stat sb;

    // Loop thru the index and make folders n shit.
    printf("Loop Index: \n"); 
    for (int i = 0; i < index->index_length; i++) {
        printf("Looping at: %d, currenty inserting: %s \n",i,index->index_items[i].file_path);
        IndexItem current = index->index_items[i];
        char* filep = malloc((strlen(current.file_path) + 2)*sizeof(char));
        strcpy(filep,current.file_path);
        char* path = malloc((strlen(current.file_path) + 2)*sizeof(char));
        path[0] = 0; 

        char* nextFolder = strtok(filep,"/");

        // Node we're adding to tree
        // Making the node
        Tree* new_node;
        new_node = malloc(sizeof(Tree));
        new_node->hash = current.hash;
        new_node->nodeType = NodeType_blob;
        new_node->name = current.file_path;
        new_node->children = malloc(sizeof(Tree));
        new_node->cnum = 0;
        add_to(root, new_node, nextFolder,path);
        free(path);
    }

    return root;
}
// For adding files
void
add_to(Tree* root, Tree* new_node, char* nextFolder,char* path)
{
    strcat(path,nextFolder);
    printf("Path: %s, and the name: %s \n",path,new_node->name);
    if (strcmp(path,new_node->name) != 0) { // If still going to path 
        strcat(path,"/");
        // If folder doesn't exist, make it
        int folderpos = find_folder(root,path);
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
            // print_tree(new_folder);
            // Add folder and recur
            root->cnum+=1;
            root->children = realloc(root->children,root->cnum*sizeof(Tree));
            root->children[root->cnum-1] = *(new_folder);
            
            add_to(&root->children[root->cnum-1],new_node,nextFolder,path);
        }
        //Go into folder
        else {
            printf("Go into folder postion %d \n",folderpos);
            add_to(&root->children[folderpos],new_node,nextFolder,path);
        }
    }
    else {//We've reached the end, add the object file here. 
        printf("OBJECT INSERTING \n");
        root->cnum+=1;
        root->children = realloc(root->children,root->cnum*sizeof(Tree));
        root->children[root->cnum-1] = *(new_node);//Set it equal
    }
}

void
print_tree(Tree* root)
{
    printf("Hash: %s \n Name: %s \n Nodetype: %d \n Num of children: %d \n",hash_to_string(root->hash),root->name,root->nodeType,root->cnum);
}

// Commit the tree object
Tree*
commit_tree(void)
{
   Tree* root; 
   printf("Hello \n");
    
    //First check if head exists
    unsigned char* recent_commit = get_head_commit();
    if (NULL != recent_commit){
        // File exists
        Tree* root;
        // root = duplicate_tree(recent_commit, "");

        // LOGIC
    }
    else {
        printf("Making the tree for the first tiem! \n");
        printf("Index length: %d \n",read_index()->index_length);
        root =  init_tree(read_index());
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
    printf("Begin hashing! \n");
    hash_all(root);
    printf("Begin writing the tree! \n");
    write_full_tree(root);
}

int
find_folder(Tree* root, char* path)
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
    //Return 1 if folder empty, return 0 if folder is not empty.
    if (root->cnum == 0 && root->nodeType == NodeType_tree) {
        return 1;
    }
    else if (root->nodeType == NodeType_blob) {
        return 0;
    }
    else { // If folder is not empty, keep on recurring
        for (int i = 0; i < root->cnum; ++i) {
            //if  
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

void 
write_full_tree(Tree* root) {
    write_tree(root);
    for (int i = 0; i < root->cnum; i++) {
        write_full_tree(&root->children[i]);
    }
}



