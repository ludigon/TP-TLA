#ifndef TREE_H
#define TREE_H
#include "nodes.h"
#include "nodeGen.h"


typedef char*(*reduceFunction)(node_t*);
typedef struct var_record {
    char * name;
    var_t var_type;
} var_record;


char *exec(node_t * root);

#endif