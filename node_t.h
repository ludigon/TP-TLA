#pragma once

typedef enum {
  CONSTANT_NODE = 0,
  VARIABLE_NODE,
  INITIALIZE_NODE,
  OPERATION_NODE,
  STRING_NODE,
  PRINT_NODE,
  INPUT_NODE,
  UNARY_OPERATION_NODE,
  WHILE_NODE,
  IF_NODE,
  LINE_LIST_NODE,
  ASSIGN_NODE
} node_type;

typedef struct nodeT{
    node_type type;
    int line;
} node_t;

