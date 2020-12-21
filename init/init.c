#include <conio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

static int mkfolder(char* path,char* foldername);
static int init(char* path);
int main(int argc,char** argv)
{
    init("./9it");
    return 0;
}


static int 
init(char* path) 
{
   //Make the 9-it folder
   mkfolder(path,"");

   //Make objects folder
   mkfolder(path,"/objects");

   //Make index file
   
   //Make refs folder
   mkfolder(path,"/refs");

   //Make tags folder
   mkfolder(path,"/refs/tags");

   //Make heads folder
   mkfolder(path,"/refs/heads");

   //Make HEAD file


   return 0;
}

static int 
mkfolder(char* path, char* foldername)
{
    char temp[100] = "";
    
    strcat(temp,path);
    strcat(temp,foldername);

    #ifdef _WIN32
    int check;
    
    check = mkdir(temp);
    if (!check){
        printf("Git Directory created in windows\n");
    }
    else{
        printf("Error in creating git directory in windows\n");
        exit(1);
    }

    #else
    if (mkdir(temp, 0777) == -1){
        printf("Error creating git directory linux/other\n");
        exit(1);
    }
    else{
        printf("Git Directory created in linux/other\n");
    }
    
    #endif

    return 0;
}
