#include "headers/fileutils.h"
#include "headers/objutils.h"
#include "headers/globals.h"
#include "headers/commands.h"
#include "headers/nodeutils.h"

int 
init(char* root) 
{
    char* path;
    int ind;
    char* dirs[] = {
        "obj/",
        "temp/",
        "refs/tags/",
        "refs/heads/",
        0
    };

    ind = 0;
    while ((path = dirs[ind]) != 0) {

        if (mkfolder(root, path) == -1) {
            // prob also clean up half created dir
            return -1;
        }
         
        ind++;
    }

    /* error check these ma nibba */
    char* index_file = cat_str(2, root, INDEX_FILE);
    char* head_file = cat_str(2, root, HEAD_FILE);
    char* ident_file = cat_str(2, root, IDENT_FILE);
    char* branch_file = cat_str(3, root, HEADS_DIR, HEAD_DEFAULT);

    write_to_file(index_file, INDEX_DEFAULT_HEADER, HEADER_LENGTH); //index file
    write_to_file(head_file, HEAD_DEFAULT, strlen(HEAD_DEFAULT)+1);
    write_to_file(ident_file, "", 0);
    write_to_file(branch_file, "", 0);

    free(index_file);
    free(head_file);
    free(ident_file);
    free(branch_file);

    printf("Successfully initialized 9it in current working directory.\n");

    return 0;
}

int
add(char* local_path)
{ // need to ignore .9it folder and handle .

    char* relative_path;
    Stat file_stat;
    int is_dir;

    /* make local_path relative to repo root */
    relative_path = get_local_path();
    relative_path = rcat_str(2, relative_path, local_path);

    /* check if dir or file */
    if (lstat(local_path, &file_stat) == -1) {
        is_dir = 0;
    } else {
        is_dir = !S_ISDIR(file_stat.st_mode) == 0;
    }

    if (is_dir) { 
        /* append a slash if it's not there */
        if (strend(relative_path, "/") == 0) {
            rcat_str(2, relative_path, "/");
        } 

        /* handle . */
        if (strcmp(relative_path, "./") == 0) {
            relative_path = realloc(relative_path, 1);
            relative_path[0] = 0;
        }

        add_index_dir(relative_path);

    } else {

        add_index_item(relative_path);

    }
     
    free(relative_path);

    return 0;
}

int 
add_update(void)
{
    unsigned char* latest_commit;
    char** tracked;
    char* tracked_file;
    int ind;

    latest_commit = get_head_commit();
    tracked = list_all_objects(latest_commit);
    free(latest_commit);

    ind = 0;
    while ((tracked_file = tracked[ind]) != 0) {

        add(tracked_file);        

        ind += 1;
    }

    free(tracked); // also need to free all the elements but whateve 

    return 0;
}

int
branch(char* branch_name)
{ // create new branch
    char* new_branch_path;
    unsigned char* head_commit;

    new_branch_path = cat_str(3, get_dot_dir(), HEADS_DIR, branch_name);
    /* check to see if branch was created already */
    if (access(new_branch_path, F_OK) == 0) {
        printf("Branch '%s' already exists\n", branch_name); 
        return -1;
    }

    /* create new branch file */
    head_commit = get_head_commit();
    write_to_file(new_branch_path, head_commit, SHA_DIGEST_LENGTH);
    free(new_branch_path);

    return 0;

}

int
branch_delete(char* branch_name)
{
    char* branch_path;
    char* cur_branch;

    if (strcmp(branch_name, HEAD_DEFAULT) == 0) {
        printf("You cannot delete the main branch, respect your senpais.\n");
        return -1;
    }
    
    cur_branch = get_cur_branch();
    if (strcmp(branch_name, cur_branch) == 0) {
        printf("You cannot delete the branch you are on, please checkout to another branch\n");
        return -1;
    }
    free(cur_branch);

    branch_path = cat_str(3, get_dot_dir(), HEADS_DIR, branch_name);
    if (remove(branch_path) == -1) {
        printf("Could not delete branch: %s\n", branch_name);
        return -1;
    }

    free(branch_path);

    return 0;
}

