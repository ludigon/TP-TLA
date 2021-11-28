#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodeGen.h"
#include "node_t.h"
#include "nodes.h"
#include "treeReductor.h"
#include "tree_node.h"

#define MAX_NUM_LEN 25
#define MAX_VARIABLES 200
#define MAX_ERROR_MSG_LEN 1000

extern void yyerror(const char *);
extern int yylineno;

char * reduceInitializeNode(node_t * node);
char * reduceVariableNode(node_t * node);
char * reduceConstantNode(node_t * node);
char * reduceOperationNode(node_t * node);
char * reduceStringNode(node_t * node);
char * reducePrintNode(node_t * node);
char * reduceUnaryOperationNode(node_t * node);
char * reduceWhileNode(node_t * node);
char * reduceIfNode(node_t * node);
char * reduceLineListNode(node_t * node);

char * get_prefix(var_t var_type);
char * print_str(node_t *);
char * print_int(node_t *);
char * print_var(variable_node *);
char * computeBinaryOperation(int op1, int op2, char * operator);
char * computeUnaryOperation(int op, char * operator);
int variableExists(char *);
void saveVariable(char * name, var_t var_type);
var_t getVarType(char * name);

static reduceFunction reducers[] = {
    reduceConstantNode,
    reduceVariableNode,
    reduceInitializeNode,
    reduceOperationNode,
    reduceStringNode,
    reducePrintNode,
    reduceUnaryOperationNode,
	reduceWhileNode,
	reduceIfNode,
	reduceLineListNode
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
            printf("Line %d -> ERROR: Undeclared variable \"%s\"\n", yylineno, var->name);
            exit(1);
        }
        if (getVarType(var->name) != new_node->var_type) {
            yyerror("ERROR: Cannot assign value from one variable type to another\n");
            exit(1);
        }
    }

    
    if (variableExists(name)) {
        printf("ERROR: variable %s already exists!", name);
        exit(1);
    } else {
        saveVariable(name, new_node->var_type);
    }

    char * middle = " = ";
    char * value = exec(new_node->value);

    char * result = malloc((strlen(prefix) + strlen(name) + strlen(middle) + strlen(value) + 1 + 1) * sizeof(char));
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
    char * result = malloc(strlen(new_node->name) * sizeof(char) + 1);
    strcpy(result, new_node->name);
    return result;
}

char * reduceConstantNode(node_t * node) {
    if (node->type != CONSTANT_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED CONSTANT_NODE");
        exit(1);
    }
    constant_node * new_node = (constant_node*)node;
    char * result = malloc(MAX_NUM_LEN * sizeof(char) + 1);
    if (new_node->number >= 0) {
        sprintf(result, "%d", new_node->number);
    } else {
        sprintf(result, "(%d)", new_node->number);
    }
    return result;
}

char * reduceOperationNode(node_t * node) {
    //Chequeamos que sea del tipo
    if (node->type != OPERATION_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED OPERATION_NODE");
        exit(1);
    }
    //Casteo
    operation_node * new_node = (operation_node*)node;
    // flags que me indican si los operandos son variables
    int var1 = 0;
    int var2 = 0;
    //Si algun operando es variable, me fijo que la variable este definida y que sean del mismo tipo
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

    char * result;
    //por ultimo, si ambos operandos son numeros, hago la reduccion de la cuenta (2+2) -> 4
    if (new_node->op1->type == CONSTANT_NODE && new_node->op2->type == CONSTANT_NODE) {
        constant_node * node1 = (constant_node*)new_node->op1;
        constant_node * node2 = (constant_node*)new_node->op2;
        result = computeBinaryOperation(node1->number, node2->number, new_node->operator);
    } else {
        char * first = exec(new_node->op1);
        char * second = exec(new_node->op2);
        result = malloc((strlen(first) + 3 + strlen(second) + strlen(new_node->operator))*sizeof(char));
        sprintf(result, "(%s%s%s)", first, new_node->operator, second);
        free(first);
        free(second);
    }
    return(result);
}

char * reduceStringNode(node_t * node) {
    if (node->type != STRING_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED STRING_NODE");
        exit(1);
    }
    string_node * new_node = (string_node*)node;
    char * result = malloc(strlen(new_node->str) * sizeof(char) + 1);
    strcpy(result, new_node->str);
    return result;
}

char * reducePrintNode(node_t * node) {
    if (node->type != PRINT_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED PRINT_NODE");
        exit(1);
    }
    print_node * new_node = (print_node*)node;
    if (new_node->value == NULL) {
        printf("ERROR: Empty \"print\" statement");
        exit(1);
    }
    switch (new_node->value->type) {
        case STRING_NODE:
            return print_str(new_node->value);
        case OPERATION_NODE:
        case CONSTANT_NODE:
            return print_int(new_node->value);
        case VARIABLE_NODE:
			return print_var((variable_node *)new_node->value);
		default:
			printf("ERROR: Cant print that value.");
    }
    return NULL;
}

