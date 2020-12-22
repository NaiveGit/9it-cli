#include "headers/objutils.h"

extern unsigned char*
hash_file(char* file_path, int* hash_length)
{
    FILE* file;
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];
    char* outbuffer;
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
    outbuffer = malloc(SHA_DIGEST_LENGTH*sizeof(unsigned char));
    SHA1_Final(outbuffer, &ctx); 
    *hash_length = SHA_DIGEST_LENGTH;
    
    return outbuffer;
}


