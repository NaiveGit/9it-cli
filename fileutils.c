
#include "headers/fileutils.h"

#define READ_CHUNK_SIZE 1024

int 
init_aux(char* root) 
{
    char* path;
    int ind;
    char* dirs[] = {
        "",
        "obj/",
        "refs/",
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
    
    printf("Successfully initialized 9it in current working directory.\n");

    return 0;
}

int 
mkfolder(char* root, char* dir_name)
{ // make this recursive for convience
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


char*
hash_stream(FILE* stream, int* hash_length)
{
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];
    unsigned char outbuffer[SHA_DIGEST_LENGTH];
    char* hexstring;
    SHA_CTX ctx;

    SHA1_Init(&ctx);
    fsize = 0;
    while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, stream)) > 0) {
        fsize += rsize;
        SHA1_Update(&ctx, inbuffer, rsize);
    }

    /* output */
    SHA1_Final(outbuffer, &ctx); 
    *hash_length = 2*SHA_DIGEST_LENGTH+1;
   
    /* convert hex to readable character */ 
    hexstring = malloc(2*SHA_DIGEST_LENGTH*sizeof(char)+1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hexstring+i*2, "%02x", outbuffer[i]);
    }
    hexstring[2*SHA_DIGEST_LENGTH] = 0;
    
    return hexstring;
}

int
compress_file(FILE* stream, char* outname)
{
    gzFile outfile;
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];

    outfile = gzopen(outname, "wb");
    if (outfile== NULL) {
        printf("Something went wrong with writing to %s\n", outfile);
        return -1;
    }

    fsize = 0;
    while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, stream)) > 0) {
        fsize += rsize;
        gzwrite(outfile, inbuffer, rsize);
    }

    gzclose(outfile);

    return fsize;
}
