
#include "headers/globals.h"
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

    /* error check these ma nibba */
    write_to_file(INDEX_FILE, INDEX_DEFAULT_HEADER, INDEX_HEADER_LENGTH); //index file
    write_to_file(HEAD_FILE, HEAD_DEFAULT, strlen(HEAD_DEFAULT));
    
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
        // printf("Folder loc: '%s'\n",recur);
        // printf("'%d'",stat(recur,&sb));
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


int
write_to_file(char* file_path, char* contents, int length)
{ 
    FILE* bufstream;
    FILE* outstream;

    bufstream = fmemopen(contents, length, "rb");
    if (bufstream == NULL) {
        perror(NULL);
        return -1;
    }
    outstream = fopen(file_path, "wb");
    if (outstream == NULL) {
        perror(NULL);
        return -1;
    }
    
    copy_stream(bufstream, outstream); // error check this?

    fclose(bufstream);
    fclose(outstream);

    return 0;

}
/* all of these functions are very similar, perhaps find a way to abstract */
int
copy_stream(FILE* instream, FILE* outstream)
{
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE]; 

    fsize = 0;
    while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, instream)) > 0) {
        fsize += rsize;
        fwrite(inbuffer, 1, rsize, outstream);
    }

    return rsize;
}

unsigned char*
hash_stream(FILE* stream)
{
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE]; // maybe just use pointer offsets instead
    unsigned char outbuffer[SHA_DIGEST_LENGTH];
    unsigned char* hexstring;
    SHA_CTX ctx;

    SHA1_Init(&ctx);
    fsize = 0;
    while ((rsize = fread(inbuffer, 1, READ_CHUNK_SIZE, stream)) > 0) {
        SHA1_Update(&ctx, inbuffer, rsize);
        fsize += rsize;
    }

    /* output */
    SHA1_Final(outbuffer, &ctx); 

    hexstring = malloc(SHA_DIGEST_LENGTH*sizeof(unsigned char));
    memcpy(hexstring, outbuffer, SHA_DIGEST_LENGTH);
    
    return hexstring;
}

char*
hash_to_string(unsigned char* hash)
{
    char* hexstring;

    hexstring = malloc(SHA_DIGEST_LENGTH*2+1); // remember null byte
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hexstring+i*2, "%02x", hash[i]);
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
        gzwrite(outfile, inbuffer, rsize);
        fsize += rsize;
    }

    gzclose(outfile);

    return fsize;
}

char*
read_until_null(FILE* stream)
{
    int rsize;
    int msize;
    char* out_str;
    char rchar;

    rsize = 0;
    msize = READ_CHUNK_SIZE;
    out_str = malloc(msize);

    do {
        if (rsize > msize) {
            msize += READ_CHUNK_SIZE;
            out_str = realloc(out_str, msize);
        }

        if (fread(&rchar, sizeof(char), 1, stream) == 0) { //EOF?
            break;
        }
        
        out_str[rsize] = rchar;
        rsize += 1;

    } while (rchar != 0);

    return out_str;
}

void
write_hash(FILE* stream, unsigned char* hash)
{ // might need to error check this
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        fwrite(&hash[i], sizeof(unsigned char), 1, stream);
    }
}

void
write_null(FILE* stream)
{
    char null = 0;
    fwrite(&null, sizeof(char), 1, stream);
}

char*
cat_obj_dir(char* obj_file)
{
    char* out_path;

    out_path = malloc(strlen(OBJ_DIR)+strlen(obj_file)+1);
    memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1);
    strcat(out_path, obj_file);

    return out_path;
}
