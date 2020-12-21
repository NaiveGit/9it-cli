#include <conio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

static int mkfolder(char *path);

int main(int argc, char **argv ) {
   // If just git init
   char *path;
   printf("%d",argc);
   if (argc == 1)
   {
       
       path = "./9it";
       
   
       if (mkfolder(path) == 1)
       {
           return 1;
       }
   }
   //If git init with bare
   else
   {
       path = "./";
   }

    //Make objects folder
    //Make index file
    //Make description file
    //MAke refs folder, which contains tags/ and heads/
    //Make HEAD file
    char temp[100] = "";
    
    strcat(temp,path);
    strcat(temp,"/objects");
    if (mkfolder(temp) == 1)
       {
           return 1;
       }
    temp[0] = '\0';
    
    strcat(temp,path);
    strcat(temp,"/refs");
    if (mkfolder(temp) == 1)
       {
           return 1;
       }

    temp[0] = '\0';
    
    strcat(temp,path);
    strcat(temp,"/refs/tags");
    if (mkfolder(temp) == 1)
       {
           return 1;
       }

    temp[0] = '\0';
    
    strcat(temp,path);
    strcat(temp,"/refs/heads");
    if (mkfolder(temp) == 1)
       {
           return 1;
       }

    //Make the HEAD, description, and index file
    
   
   return 0;
}

static int 
mkfolder(char *path)
{
    #ifdef _WIN32
    int check;
    
    check = mkdir(path);
    if (!check)
    {
        printf("Git Directory created in windows\n");
    }
    else{
        printf("Error in creating git directory in windows\n");
        return 1;
    }

    #else
    if (mkdir(path, 0777) == -1)
    {
        printf("Error creating git directory linux/other\n");
        return 1;
    }
    else
    {
        printf("Git Directory created in linux/other\n");
    }
    
    #endif

    return 0;
}
