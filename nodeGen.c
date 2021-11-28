#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nodes.h"
#include "nodeGen.h"

initialize_node * createInitializeNode(node_t * var, node_t * value, var_t var_type) {
    initialize_node * node = malloc(sizeof(initialize_node));
    node->type = INITIALIZE_NODE;
    node->var =  var;
    node->var_type = var_type;
    node->value = value;
    return node;
}

variable_node * createVariableNode(const char * name) {
    variable_node * node = malloc(sizeof(variable_node));
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) * sizeof(char));
    strcpy(node->name, name);
    return node;
}

constant_node * createConstantNode(const int value) {
    constant_node * node = malloc(sizeof(constant_node));
    node->type = CONSTANT_NODE;
    node->number = value;
    return node;
}

operation_node * createOperationNode(node_t * op1, char * operator, node_t * op2) {
    operation_node * node = malloc(sizeof(operation_node));
    node->type = OPERATION_NODE;
    node->op1 = op1;
    node->operator = operator;
    node->op2 = op2;
    return node;
}

string_node * createStringNode(char * str) {
    string_node * node = malloc(sizeof(string_node));
    node->type = STRING_NODE;
    node->str = malloc(strlen(str) * sizeof(char) + 1);
    strcpy(node->str, str);
    return node;
}

print_node * createPrintNode(node_t * value) {
    print_node * node = malloc(sizeof(print_node));
    node->type = PRINT_NODE;
    node->value = value;
    return node;
}

unary_operation_node * createUnaryOperationNode(node_t * op, char * operator) {
    unary_operation_node * node = malloc(sizeof(unary_operation_node));
    node->type = UNARY_OPERATION_NODE;
    node->op = op;
    node->operator = operator;
    return node;
}

tree_node * generateTree(node_t * root) {
    tree_node * tree = malloc(sizeof(node_t));
    tree->root = root;
    tree->next = NULL;
    return tree; 
}

tree_node * addTree(tree_node * current, node_t * root) {
    tree_node * head = current;
    while(current->next != NULL) {
        current = current->next;
    }
    if (root != NULL) {
        current->next = generateTree(root);
    } else {
        current->next = NULL;
    }
    return head;
}