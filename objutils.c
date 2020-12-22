#include "headers/objutils.h"
#include "headers/globals.h"

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
    *hash_length = 2*SHA_DIGEST_LENGTH+1;
   
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

int
write_blob(char* org_file_path)
{
    char* hashed_filename;
    int hash_length;
    char* out_path;

    hashed_filename = hash_file(org_file_path, &hash_length);

    out_path = malloc(strlen(OBJ_DIR)+strlen(hashed_filename)+1);
    memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(out_path, hashed_filename);
    
    compress_file(org_file_path, out_path);
    free(hashed_filename);
    free(out_path);

    return 0;
}

int
write_tree(Tree* tree)
{

    return 0;
}

int
write_commit(Commit* commit)
{

    return 0;
}

