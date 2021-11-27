#ifndef NODES_GEN_H
#define NODES_GEN_H
#include "nodes.h"

typedef struct tree_node {
    node_t * root;
    struct tree_node * next;
} tree_node;

initialize_node * createInitializeNode(node_t * var, node_t * value);
variable_node * createVariableNode(const char * name);
constant_node * createConstantNode(const int value);
tree_node * generateTree(node_t * root);
tree_node * addTree(tree_node * current, node_t * root);

#endif