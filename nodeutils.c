Tree*
maketree(Index* index)
{
    Tree root;
    root.nodeType = tree;
    root.name = "Root folder lmao";
    root.children = malloc(0);
    root.cnum = 0;

    //Making the node
    Tree node;

    

    //Loop thru the index and make folders n shit.
    
    for (i = 0; i<index.index_length;++i){
        IndexItem current = index.index_items[i];
        char *ptr = strtok(current.file_path,"/");

        //Node we're adding to tree
        node.hash = current.hash;
        node.oType = blob;
        node.name = current.file_path;
        node.children = malloc(0);
        node.cnum = 0;

        addto(node,ptr,root);
    }

    return root;
}

void
addto(Tree node,char* ptr, Tree root)
{
    if (ptr!=NULL){//If still going to path
        




        ptr = strtok(NULL,"/");
    }
    else{//We've reached the end, add the object file here. 
        root.cnum+=1;
        root.children = realloc(root.children,root.cnum);
        root.children[root.cnum-1] = ;//Figure out what i use

    }
}


