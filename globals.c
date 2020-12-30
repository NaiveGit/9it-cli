#include "headers/globals.h"
#include "headers/fileutils.h"

char* dot_dir = NULL;

char* find_dot_dir(void);

const char*
get_dot_dir(void)
{
    // have not been initialized yet
    if (dot_dir == NULL) {
        dot_dir = find_dot_dir();
    } 

    return (const char*)dot_dir;
}

void
set_dot_dir(char* dir)
{
    assert(dot_dir == NULL);

    dot_dir = dir;
}

char*
find_dot_dir(void)
{
    char* cwd;

    cwd = get_cwd();

    do { // this is beyond retarded.

        /* check if current dir is 9it working */
        strcat(cwd, "/.9it/9IT"); 
        if (access(cwd, F_OK) == 0) {
            *(strrchr(cwd, '/')+1) = 0;
            break;
        }
        *strrchr(cwd, '/') = 0;
        *strrchr(cwd, '/') = 0;
        strcat(cwd, "/9IT"); 
        if (access(cwd, F_OK) == 0) {
            *(strrchr(cwd, '/')+1) = 0;
            break;
        }

        *strrchr(cwd, '/') = 0;
        *strrchr(cwd, '/') = 0;

    } while (strlen(cwd) > 0); 

    if (strlen(cwd) == 0) {
        printf("9it repo not initalized\n");
        exit(1);
    }


    return cwd;
}
