#ifndef _EVAL_C_
#define _EVAL_C_

#include <stdlib.h>
#include <string.h>
#include "syntax-tree-node.h"
#include "flex-headers.h"
#include "bison-headers.h"
#include "../gen/bison.h"

int evaluate(syntax_node_t* root) {
	int broken = 0, continued = 0;
	
	syntax_node_t* result = evaluate_node(root, &broken, &continued);	
	printf("\n");
	

	if (result) {

		if (broken != 0 || continued != 0) {
			printf("Warning: break or continue used somewhere outside of loop. [IGN]\n");
		}
	
		printf("Evaluation returned element: ");
		print_node(-1, result);
		return 0;
	} else {
		printf("Evaluation finished with error. Here is list of defined variables, may be usefull:\n");
		print_variables();
		return 1;
	}
}
	
syntax_node_t* evaluate_node(syntax_node_t* node, int* brokenp, int* continuedp) {
	if (!node)
		return NULL;
	
	if (print_each_tree) {
		print_node(0, node);
		printf("-------------\n");
	}

  switch(node->type) {
		case NUMBER:
		case TEXT:
			return node;
		
		case VARIABLE:
			return evaluate_variable(node);
		
		case OPERATION:
				return evaluate_operation(node, brokenp, continuedp);
		case ASSIGNMENT:
				return evaluate_assignment(node, brokenp, continuedp);
		case COMMANDS:
				return evaluate_commands(node, brokenp, continuedp);
		case SPECIAL:
				return evaluate_special(node, brokenp, continuedp);
		
		default:
			ERROR("Unrecognized node type %d\n", node->type);		
	}
  	return NULL;
}

syntax_node_t* evaluate_variable(syntax_node_t* node) {
	variables_t* var = variables;

	while (var) {
		int cmp = strcmp(node->value.text, var->name);
		if (cmp == 0) {
			return create_number(var->value);
		}
		var = var->next;
	}	


	if (define_unused) {
		int* varp = define_variable(node);
		return create_number(*varp);
	} else {
		ERROR("Variable %s undefined\n", node->value.text);
	}
}


int* define_variable(syntax_node_t* node) {

	if (!variables) {
		variables = (variables_t*) malloc(sizeof(variables));
		if (!variables)
			ERROR("Cannot allocate memory for variables");
		variables->name = node->value.text;
		return &(variables->value);
	}

	variables_t* var = variables;

	while (var) {
		int cmp = strcmp(node->value.text, var->name);
		if (cmp == 0)
			return &(var->value);
		var = var->next;
	}
	
	var = (variables_t*) malloc(sizeof(variables));
	if (!var)
		ERROR("Cannot allocate memory for variable %s", node->value.text);
	
	var->name = node->value.text;
	var->next = variables;
	variables = var;

	return &(var->value);
}		
			
void print_variables() {
	variables_t* var = variables;
	
	while (var) {
		printf("%s  ->  %d\n", var->name, var->value);
		var = var->next;
	}
	printf("--- That's all folks ---\n");

}

