#include "headers/objutils.h"
#include "headers/fileutils.h"
#include "headers/globals.h"

#define INDEX_DEFAULT_HEADER "DIRC\x00\x00\x00\x01\x00\x00\x00\x00"
#define INDEX_HEADER_START 0
#define INDEX_HEADER_ENTRY_START 8
#define INDEX_HEADER_ENTRY_LENGTH 4
#define INDEX_HEADER_LENGTH 12

#define INDEX_ENTRY_START INDEX_HEADER_LENGTH
#define INDEX_ENTRY_LENGTH 0

/* char* */
/* write_blob(char* file_path) */
/* { */
/*     FILE* file; */
/*     char* hashed_filename; */
/*     int hash_length; */
/*     char* out_path; */

/*     /1* open original file *1/ */
/*     file = fopen(file_path, "rb"); */
/*     if (file == NULL) { */
/*         printf("Something went wrong opening %s\n", file_path); */
/*         return NULL; */
/*     } */

/*     hashed_filename = hash_stream(file, &hash_length); */

/*     /1* check to see if object already exists *1/ */
/*     if (access(hashed_filename, F_OK) != 0) { // does not exist */

/*         /1* create the file *1/ */
/*         /1* build the correct out dir *1/ */
/*         out_path = malloc(strlen(OBJ_DIR)+strlen(hashed_filename)+1); */
/*         memcpy(out_path, OBJ_DIR, strlen(OBJ_DIR)+1); */
/*         strcat(out_path, hashed_filename); */

/*         compress_file(file, out_path); // prob should error check this */

/*         free(out_path); */
/*     } */

/*     /1* clean up *1/ */
/*     free(file); */

/*     return hashed_filename; */
/* } */

/* char* */
/* write_tree(Tree* tree) */
/* { */
/*     /1* shove everything into hash *1/ */

/*     return NULL; */
/* } */

/* char* */
/* write_commit(Commit* commit) */
/* { */
/*     return NULL; */
/* } */

/* int */
/* init_index(void) */
/* { */ 
/*     FILE* bufstream; */
/*     FILE* outstream; */

/*     bufstream = fmemopen(INDEX_DEFAULT_HEADER, INDEX_HEADER_LENGTH, "rb"); */
/*     if (bufstream == NULL) { */
/*         perror(NULL); */
/*         /1* printf("Something went wrong opening the string buffer\n"); *1/ */
/*         return -1; */
/*     } */
/*     outstream = fopen(INDEX_FILE, "wb"); */
/*     if (outstream == NULL) { */
/*         perror(NULL); */
/*         /1* printf("Something went wrong opening the index file\n"); *1/ */
/*         return -1; */
/*     } */
    
/*     copy_stream(bufstream, outstream); // error check this? */

/*     fclose(bufstream); */
/*     fclose(outstream); */

/*     return 0; */
/* } */

/* Index* */
/* read_index(void) */
/* { */
/*     Index index; */
/*     Index* return_index; */
/*     IndexItem* index_array; */
/*     IndexItem index_item; */
/*     FILE* index_file; */
/*     int entry_count; */

/*     index_file = fopen(INDEX_FILE, "rb"); */
/*     if (index_file == NULL) { */
/*         perror(NULL); */
/*         /1* printf("Cannot open index file\n"); *1/ */
/*         return NULL; */
/*     } */

/*     /1* prob check if index file has right format and such *1/ */
/*     /1* perror("Index file may be corrupt"); *1/ */
/*     /1* verify checksum and such *1/ */

/*     /1* read the header *1/ */
/*     fseek(index_file, INDEX_HEADER_ENTRY_START, SEEK_SET); */
/*     fread(&entry_count, INDEX_HEADER_ENTRY_LENGTH, 4, index_file); */
/*     index.index_length = entry_count; */

/*     index_array = malloc(entry_count*sizeof(IndexItem)); */
/*     for (int i = 0; i < entry_count; i++) { */
/*     } */

/*     index.index_items = &index_array; */

/*     /1* make a copy of the index struct and return it *1/ */
/*     return_index = malloc(sizeof(Index)); */
/*     memcpy(return_index, &index, sizeof(Index)); */
/*     // may also need to memcpy the indexitem array inside */

/*     return return_index; */
/* } */

/* Index* */
/* write_index(Index* index) */
/* { */
/*     return NULL; */
/* } */

/* int */
/* add_index_item(char* file_path) */
/* { */
/*     Index index; */
/*     char* hashed_filename; */
/*     Stat file_stat; */
/*     IndexItem new_indexitem; */

/*     /1* read the old index *1/ */
/*     index = *(read_index()); */

/*     /1* grab file stats *1/ */
/*     lstat(file_path, &file_stat); // error check!! */

/*     /1* write it to blobs - doing this so early is kinda wack, but we need the hashed file name *1/ */
/*     hashed_filename = write_blob(file_path); // error check!! */

/*     /1* build new indexitem struct *1/ */
/*     /1* add time stuff *1/ */
/*     new_indexitem.dev = file_state.st_dev; */
/*     new_indexitem.ino = file_state.st_ino; */
/*     new_indexitem.mode = file_state.st_mode; */
/*     new_indexitem.uid = file_state.st_uid; */
/*     new_indexitem.gid = file_state.st_gid; */
/*     new_indexitem.file_size = file_state.st_size; */
/*     new_indexitem.hash = hashed_filename; */

/*     /1* append to end of index_items in index struct *1/ */
/*     index.index_items = realloc(index.index_items, (index.index_length+1)*sizeof(IndexItem)); */
/*     memcpy(index.index_items[index.index_length], new_indexitem, sizeof(IndexItem)); */

/*     /1* build the binary *1/ */


/*     /1* write to index *1/ */
/*     write_index(index); */


/*     /1* clean up *1/ */
/*     free(index); */
/*     /1* also free index array *1/ */

/*     return 0; */

/* } */
