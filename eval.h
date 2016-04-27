#ifndef _EVAL_H
#define _EVAL_H

#include <stdio.h>
#include "syntax-tree-node.h"

#define ERROR(ARGS...) {\
	fprintf(stderr, ARGS);\
	return NULL;\
}

typedef struct variables_t {
	char* name;
	int value;
	struct variables_t *next;
} variables_t;


variables_t* variables = NULL;
/*
int continued = 0;
int broken = 0;
*/
int define_unused = 0;
int print_each_tree = 1;

int evaluate(syntax_node_t* root);
syntax_node_t* evaluate_node(syntax_node_t* node, int* brokenp, int* continuedp);
syntax_node_t* evaluate_variable(syntax_node_t* node);
int* define_variable(syntax_node_t* node);
void print_variables();
syntax_node_t* evaluate_operation(syntax_node_t* node, int* brokenp, int* continuedp);
syntax_node_t* evaluate_assignment(syntax_node_t* node, int* brokenp, int* continuedp); 
syntax_node_t* evaluate_commands(syntax_node_t* node, int* brokenp, int* continuedp);
syntax_node_t* evaluate_special(syntax_node_t* node, int* brokenp, int* continuedp);




/*
int evaluate(syntax_node_t* node);
syntax_node_t* evaluate_node(syntax_node_t* node, int* brokenp, int* continuedp);

syntax_node_t* evaluate_operation(syntax_node_t* node, int* brokenp, int* continuedp);
syntax_node_t* evaluate_assignment(syntax_node_t* node, int* brokenp, int* continuedp);
syntax_node_t* evaluate_commands(syntax_node_t* node, int* brokenp, int* continuedp);
syntax_node_t* evaluate_special(syntax_node_t* node, int* brokenp, int* continuedp);

syntax_node_t* evaluate_variable(syntax_node_t* node);
int* define_variable(syntax_node_t* node);
void print_variables();
*/

#include "eval.c"
#endif