syntax_node_t* evaluate_operation(syntax_node_t* node, int* brokenp, int* continuedp) {
	
	syntax_node_t* lnode = node->child;
	syntax_node_t* lvalue = evaluate_node(lnode, brokenp, continuedp);
	if (!lvalue)
		return NULL;
	
	if (!(lnode->next)) {
		 syntax_node_t* value = evaluate_node(lnode, brokenp, continuedp);
		 if (!value)
			 return NULL;

		switch (node->value.operator) {
			case TK_OPERATOR_PLUS:
				return create_number(+ value->value.number);
			case TK_OPERATOR_MINUS:
				return create_number(- value->value.number);
		
			case TK_OPERATOR_INCREMENT: {
					int intval = value->value.number;
					int* val_addr = define_variable(lnode);
					*val_addr = intval + 1;
					return create_number(intval);
				}
			case TK_OPERATOR_DECREMENT:	{
					int intval = value->value.number;
					int* val_addr = define_variable(lnode);
					*val_addr = intval - 1;
					return create_number(intval);
				}
			case TK_OPERATOR_PRE_INC: {
					int intval = value->value.number;
					int* val_addr = define_variable(lnode);
					*val_addr = intval + 1;
					return create_number(intval + 1);
				}
			case TK_OPERATOR_PRE_DEC: {
					int intval = value->value.number;
					int* val_addr = define_variable(lnode);
					*val_addr = intval - 1;
					return create_number(intval - 1);
				}
		
			case TK_OPERATOR_NOT:
				return create_number(! (value->value.number));
			case TK_OPERATOR_BIT_NOT:
				return create_number(~ (value->value.number));
			default:
				ERROR("Unexpected unary operator %x", node->value.operator);
		}
	} else {
		syntax_node_t* rnode = lnode->next;
		syntax_node_t* rvalue = evaluate_node(rnode, brokenp, continuedp);
		if (!rvalue)
			return NULL;

		int lval = lvalue->value.number;
		int rval = rvalue->value.number;

		switch (node->value.operator) {
			case TK_OPERATOR_PLUS:
				return create_number(lval + rval);
			case TK_OPERATOR_MINUS:
				return create_number(lval - rval);
			case TK_OPERATOR_TIMES:
				return create_number(lval * rval);
			case TK_OPERATOR_DIVIDE:
				if (rval == 0)
					ERROR("Divison by zero");
				return create_number(lval / rval);
			case TK_OPERATOR_MODULO:
				if (rval == 0)
					ERROR("Divison (modulo) by zero");
				return create_number(lval % rval);
			case TK_OPERATOR_EQUAL:
				return create_number(lval == rval);
			case TK_OPERATOR_NOT_EQUAL:
				return create_number(lval != rval);
			case TK_OPERATOR_LESS_THAN:
				return create_number(lval < rval);
			case TK_OPERATOR_GREATER_THAN:
				return create_number(lval > rval);
			case TK_OPERATOR_LESS_EQUAL:
				return create_number(lval <= rval);
			case TK_OPERATOR_GREATER_EQUAL:
				return create_number(lval >= rval);
	
			case TK_OPERATOR_AND:
				return create_number(lval && rval);
			case TK_OPERATOR_OR:
				return create_number(lval || rval);

			case TK_OPERATOR_BIT_AND:
				return create_number(lval & rval);
			case TK_OPERATOR_BIT_OR:
				return create_number(lval | rval);
			case TK_OPERATOR_BIT_XOR:
				return create_number(lval ^ rval);
			case TK_OPERATOR_SHIFT_LEFT:
				return create_number(lval << rval);
			case TK_OPERATOR_SHIFT_RIGHT:
				return create_number(lval >> rval);
			default:
				ERROR("Unexpected binary operator %x\n", node->value.operator);
		}
	}

	return NULL;
}
	
syntax_node_t* evaluate_assignment(syntax_node_t* node, int* brokenp, int* continuedp) {
	syntax_node_t* variable = node->child;
	syntax_node_t* expr = variable->next;
	
	int* var_addr = define_variable(variable);

	int var = *var_addr;

	syntax_node_t* val_node = evaluate_node(expr, brokenp, continuedp);
	if (!val_node)
		return NULL;

	int value = val_node->value.number;

	switch (node->value.operator) {
		case TK_OPERATOR_ASSIGNMENT:
			var = value;
			break;
		case TK_OPERATOR_PLUS:
			var += value;
			break;
	case TK_OPERATOR_MINUS:
			var -= value;
			break;
	case TK_OPERATOR_TIMES:
			var *= value;
			break;
	case TK_OPERATOR_DIVIDE:
			if (value == 0)
				ERROR("Divison by zero in assingment");

			var /= value;
			break;
	case TK_OPERATOR_MODULO:
			if (value == 0)
				ERROR("Divison (modulo) by zero in assingment");		
			
			var %= value;
			break;
	case TK_OPERATOR_BIT_AND:
			var &= value;
			break;
	case TK_OPERATOR_BIT_OR:
			var |= value;
			break;
	case TK_OPERATOR_BIT_XOR:
			var ^= value;
			break;
	case TK_OPERATOR_SHIFT_RIGHT:
			var >>= value;
			break;
	case TK_OPERATOR_SHIFT_LEFT:
			var <<= value;
			break;
	default:
			ERROR("Unexpected assign operator %x\n", node->value.operator);
	}


	*var_addr = var;
	return create_number(var);
}

