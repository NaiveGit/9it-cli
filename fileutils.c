#include "headers/globals.h"
#include "headers/fileutils.h"
#include "headers/objutils.h"
#define READ_CHUNK_SIZE 128

int 
mkfolder(char* root, char* dir_name)
{
    struct stat sb;//For folder
    //Full folder path
    char* temp;

    temp = cat_str(2, root, dir_name);

    //Begin Recursion
    char *ptr = strtok(temp,"/");
    char recur[strlen(temp)+1];
    recur[0] = 0;
    
    while (ptr!=NULL){
        strcat(recur,ptr);
        strcat(recur,"/");

        if (stat(recur,&sb) != 0){//if folder does not exist, then execute command. Otherwise, skip it.
            if (mkdir(recur, 0777) == -1) {
                printf("Error creating directory %s\n", recur);
                return -1;
            } 
        }
        ptr = strtok(NULL,"/");
    }

    free(temp);

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

unsigned char*
string_to_hash(char* hexstring)
{
    unsigned char* hash;

    assert(strlen(hexstring) == 2*SHA_DIGEST_LENGTH);    

    hash = malloc(SHA_DIGEST_LENGTH);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        char hex[2];

        memcpy(hex, hexstring+i*2, 2);
       
        hash[i] = (int)strtol(hex, NULL, 16);
    }

    return hash;
}

int
compress_file(FILE* stream, char* out_path)
{
    gzFile outfile;
    int rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];

    outfile = gzopen(out_path, "ab");
    if (outfile == NULL) {
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

int
uncompress_file(char* blob_path, char* out_path)
{
    gzFile infile;
    FILE* outfile;
    size_t rsize;
    int fsize;
    char inbuffer[READ_CHUNK_SIZE];

    infile = gzopen(blob_path, "rb"); 
    outfile = fopen(out_path, "wb");

    if (infile == NULL) {
        printf("Error opening blob\n");
        return -1;
    }
    if (outfile == NULL) {
        printf("Error opening uncompressed file\n");
        return -1;
    }

    /* maybe check header to see if its a valid object */

    /* printf("seeking %d\n",gzseek(infile, HEADER_LENGTH, SEEK_SET)); */

    fsize = 0;
    while ((rsize = gzread(infile, inbuffer, READ_CHUNK_SIZE)) > 0) {
        fwrite(inbuffer, 1, rsize, outfile);
        fsize += rsize;
    }

    gzclose(infile);
    fclose(outfile);

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
cat_str(int num, ...) // mallocs
{
    va_list args;
    int length;
    char* out;
    char* cur;

    va_start(args, num);

    length = 1;
    out = malloc(sizeof(char));
    out[0] = 0;

    for (int i = 0; i < num; i++) {
        cur = va_arg(args, char*);
        length += strlen(cur);
        out = realloc(out, length);
        strcat(out, cur);
    }

    va_end(args);

    return out;
}

char*
rcat_str(int num, char* first, ...) // resizes
{
    va_list args;
    int length;
    char* cur;

    num -= 1;

    va_start(args, first);

    length = strlen(first);
    first = realloc(first, length+1);

    for (int i = 0; i < num; i++) {
        cur = va_arg(args, char*);
        length += strlen(cur);
        first = realloc(first, length);
        strcat(first, cur);
    }

    va_end(args);

    return first;
}


int // from https://codereview.stackexchange.com/questions/54722/determine-if-one-string-occurs-at-the-end-of-another/54724
strend(char* s, char* t)
{
    size_t ls = strlen(s);
    size_t lt = strlen(t);

    if (ls >= lt) {
        return (0 == memcmp(t, s + (ls - lt), lt));
    }
    return 0; 
}

char*
get_cwd(void)
{
    char cwd[PATH_MAX+2];
    char* out;

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Unable to get cwd\n");
        exit(1);
    }

    out = strdup(cwd);
    strcat(out, "/");

    return out;
}

char* // returns cwd relative to repo root
get_local_path(void)
{
    char* cwd;
    char* repo_root;
    int out_len;
    char* out;

    cwd = get_cwd();
    repo_root = get_repo_root(); 

    /* cwd SHOULD start with repo_root */
    assert(memcmp(repo_root, cwd, strlen(repo_root)) == 0);

    out_len = strlen(cwd) - strlen(repo_root);
    out = malloc(out_len+1);
    memcpy(out, cwd+strlen(repo_root), out_len+1);

    return out;

}
