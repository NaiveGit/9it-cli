#include "headers/objutils.h"

#define READ_CHUNK_SIZE 1024

char*
hash_file(char* file_path, int* hash_length)
{
    FILE* file;
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];
    unsigned char outbuffer[SHA_DIGEST_LENGTH];
    char* hexstring;
    SHA_CTX ctx;

    file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Something went wrong opening %s\n", file_path);
        return NULL;
    }

    SHA1_Init(&ctx);
    fsize = 0;
    while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, file)) > 0) {
        fsize += rsize;
        SHA1_Update(&ctx, inbuffer, rsize);
    }

    fclose(file);

    /* output */
    SHA1_Final(outbuffer, &ctx); 
    *hash_length = SHA_DIGEST_LENGTH;
   
    /* convert hex to readable character */ 
    hexstring = malloc(2*SHA_DIGEST_LENGTH*sizeof(char)+1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hexstring+i*2, "%02x", outbuffer[i]);
    }
    hexstring[2*SHA_DIGEST_LENGTH] = 0;
    
    return hexstring;
}

/* this is rly similar to above, maybe combine them? */
int
compress_file(char* file_path, char* outname)
{
    FILE* file;
    gzFile outfile;
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];

    file = fopen(file_path, "rb");
    outfile = gzopen(outname, "wb");
    if (file == NULL) {
        printf("Something went wrong opening %s\n", file_path);
        return -1;
    }
    if (outfile== NULL) {
        printf("Something went wrong with writing to %s\n", outfile);
        return -1;
    }

    fsize = 0;
    while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, file)) > 0) {
        fsize += rsize;
        gzwrite(outfile, inbuffer, rsize);
    }

    fclose(file);
    gzclose(outfile);

    return fsize;
}


