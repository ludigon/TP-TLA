#pragma once

#include "node_t.h"

typedef struct tree_node {
    node_t * root;
    struct tree_node * next;
} tree_node;
