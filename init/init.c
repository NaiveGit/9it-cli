#include <conio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 

int main(int argc, char **argv ) {
   // If just git init
   char *path;
   printf("%d",argc);
   if (argc == 1)
   {
       
       path = "9it";
       
   
       #ifdef _WIN32
       int check;
       
       check = mkdir(path);
       if (!check)
       {
           printf("Git Directory created in windows");
       }
       else{
           printf("Error in creating git directory in windows");
       }

       #else
       if (mkdir(path, 0777) == -1)
       {
           printf("Error creating git directory linux/other");
       }
       else
       {
           printf("Git Directory created in linux/other");
       }
        
       #endif

   }
   //If git init with bareback
   else
   {
       path = "./";

   }

   
   
   return 0;
}