int
branch_info(void)
{
    char* branch_path;
    DIR* dir;
    Dirent* dirent;
    char* cur_branch;

    branch_path = cat_str(2, get_dot_dir(), HEADS_DIR);
    dir = opendir(branch_path);
    free(branch_path);
    if (dir == NULL) {
        perror("branch_info > opendir");
        return -1;
    }

    cur_branch = get_cur_branch();

    /* list all files in heads */
    while ((dirent = readdir(dir)) != NULL) {

        if (strcmp(dirent->d_name, "..") == 0) continue; 
        if (strcmp(dirent->d_name, ".") == 0) continue; 

        if (strcmp(dirent->d_name, cur_branch) == 0) {
            printf("*" "\x1b[32m" " %s\n" "\x1b[0m", dirent->d_name);
        } else {
            printf("  %s\n", dirent->d_name);
        }
    }

    free(cur_branch);
    closedir(dir);

    return 0;
}

int
cat(char* obj_path)
{
    FILE* obj_file;
    char type[HEADER_TYPE_LENGTH];

    /* first find out what type of file it is */
    obj_file = fopen(obj_path, "rb");
    if (obj_file == NULL) {
        printf("Invalid path\n");
        return -1;
    }

    /* read first 4 bytes of header */
    fread(&type, sizeof(char), HEADER_TYPE_LENGTH, obj_file);
    fclose(obj_file);

    if (strcmp(type, BLOB_TYPE) == 0) {
        printf("\x1b[36m" "=-=-=-=-=-=-=-=  9it BLOB  =-=-=-=-=-=-=-=\n" "\x1b[0m");
        printf("(probably)\n");
        return 0;

    } else if (strcmp(type, TREE_TYPE) == 0) {
        Tree tree;
        unsigned char* hash;

        /* get hash from file path, there could be problems if theres no leading slash */
        hash = string_to_hash(strrchr(obj_path, '/')+1);

        tree.hash = hash;
        read_tree(&tree);

        printf("\x1b[32m" "=-=-=-=-=-=-=-=  9it TREE  =-=-=-=-=-=-=-=\n" "\x1b[0m");
        printf("Tree hash: %s\n", hash_to_string(tree.hash));
        for (int i = 0; i < tree.cnum; i++) {
            Tree child;

            child = tree.children[i];
            printf("%d %s %s\n", child.nodeType, hash_to_string(child.hash), child.name);
            printf("\n");
        }

    } else if (strcmp(type, COMMIT_TYPE) == 0) {
        Commit commit;
        unsigned char* hash;

        hash = string_to_hash(strrchr(obj_path, '/')+1);

        commit.hash = hash;
        read_commit(&commit);

        printf("\x1b[33m" "=-=-=-=-=-=-=-= 9it COMMIT =-=-=-=-=-=-=-=\n" "\x1b[0m"); 
        /* free the hash_tostrings */
        printf("Commit hash: %s\n", hash_to_string(commit.hash));
        if (commit.parent_commit_hash != NULL) {
            printf("Previous commit: %s\n", hash_to_string(commit.parent_commit_hash));
        }
        printf("Root tree: %s\n", hash_to_string(commit.root_tree_hash));
        printf("Committer: %s\n", commit.committer);
        /* printf("") */ /* time stamp */
        printf("Message: %s\n", commit.msg);

    } else {
        printf("This does not appear to be a 9it object\n");
        return -1;

    }

    return 0;

}

int
checkout(char* branch_name)
{
    char* branch_path;

    branch_path = cat_str(2, get_dot_dir(), HEADS_DIR);
    if (access(branch_path, F_OK) != 0) {
        printf("Branch %s does not exist\n", branch_name); 
        return -1;
    }
    free(branch_path);

    write_head_commit(branch_name); 

    printf("Switching to branch: %s\n", branch_name);

    return 0;
}

int
commit(char* commit_msg)
{ // tree must be hashed and ready
    Commit new_commit;
    Tree* root_tree;
    char* username;
    char* cur_branch;

    /* grab the index tree */
    root_tree = commit_tree();

    /* get user name */
    username = getenv(NINEIT_USERNAME);
    if (username == NULL) {
        printf("Please set the NINEIT_USERNAME environment variable.\n");
        exit(1);
    }

    /* make commit */
    new_commit.root_tree_hash = root_tree->hash;
    new_commit.committer = username;
    new_commit.timestamp = time(0);
    new_commit.msg = commit_msg;
    new_commit.parent_commit_hash = get_head_commit();

    /* write commit */
    hash_commit(&new_commit); 
    write_commit(&new_commit);

    /* update refs */
    cur_branch = get_cur_branch();
    write_ref(cur_branch, new_commit.hash);
    free(cur_branch);

    /* clear stuff */
    clear_index();
    clear_temp();

    return 0;
}

