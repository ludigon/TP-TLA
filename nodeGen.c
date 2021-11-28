#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nodes.h"
#include "nodeGen.h"
extern int yylineno;

line_list_node * createLineListNode(tree_node * root) {
	line_list_node * node = malloc(sizeof(line_list_node));
	node->type = LINE_LIST_NODE;
    node->line = yylineno-1;
	node->root = root;
    return node;
}

initialize_node * createInitializeNode(node_t * var, node_t * value, var_t var_type) {
    initialize_node * node = malloc(sizeof(initialize_node));
    node->type = INITIALIZE_NODE;
    node->line = yylineno-1;
    node->var =  var;
    node->var_type = var_type;
    node->value = value;

    return node;
}

variable_node * createVariableNode(const char * name) {
    variable_node * node = malloc(sizeof(variable_node));
    node->type = VARIABLE_NODE;
    node->line = yylineno-1;
    node->name = malloc(strlen(name) * sizeof(char) + 1 + strlen("golf_"));
    sprintf(node->name, "golf_%s", name);
    // strcpy(node->name, name);

    return node;
}

constant_node * createConstantNode(const int value) {
    constant_node * node = malloc(sizeof(constant_node));
    node->type = CONSTANT_NODE;
    node->line = yylineno-1;
    node->number = value;

    return node;
}

operation_node * createOperationNode(node_t * op1, char * operator, node_t * op2) {
    operation_node * node = malloc(sizeof(operation_node));
    node->type = OPERATION_NODE;
    node->line = yylineno-1;
    node->op1 = op1;
    node->operator = operator;
    node->op2 = op2;

    return node;
}

string_node * createStringNode(char * str) {
    string_node * node = malloc(sizeof(string_node));
    node->type = STRING_NODE;
    node->line = yylineno-1;
    node->str = malloc(strlen(str) * sizeof(char) + 1);
    strcpy(node->str, str);

    return node;
}

print_node * createPrintNode(node_t * value) {
    print_node * node = malloc(sizeof(print_node));
    node->type = PRINT_NODE;
    node->line = yylineno-1;
    node->value = value;

    return node;
}

input_node * createInputNode(node_t * var) {
	input_node * node = malloc(sizeof(input_node));
    node->type = INPUT_NODE;
	node->line = yylineno-1;
	node->var = var;
	return node;
}

unary_operation_node * createUnaryOperationNode(node_t * op, char * operator) {
    unary_operation_node * node = malloc(sizeof(unary_operation_node));
    node->type = UNARY_OPERATION_NODE;
    node->line = yylineno-1;
    node->op = op;
    node->operator = operator;

    return node;
}

while_node * createWhileNode(node_t * expression, node_t * block) {
    while_node * node = malloc(sizeof(while_node));
    node->type = WHILE_NODE;
    node->line = yylineno-1;
    node->expression = expression;
    node->block = block;

    return node;
}

if_node * createIfNode(node_t * expression, node_t * block, node_t * else_block) {
    if_node * node = malloc(sizeof(if_node));
    node->type = IF_NODE;
    node->line = yylineno-1;
    node->expression = expression;
    node->block = block;
	node->else_block = else_block;

    return node;
}

assign_node * createAssignNode(node_t * var, node_t * right_side) {
    assign_node * node = malloc(sizeof(assign_node));
    node->type = ASSIGN_NODE;
    node->line = yylineno-1;
    node->var = var;
    node->right_side = right_side;

    return node;
}

tree_node * generateTree(node_t * root) {
    tree_node * tree = malloc(sizeof(tree_node));
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
