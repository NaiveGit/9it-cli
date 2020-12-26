
#include "headers/fileutils.h"
#include "headers/objutils.h"
#define READ_CHUNK_SIZE 1024

int 
init_aux(char* root) 
{
    char* path;
    int ind;
    char* dirs[] = {
        "obj/",
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

    /* init index file */
    /* if (init_index() == -1) { */
    /*     printf("Error initializing index file.\n"); */
    /* } */
    
    printf("Successfully initialized 9it in current working directory.\n");

    return 0;
}

int 
mkfolder(char* root, char* dir_name)
{
    struct stat sb;//For folder
    //Full folder path
    char temp[100] = "";
    strcat(temp, root);
    strcat(temp, dir_name);

    //Begin Recursion
    char *ptr = strtok(temp,"/");
    char recur[100] = "";
    
    while (ptr!=NULL){
        strcat(recur,ptr);
        strcat(recur,"/");
        printf("Folder loc: '%s'\n",recur);
        printf("'%d'",stat(recur,&sb));
        if (stat(recur,&sb) != 0){//if folder does not exist, then execute command. Otherwise, skip it.
            #ifdef _WIN32
            if (_mkdir(recur) != 0) {
                printf("Error creating directory %s\n", recur);
                return -1;
            }

            #else
            if (mkdir(recur, 0777) == -1) {
                printf("Error creating directory %s\n", recur);
                return -1;
            } 
            #endif
        }
        ptr = strtok(NULL,"/");
    }

    return 0;
}


/* all of these functions are very similar, perhaps find a way to abstract */
/* int */
/* copy_stream(FILE* instream, FILE* outstream) */
/* { */
/*     size_t rsize; */
/*     int fsize; */
/*     char inbuffer[READ_CHUNK_SIZE]; */ 

/*     fsize = 0; */
/*     while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, instream)) > 0) { */
/*         fsize += rsize; */
/*         fwrite(inbuffer, 1, rsize, outstream); */
/*     } */

/*     return rsize; */
/* } */

/* char* */
/* hash_stream(FILE* stream, int* hash_length) */
/* { */
/*     size_t rsize; */
/*     int fsize; */
/*     char inbuffer[READ_CHUNK_SIZE]; // maybe just use pointer offsets instead */
/*     unsigned char outbuffer[SHA_DIGEST_LENGTH]; */
/*     char* hexstring; */
/*     SHA_CTX ctx; */

/*     SHA1_Init(&ctx); */
/*     fsize = 0; */
/*     while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, stream)) > 0) { */
/*         SHA1_Update(&ctx, inbuffer, rsize); */
/*         fsize += rsize; */
/*     } */

/*     /1* output *1/ */
/*     SHA1_Final(outbuffer, &ctx); */ 
/*     *hash_length = 2*SHA_DIGEST_LENGTH+1; */
   
/*     /1* convert hex to readable character *1/ */ 
/*     hexstring = malloc(2*SHA_DIGEST_LENGTH*sizeof(char)+1); */
/*     for (int i = 0; i < SHA_DIGEST_LENGTH; i++) { */
/*         sprintf(hexstring+i*2, "%02x", outbuffer[i]); */
/*     } */
/*     hexstring[2*SHA_DIGEST_LENGTH] = 0; */
    
/*     return hexstring; */
/* } */

/* int */
/* compress_file(FILE* stream, char* outname) */
/* { */
/*     gzFile outfile; */
/*     size_t rsize; */
/*     int fsize; */
/*     char inbuffer[READ_CHUNK_SIZE]; */

/*     outfile = gzopen(outname, "wb"); */
/*     if (outfile== NULL) { */
/*         printf("Something went wrong with writing to %s\n", outfile); */
/*         return -1; */
/*     } */

/*     fsize = 0; */
/*     while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, stream)) > 0) { */
/*         gzwrite(outfile, inbuffer, rsize); */
/*         fsize += rsize; */
/*     } */

/*     gzclose(outfile); */

/*     return fsize; */
/* } */