int
log_horizon(void)
{
    Commit cur_commit;
    char* hexstring;
    char* commit_path;

    cur_commit.hash = get_head_commit();
    if (cur_commit.hash == NULL) {
        printf("There are no commits, get to work!\n");
        return 0;
    }

    while (cur_commit.hash != NULL) {
        read_commit(&cur_commit); 
        
        /* printf("COMMIT!\n"); */
        hexstring = hash_to_string(cur_commit.hash);
        commit_path = cat_str(3, get_dot_dir(), OBJ_DIR, hexstring);
        cat(commit_path);
        printf("\n");
        free(hexstring);
        free(commit_path);

        cur_commit.hash = cur_commit.parent_commit_hash;

    }

    return 0;
}

int
restore(char* local_path)
{ 
    char* relative_path;

    /* make local_path relative to repo root */
    relative_path = get_local_path();
    relative_path = rcat_str(2, relative_path, local_path);

    remove_index(relative_path);

    free(relative_path);

    return 0;
}

int
revert(char* hash)
{
    unsigned char* commit_hash;
    Commit rcommit;
    char* username;
    Commit new_commit;
    char* revert_msg;
    char* cur_branch;

    commit_hash = string_to_hash(hash);
    rcommit.hash = commit_hash;
    read_commit(&rcommit);
    
    revert_commit(rcommit.root_tree_hash);

    /* get user name */
    username = getenv(NINEIT_USERNAME);
    if (username == NULL) {
        printf("Please set the NINEIT_USERNAME environment variable.\n");
        exit(1);
    }
    
    /* make commit */
    new_commit.root_tree_hash = rcommit.root_tree_hash;
    new_commit.committer = username;
    new_commit.timestamp = time(0);

    revert_msg = cat_str(2, "Reverted to commit ", hash);
    new_commit.msg = revert_msg;

    new_commit.parent_commit_hash = get_head_commit();

    /* write commit */
    hash_commit(&new_commit); 
    write_commit(&new_commit);

    /* update refs */
    cur_branch = get_cur_branch();
    write_ref(cur_branch, new_commit.hash);
    free(cur_branch);

    /* clear index */
    clear_index();

    return 0;
}

int
status(void)
{
    char* branch_name;
    Index index;
    IndexItem index_item;
    char** unstaged;
    char** untracked;
    int ind;
    char* item;
    
    branch_name = get_cur_branch();
    printf("You are currently on the ~" "\x1b[32m" "%s" "\x1b[0m" "~ branch\n", branch_name); 
    free(branch_name);
    
    index = *read_index();
    if (index.index_length != 0) {
        printf("\x1b[33m" "=-=-=-=-=-= Staged files =-=-=-=-=-=\n" "\x1b[0m");
        printf("  You can unstage any files with 9it unstage <filename>\n");
    }

    for (int i = 0; i < index.index_length; i++) {
        index_item = index.index_items[i];

        if (memcmp(index_item.hash, DEL_HASH, SHA_DIGEST_LENGTH) == 0) {
            printf("\x1b[32m" "\tdeleted: %s\n" "\x1b[0m", index_item.file_path);
        } else {
            printf("\x1b[32m" "\tmodified: %s\n" "\x1b[0m", index_item.file_path);
        }

    }

    /* possibly add an 'added' or 'modified' or 'deleted' indicator for this too */
    unstaged = compare_index_working();
    if (unstaged[0] != 0) {
        printf("\x1b[33m" "=-=-=-=-=-= Tracked files -=-=-=-=-=\n" "\x1b[0m");
        printf("  You can update tracked files with 9it add <filename>\n");
    }

    ind = 0;
    while ((item = unstaged[ind]) != 0) {
        printf("\x1b[31m" "\t%s\n" "\x1b[0m", item);        
        ind += 1;
    }

    untracked = get_untracked(); 
    /* if (untracked[0] != 0) { */
    /*     printf("Untracked files =-=-=-=-=-=\n"); */
    /*     printf("  You can stage more files with 9it add <filename>\n"); */
    /* } */

    /* ind = 0; */
    /* while ((item = untracked[ind]) != 0) { */
    /*     printf("\x1b[31m" "\t%s\n" "\x1b[0m", item); */        
    /*     ind += 1; */
    /* } */

    return 0;
}