char * reduceUnaryOperationNode(node_t * node) {
    if (node->type != UNARY_OPERATION_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED UNARY_OPERATION_NODE");
        exit(1);
    }
    unary_operation_node * new_node = (unary_operation_node*)node;
    //Si es una variable, me fijo que exista

    
    if (new_node->op->type == VARIABLE_NODE) {
        variable_node * op = (variable_node *)new_node->op;
        if (!variableExists(op->name)) {
            printf("ERROR: Undeclared variable \"%s\"", op->name);
            exit(1);
        }
        //Si existe, me fijo que sea de tipo int
        if (!(getVarType(op->name) == INT_VAR)) {
            printf("ERROR: Invalid operation for variable type");
            exit(1);
        }
    }
    //Si es una constante, me fijo el invertirla y devolverla su valor calculado
    else if (new_node->op->type == CONSTANT_NODE) {
        constant_node * op = (constant_node*)new_node->op;
        return computeUnaryOperation(op->number, new_node->operator);
    }
    char * operand = exec(new_node->op);
    char * result = malloc((strlen(new_node->operator) + strlen(operand) + 3) * sizeof(char));
    sprintf(result, "%s(%s)", new_node->operator, operand);
    free(operand);
    return result;
}

char * reduceWhileNode(node_t * node) {
    if (node->type != WHILE_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED WHILE_NODE");
        exit(1);
    }
    while_node * new_node = (while_node*)node;
	char * expression = exec(new_node->expression);
	char * block = exec(new_node->block);
	char * result = malloc(strlen("while () {\n}\n") + strlen(expression) + strlen(block) + 1);
	sprintf(result, "while (%s) {\n%s}\n", expression, block);
	free(expression);
	free(block);
	return result;
}

char * reduceIfNode(node_t * node) {
    if (node->type != IF_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED IF_NODE");
        exit(1);
    }
    if_node * new_node = (if_node*)node;
	char * expression = exec(new_node->expression);
	char * block = exec(new_node->block);
	char * result = malloc(strlen("if () {\n}\n") + strlen(expression) + strlen(block) + 1);
	sprintf(result, "if (%s) {\n%s}\n", expression, block);
	free(expression);
	free(block);
	return result;
}

char * reduceLineListNode(node_t * node) {
	if (node->type != LINE_LIST_NODE) {
        printf("ERROR: INCORRECT NODE TYPE. EXPECTED LINE_LIST_NODE");
        exit(1);
	}
	
    line_list_node * new_node = (line_list_node*)node;
	tree_node *n = new_node->root;
	size_t capacity = 16;
	size_t size = 0;
	char * result = malloc(capacity);
	while (n != NULL) {
		char * line = exec(n->root);
		size_t line_length = strlen(line);

		while (size + line_length + 1 >= capacity) {
			result = realloc(result, capacity *= 2);
		}

		strcpy(result + size, line);
		free(line);

		size += line_length;

		n = n->next;
	}
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
        i++;
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

char * print_str(node_t * node) {
    char * prefix = "printf(\"\%s\",";
    char * string = exec((node_t*)node);

    char * suffix = ");\n";
    char * result = malloc(strlen(prefix) + strlen(string) + strlen(suffix) + 10);
    sprintf(result, "%s%s%s", prefix,string,suffix);
    free(string);
    return result;
}

char * print_int(node_t * node) {
    char * prefix = "printf(\"\%d\",";
    char * string = exec((node_t*)node);
    char * suffix = ");\n";
    char * result = malloc(strlen(prefix) + strlen(string) + strlen(suffix) + 10);
    sprintf(result, "%s%s%s", prefix,string,suffix);
    free(string);
    return result;
}

char * print_var(variable_node * node) {
    char * prefix;
	if (getVarType(node->name) == STR_VAR)
		prefix = "printf(\"\%s\",";
	else
		prefix = "printf(\"\%d\",";
    char * string = exec((node_t*)node);
    char * suffix = ");\n";
    char * result = malloc(strlen(prefix) + strlen(string) + strlen(suffix) + 10);
    sprintf(result, "%s%s%s", prefix,string,suffix);
    free(string);
    return result;
}

//Esto puede mejorar
char * computeBinaryOperation(int op1, int op2, char * operator) {
    char * answer = malloc(MAX_NUM_LEN);
    int result;
    if (strcmp(operator, "+") == 0) {
        result = op1 + op2;
    } else if (strcmp(operator, "-") == 0){
        result = op1 - op2;
    } else if (strcmp(operator, "*") == 0){
        result = op1 * op2;
    } else if (strcmp(operator, "/") == 0){
        if (op2 == 0) {
            printf("ERROR: Cannot divide by 0");
            exit(1);
        }
        result = op1 / op2;
    } else if (strcmp(operator, "\%") == 0){
        if (op2 == 0) {
            printf("ERROR: Cannot divide by 0");
            exit(1);
        }
        result = op1 % op2;
    } else if (strcmp(operator, ">") == 0){
        result = op1 > op2;
    } else if (strcmp(operator, ">=") == 0){
        result = op1 >= op2;
    } else if (strcmp(operator, "<") == 0){
        result = op1 < op2;
    } else if (strcmp(operator, "<=") == 0){
        result = op1 <= op2;
    } else if (strcmp(operator, "&&") == 0){
        result = op1 && op2;
    } else if (strcmp(operator, "||") == 0){
        result = op1 || op2;
    } else {
        printf("ERROR: invalid operator %s", operator);
        exit(1);
    }
    sprintf(answer, "%d", result);
    return answer;
}

char * computeUnaryOperation(int op, char * operator) {
    char * answer = malloc(MAX_NUM_LEN);
    int result;
    if (strcmp(operator, "!") == 0) {
        result = !op;
    } else {
        printf("ERROR: invalid unary operator %s", operator);
        exit(1);
    }
    sprintf(answer, "%d", result);
    return answer;
}