syntax_node_t* evaluate_commands(syntax_node_t* node, int* brokenp, int* continuedp) {
	if (!(node->child)) {
		return create_number(0);
	}
	
	syntax_node_t* result;

	syntax_node_t* child = node->child;
	while (child) {
		result = evaluate_node(child, brokenp, continuedp);
		if (!result)
			return NULL;
		child = child->next;
	}

	return result;
}

syntax_node_t* evaluate_special(syntax_node_t* node, int* brokenp, int* continuedp) {
	switch (node->value.special) {
		case IF:	{
				syntax_node_t* cond = node->child;
				syntax_node_t* cmd = cond->next;
				syntax_node_t* elsecmd = cmd->next;

				syntax_node_t* condval = evaluate_node(cond, brokenp, continuedp);
				if (!condval)
					return NULL;

				if (condval->value.number) {
					return evaluate_node(cmd, brokenp, continuedp);
				} else {
					if (elsecmd)
						return evaluate_node(elsecmd, brokenp, continuedp);
					else
						return create_number(0);
				} 
			}
		case FOR:	{
				syntax_node_t* init = node->child;
				syntax_node_t* cond = init->next;
				syntax_node_t* incr = cond->next;
				syntax_node_t* cmds = incr->next;
			
				syntax_node_t *condval, *result;
				
				result = evaluate_node(init, brokenp, continuedp);
				if (!result)
					return NULL;

				while (1) {
					int new_broken = 0, new_continued = 0;

					condval = evaluate_node(cond, brokenp, continuedp);
					if (!condval)
						return NULL;
					
					if (!(condval->value.number))
						break;
					
					result = evaluate_node(cmds, &new_broken, &new_continued);
					if (!result)
						return NULL;
					
					if (new_continued)
						continue;
					if (new_broken)
						break;
			
					result = evaluate_node(incr, brokenp, continuedp);
					if (!result)
						return NULL;
				}
						
				return result;	 
			}
		case WHILE:	{
				syntax_node_t* cond = node->child;
				syntax_node_t* cmds = cond->next;

				syntax_node_t *condval, *result;

				while (1) {
					int new_broken = 0, new_continued = 0;
					
					condval = evaluate_node(cond, brokenp, continuedp);
					if (!condval)
						return NULL;
					
					if (!(condval->value.number))
						break;
					
					result = evaluate_node(cmds, &new_broken, &new_continued);
					if (!result)
						return NULL;
					
					if (new_continued)
						continue;
					if (new_broken)
						break;
				}
				
				return result;	 
			}
		case DO_WHILE:	{
				syntax_node_t* cond = node->child;
				syntax_node_t* cmds = cond->next;
				
				syntax_node_t *condval, *result;
				
				while (1) {
					int new_broken = 0, new_continued = 0;
					
					result = evaluate_node(cmds, &new_broken, &new_continued);
					if (!result)
						return NULL;
					
					if (new_continued)
						continue;
					if (new_broken)
						break;

					condval = evaluate_node(cond, brokenp, continuedp);
					if (!condval)
						return NULL;
					
					if (!(condval->value.number))
						break;
				}
				
				return result;		
			}
		case BREAK:
			*brokenp = 1;
			return create_number(0);

		case CONTINUE:
			*continuedp = 1;
			return create_number(0);

		case PRINT: {
				syntax_node_t* param1 = node->child;
				syntax_node_t* param2 = param1->next;
			
				if (param1->type == TEXT && !param2) {
					printf("%s", param1->value.text);
					return create_number(0);
				}
				
				char* format;
				syntax_node_t* expr;

				if (!param2) {
					format = "%d\n";
					expr = param1;
				} else {
					format = param1->value.text;
					expr = param2;
				}

				syntax_node_t* value = evaluate_node(expr, brokenp, continuedp);
				if (!value)
					return NULL;

				printf(format, value->value.number);
				return create_number(0);
			}
		case SCAN: {
				syntax_node_t* var = node->child;
				int* value_addr = define_variable(var);

				scanf("%d", value_addr);
				
				return create_number(0);
			}
		}

	return NULL;
}
#endif
