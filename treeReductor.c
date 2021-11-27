#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodeGen.h"
#include "nodes.h"
#define MAX_NUM_LEN 25
#define MAX_VARIABLES 200
char * reduceInitializeNode(node_t * node);
char * reduceVariableNode(node_t * node);
char * reduceConstantNode(node_t * node);
int variableExists(char *);
void saveVariable(char *);

static char * variables[MAX_VARIABLES];

void init_var_table() {
    for (int i = 0; i < MAX_VARIABLES; i++) {
        variables[i] = NULL;
    }
}

void exec(node_t * root) {
    switch (root->type)
    {
    case INITIALIZE_NODE:
        printf("%s\n", reduceInitializeNode(root));
        break;
    default:
        break;
    }
}

char * reduceInitializeNode(node_t * node) {
    if (node->type != INITIALIZE_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED INITIALIZE_NODE");
        exit(1);
    }
    initialize_node * new_node = (initialize_node*)node;
    char * prefix = "int ";
    char * name = reduceVariableNode(new_node->var);
    if (variableExists(name)) {
        printf("ERROR: variable %s already exists!", name);
        exit(1);
    } else {
        saveVariable(name);
    }
    char * middle = "=";
    char * value = reduceConstantNode(new_node->value);

    char * result = malloc((strlen(prefix) + strlen(name) + strlen(middle) + strlen(value) + 1) * sizeof(char));
    sprintf(result, "int %s = %s;", name, value);
    free(name);
    free(value);
    return result;
}

char * reduceVariableNode(node_t * node) {
    if (node->type != VARIABLE_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED VARIABLE_NODE");
        exit(1);
    }
    variable_node * new_node = (variable_node *)node;
    char * result = malloc(strlen(new_node->name) * sizeof(char));
    strcpy(result, new_node->name);
    return result;
}

char * reduceConstantNode(node_t * node) {
    if (node->type != CONSTANT_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED CONSTANT_NODE");
        exit(1);
    }
    constant_node * new_node = (constant_node*)node;
    char * result = malloc(MAX_NUM_LEN * sizeof(char));
    sprintf(result, "%d", new_node->number);
    return result;
}

int variableExists(char * name) {
    int found = 0;
    int i;
    for (i = 0; !found && i < MAX_VARIABLES && variables[i] != 0; i++) {
        if (strcmp(name, variables[i]) == 0) {
            found = 1;
        }
    }
    if (!found && i == MAX_VARIABLES) {
        printf("ERROR: max number of variables ASD reached! (%d)", MAX_VARIABLES);
        exit(1);
    }
    return found;
}

void saveVariable(char * name) {
    int i = 0;
    while (variables[i] != 0 && i < MAX_VARIABLES) {
        i++;
    }
    if (i == MAX_VARIABLES) {
        printf("ERROR: max number of variables reached! (%d)", MAX_VARIABLES);
        exit(1);
    }
    variables[i] = malloc(strlen(name) * sizeof(char));
    strcpy(variables[i], name);

}