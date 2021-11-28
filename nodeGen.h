#ifndef NODES_GEN_H
#define NODES_GEN_H
#include "nodes.h"

typedef struct tree_node {
    node_t * root;
    struct tree_node * next;
} tree_node;

initialize_node * createInitializeNode(node_t * var, node_t * value, var_t var_type);
variable_node * createVariableNode(const char * name);
constant_node * createConstantNode(const int value);
operation_node * createOperationNode(node_t * op1, char * operator, node_t * op2);
string_node * createStringNode(char * str);
print_node * createPrintNode(node_t * value);
unary_operation_node * createUnaryOperationNode(node_t * op, char * operator);

tree_node * generateTree(node_t * root);
tree_node * addTree(tree_node * current, node_t * root);

#endif