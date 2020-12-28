
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/fileutils.h"
#include "headers/options.h"
#include "headers/objutils.h"
#include "headers/globals.h"
#include "headers/nodeutils.h"

int
main(int argc, char** argv)
{

    /* check if dot dir exists */
    /* set some global variable to location of dot dir */
    
    argp_parse(&global_argp, argc, argv, ARGP_IN_ORDER, 0, 0);

    /* init_dot9it(DOT_DIR); */ 
    /* write_blob("9it"); */
    /* Index index = *read_index(); */
    /* printf("the index has %d items in it\n", index.index_length); */
    /* add_index_item("README.md"); */
    /* read_index(); */

    /* Tree child_tree; */
    /* FILE* child_file; */
    /* unsigned char* child_hash; */

    /* child_file = fopen("testing.md", "rb"); */
    /* child_hash = hash_stream(child_file); */
    /* fclose(child_file); */

    /* child_tree.hash = child_hash; */
    /* child_tree.name = "testing.md"; */
    /* child_tree.nodeType = NodeType_blob; */
    /* child_tree.cnum = 0; */

    
    /* Tree* children_array; */
    /* children_array = malloc(sizeof(Tree)); */
    /* children_array[0] = child_tree; */


    /* Tree root_tree; */
    /* FILE* root_file; */
    /* unsigned char* root_hash; */

    /* root_file = fopen("README.md", "rb"); */
    /* root_hash = hash_stream(root_file); */
    /* fclose(root_file); */

    /* root_tree.hash = root_hash; */
    /* root_tree.name = "README.md"; */
    /* root_tree.nodeType = NodeType_tree; */
    /* root_tree.cnum = 1; */
    /* root_tree.children = children_array; */


    /* tree hashing */
    /* Tree child_tree; */
    /* FILE* child_file; */
    /* unsigned char* child_hash; */

    /* child_file = fopen("testing.md", "rb"); */
    /* child_hash = hash_stream(child_file); */
    /* fclose(child_file); */

    /* child_tree.hash = child_hash; */
    /* child_tree.name = "testing.md"; */
    /* child_tree.nodeType = NodeType_blob; */
    /* child_tree.cnum = 0; */

    
    /* Tree* children_array; */
    /* children_array = malloc(sizeof(Tree)); */
    /* children_array[0] = child_tree; */


    /* Tree root_tree; */
    /* FILE* root_file; */

    /* root_tree.name = "README.md"; */
    /* root_tree.nodeType = NodeType_tree; */
    /* root_tree.cnum = 1; */
    /* root_tree.children = children_array; */

    /* hash_tree(&root_tree); */

    /* printf("%s\n", hash_to_string(root_tree.hash)); */

    /* test read tree */
    /* write_tree(&root_tree); */  
    /* Tree* tree = malloc(sizeof(Tree)); */

    /* read_tree("f52d5ce2108ba890ad4232436c5395e82d20abb1", tree); */


    return 0;
}
