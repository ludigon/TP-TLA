#ifndef NODES_H
#define NODES_H

typedef enum {
  CONSTANT_NODE = 0,
  VARIABLE_NODE,
  INITIALIZE_NODE,
  OPERATION_NODE,
  STRING_NODE,
  PRINT_NODE
} node_type;

typedef enum {
  INT_VAR = 1,
  STR_VAR
} var_t;

typedef struct nodeT{
    node_type type;
} node_t;

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
    char operator;
} operation_node;

typedef struct string_node {
    node_type type;
    char * str;
} string_node;

typedef struct print_node {
    node_type type;
    node_t * value;
} print_node;



#endif