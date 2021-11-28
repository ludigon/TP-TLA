#ifndef NODES_H
#define NODES_H
#include "node_t.h"
#include "tree_node.h"

typedef enum {
  INT_VAR = 1,
  STR_VAR
} var_t;

typedef struct constantNode {
    node_type type;
    int number;
} constant_node;

typedef struct variableNode {
    node_type type;
    char * name;
} variable_node;

typedef struct initializeNode {
    node_type type;
    node_t * var;
    node_t * value;
    var_t var_type;
} initialize_node;

typedef struct operation_node {
    node_type type;
    node_t * op1;
    node_t * op2;
    char* operator;
} operation_node;

typedef struct string_node {
    node_type type;
    char * str;
} string_node;

typedef struct print_node {
    node_type type;
    node_t * value;
} print_node;

typedef struct unary_operation_node {
    node_type type;
    node_t * op;
    char * operator;
} unary_operation_node;

typedef struct while_node {
    node_type type;
    node_t * expression;
    node_t * block;
} while_node;

typedef struct if_node {
    node_type type;
    node_t * expression;
    node_t * block;
} if_node;

typedef struct line_list_node {
    node_type type;
    tree_node * root;
} line_list_node;

#endif
