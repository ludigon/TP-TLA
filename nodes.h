#ifndef NODES_H
#define NODES_H

typedef enum {
  CONSTANT_NODE = 0,
  VARIABLE_NODE,
  INITIALIZE_NODE
} node_type;

typedef enum {
  INTEGER = 0,
  FLOAT = 1
} var_type;

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
} initialize_node;

#endif