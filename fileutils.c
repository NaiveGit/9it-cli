
#include "headers/fileutils.h"

int 
init(char* root) 
{
    char* path;
    int ind;
    char* dirs[] = {
        "",
        "/objects",
        "/refs",
        "/refs/tags",
        "/refs/heads",
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
    
    printf("Successfully initialized 9it in current working directory.\n");

    return 0;
}

int 
mkfolder(char* root, char* dir_name)
{
    char temp[100] = "";
    
    strcat(temp, root);
    strcat(temp, dir_name);

    #ifdef _WIN32
    if (_mkdir(temp) != 0) {
        printf("Error creating directory %s\n", temp);
        return -1;
    }

    #else
    if (mkdir(temp, 0777) == -1) {
        printf("Error creating directory %s\n", temp);
        return -1;
    } 
    
    #endif

    return 0;
}
