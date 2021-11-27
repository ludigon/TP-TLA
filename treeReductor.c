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
char * reduceOperationNode(node_t * node);
char * reduceStringNode(node_t * node);
char * get_prefix(var_t var_type);
int variableExists(char *);
void saveVariable(char * name, var_t var_type);
var_t getVarType(char * name);

typedef char*(*reduceFunction)(node_t*);

typedef struct var_record {
    char * name;
    var_t var_type;
} var_record;

static reduceFunction reducers[] = {
    reduceConstantNode,
    reduceVariableNode,
    reduceInitializeNode,
    reduceOperationNode,
    reduceStringNode
};

static var_record * variables[MAX_VARIABLES];

void init_var_table() {
    for (int i = 0; i < MAX_VARIABLES; i++) {
        variables[i] = NULL;
    }
}

char * exec(node_t * root) {
    if (root == NULL) {
        return malloc(0);
    }
    return reducers[root->type](root);
}

char * reduceInitializeNode(node_t * node) {
    if (node->type != INITIALIZE_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED INITIALIZE_NODE");
        exit(1);
    }
    initialize_node * new_node = (initialize_node*)node;
    char * prefix = get_prefix(new_node->var_type);
    char * name = exec(new_node->var);

    // Chequeamos antes para evitar casos como int a = a;
    if (new_node->value->type == VARIABLE_NODE) {
        variable_node * var = (variable_node*)new_node->value;
        if (!variableExists(var->name)) {
            printf("ERROR: Undeclared variable \"%s\"", var->name);
            exit(1);
        }
    }

    
    if (variableExists(name)) {
        printf("ERROR: variable %s already exists!", name);
        exit(1);
    } else {
        saveVariable(name, new_node->var_type);
    }

    char * middle = "=";
    char * value = exec(new_node->value);

    char * result = malloc((strlen(prefix) + strlen(name) + strlen(middle) + strlen(value) + 1) * sizeof(char));
    sprintf(result, "%s%s = %s;", prefix, name, value);
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

char * reduceOperationNode(node_t * node) {
    if (node->type != OPERATION_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED OPERATION_NODE");
        exit(1);
    }
    operation_node * new_node = (operation_node*)node;
    char * first = exec(new_node->op1);
    char * second = exec(new_node->op2);
    // flags que me indican si los operandos son variables
    int var1 = 0;
    int var2 = 0;
    if (new_node->op1->type == VARIABLE_NODE) {
        variable_node * op1 = (variable_node *)new_node->op1;
        if (!variableExists(op1->name)) {
            printf("ERROR: Undeclared variable \"%s\"", op1->name);
            exit(1);
        }
        var1 = 1;
    }
    if (new_node->op2->type == VARIABLE_NODE) {
        variable_node * op2 = (variable_node *)new_node->op2;
        if (!variableExists(op2->name)) {
            printf("ERROR: Undeclared variable \"%s\"", op2->name);
            exit(1);
        }
        var2 = 1;
    }
    if (var1 && var2) {
        variable_node * var1 = (variable_node*)new_node->op1;
        variable_node * var2 = (variable_node*)new_node->op2;
        var_t var1_type = getVarType(var1->name);
        var_t var2_type = getVarType(var1->name);
        if (var1_type != var2_type) {
            printf("ERROR: Variable types must be the  same when operating!");
            exit(1);
        }
    }
    char * result = malloc((strlen(first) + 6 + strlen(second))*sizeof(char));
    sprintf(result, "(%s %c %s)", first, new_node->operator, second);
    free(first);
    free(second);
    return(result);
}

char * reduceStringNode(node_t * node) {
    if (node->type != STRING_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED STRING_NODE");
        exit(1);
    }
    string_node * new_node = (string_node*)node;
    char * result = malloc(strlen(new_node->str) * sizeof(char));
    strcpy(result, new_node->str);
    return result;
}

int variableExists(char * name) {
    int found = 0;
    int i;
    for (i = 0; !found && i < MAX_VARIABLES && variables[i] != NULL; i++) {
        if (strcmp(name, variables[i]->name) == 0) {
            found = 1;
        }
    }
    if (!found && i == MAX_VARIABLES) {
        printf("ERROR: max number of variables ASD reached! (%d)", MAX_VARIABLES);
        exit(1);
    }
    return found;
}

void saveVariable(char * name, var_t var_type) {
    int i = 0;
    while (variables[i] != NULL && i < MAX_VARIABLES) {
        i++;
    }
    if (i == MAX_VARIABLES) {
        printf("ERROR: max number of variables reached! (%d)", MAX_VARIABLES);
        exit(1);
    }
    variables[i] = malloc(sizeof(var_record));
    variables[i]->name = malloc(strlen(name) * sizeof(char) + 1);
    strcpy(variables[i]->name, name);
    variables[i]->var_type = var_type;
}

var_t getVarType(char * name) {
    int i = 0;
    int found = 0;
    while (variables[i] != NULL && !found) {
        if (strcmp(name, variables[i]->name) == 0) {
            return variables[i]->var_type;
        }
    }
    printf("ERROR: Can't get type of undeclared variable\n");
    exit(1);
}

char * get_prefix(var_t var_type) {
    switch (var_type)
    {
    case INT_VAR:
        return "int ";
        break;
    case STR_VAR:
        return "char *";
        break;
    default:
        printf("ERROR: Unknown var_type %d", var_type);
        exit(1);
        break;
    }
}