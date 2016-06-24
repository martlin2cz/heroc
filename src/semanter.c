#ifndef _SEMANTER_C_
#define _SEMANTER_C_

#include <stdlib.h>
#include <string.h>

#include "semanter.h"
#include "ast.h"
#include "ast-exporter.h"

///////////////////////////////////////////////////////////////////////////////

// frame pointer + return adress
#define FRAME_STUFF_SIZE 2

#define SEMANTIC_ERROR_BODY(...) \
	fprintf(stderr, "Semantic error: "); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, " near to:\n", message); \
	ast_export_root(stderr, context); \
	\
	(*errors)++; \


void semantic_error_0(char* message, struct ast_node_t* context, int *errors) {
	SEMANTIC_ERROR_BODY(message);
}

void semantic_error_1(char* message, void* arg1, struct ast_node_t* context,
		int *errors) {
	SEMANTIC_ERROR_BODY(message, arg1);
}

void semantic_error_2(char* message, void* arg1, void* arg2,
		struct ast_node_t* context, int *errors) {
	SEMANTIC_ERROR_BODY(message, arg1, arg2);
}

void semantic_error_3(char* message, void* arg1, void* arg2, void* arg3,
		struct ast_node_t* context, int *errors) {
	SEMANTIC_ERROR_BODY(message, arg1, arg2, arg3);
}

struct ast_node_t* find_predefined_var(char* name) {

	if (strcmp(name, "print_long") == 0) {
		ast_node_t* print_long_prpr = create_predefined_proc_decl("print_long",
				"x");
		return print_long_prpr;
	}
	if (strcmp(name, "print_char") == 0) {
		ast_node_t* print_char_prpr = create_predefined_proc_decl("print_char",
				"c");
		return print_char_prpr;
	}
	if (strcmp(name, "print_nl") == 0) {
		ast_node_t* print_nl_prpr = create_predefined_proc_decl("print_nl",
		NULL);
		return print_nl_prpr;
	}
	if (strcmp(name, "print_string") == 0) {
		ast_node_t* print_string_prpr = create_predefined_proc_decl(
				"print_string", "str");
		return print_string_prpr;
	}

	return NULL;
}

struct ast_node_t* create_predefined_proc_decl(char* name, char* arg1_name) {
	ast_node_t* id = create_identifier(name);

	ast_node_t* params;
	if (arg1_name) {
		ast_node_t* arg1 = create_identifier(arg1_name);
		params = create_parameters(arg1);
	} else {
		params = create_parameters(NULL);
	}

	ast_node_t* proc = create_procedure(id, params, NULL);

	ast_node_t* decl = create_new_node(JST_VARIABLE_DECL);
	decl->value.child = prepend(id, prepend(proc, NULL));
	return decl;

}

void append_child(struct ast_node_t* node, TOKEN_TYPE_T type, YYSTYPE value) {
	ast_node_t* new = create_new_node(type);
	new->value = value;

	ast_node_t* child = node->value.child;

	if (child) {
		while (child->next) {
			child = child->next;
		}
		child->next = new;
	} else {
		node->value.child = new;
	}
}

YYSTYPE find_value_of_meta(struct ast_node_t* node, TOKEN_TYPE_T meta) {
	ast_node_t* child = node->value.child;
	while (child) {

		if (child->type == meta) {
			return child->value;
		}

		child = child->next;
	}
	fprintf(stderr, "fvom: No value of meta %s of %p (%d) found.\n",
			to_string(meta), node, node->uid);
	YYSTYPE result;
	result.child = NULL;
	return result;
}

struct ast_node_t* find_first_nondecl(ast_node_t* identifier) {
	while (identifier) {
		if (identifier->type != JST_VARIABLE_DECL) {
			return identifier;
		}

		YYSTYPE value = find_value_of_meta(identifier, META_PREVIOUS);
		identifier = value.child;
	}

	fprintf(stderr, "ffn: No nondecl found. Ok, ain't?\n");
	return NULL;
}

struct ast_node_t* find_var_decl(ast_node_t* previous, char* name) {

	while (previous) {
		if (previous->type == JST_VARIABLE_DECL) {
			ast_node_t* var = previous->value.child;
			char* var_name = var->value.child->value.string;
			if (strcmp(var_name, name) == 0) {
				return previous;
			}
		}

		YYSTYPE value = find_value_of_meta(previous, META_PREVIOUS);
		previous = value.child;
	}

	ast_node_t* predef = find_predefined_var(name);
	if (predef) {
		return predef;
	}

	return NULL;
}

var_type_t typeof_var(ast_node_t* identifier) {
	struct ast_node_t* node = find_first_nondecl(identifier);

	if (node == NULL) {
		return VT_GLOBAL;
	} else if (node->type == CNT_STATEMENTS) {
		return VT_LOCAL;
	} else if (node->type == CNT_PARAMETERS) {
		return VT_PARAM;
	} else {
		fprintf(stderr, "tv: Unknown node type %s\n", to_string(node->type));
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

int analyze_tree(ast_node_t* root) {
	int next_var_at = 0;
	ast_node_t* previous = NULL;
	int errors = 0;

	analyze_nodes(root, &previous, NULL, &next_var_at, &errors);

	SEMANTER_LOG("Global scope requires %d cells on stack.", next_var_at);SEMANTER_LOG("Semantic analysis completed with %d errors.", errors);

	return errors;
}

void analyze_nodes(ast_node_t* node, ast_node_t** previous, ast_node_t* inloop,
		int* next_var_at, int *errors) {
	while (node) {
		analyze_one_node(node, previous, inloop, next_var_at, errors);
		node = node->next;
	}
}

void analyze_one_node(ast_node_t* node, ast_node_t** previous,
		ast_node_t* inloop, int* next_var_at, int *errors) {

	SEMANTER_LOG("Analyzing node %d of type %s", node->uid, to_string(node->type));

	switch (node->type) {
	case JST_VARIABLE:
		analyze_identifier_use(node, previous, errors);
		break;
	case STK_FOR:
	case STK_WHILE:
	case STK_DO:
		analyze_loop(node, previous, next_var_at, errors);
		break;
	case STK_BREAK:
	case STK_CONTINUE:
		analyze_loop_keyw(node, inloop, errors);
		break;
	case JST_PROCEDURE:
		analyze_procedure(node, previous, errors);
		break;
	case JST_PROCCALL:
		analyze_proccall(node, previous, errors);
		break;
	case JST_VARIABLE_DECL:
		analyze_variable_decl(node, previous, next_var_at, 1, errors);
		break;
	default:
		if (is_meta(node->type) || is_atomic(node->type)) {
			return;

		} else if (is_container(node->type)) {
			analyze_container(node, previous, inloop, next_var_at, errors);

		} else {
			//printf("!!!! TRACE: The node: %s\n", to_string(node->type));
			analyze_nodes(node->value.child, previous, inloop, next_var_at,
					errors);
		}
	}

}

void analyze_identifier_use(ast_node_t* node, ast_node_t** previous,
		int *errors) {
	char* name = node->value.child->value.string;
	ast_node_t* declaration = find_var_decl(*previous, name);

//	printf(":: -> var (in %p %d) %s decl found? %p\n", node, node->uid, name,
	//		declaration);

	if (declaration) {

		YYSTYPE decl;
		decl.child = declaration;

		append_child(node, META_DECLARATION, decl);
	} else {
		semantic_error_1("Identifier %s not found", name, node, errors);
	}
}

void analyze_loop(ast_node_t* node, ast_node_t** previous, int* next_var_at,
		int *errors) {
	ast_node_t* loop_node = node;
	ast_node_t* child = node->value.child;

	switch (node->type) {
	case STK_FOR: {
		analyze_one_node(child, previous, loop_node, NULL, errors);
		child = child->next;
		analyze_one_node(child, previous, loop_node, NULL, errors);
		child = child->next;
		analyze_one_node(child, previous, loop_node, NULL, errors);
		child = child->next;
		analyze_one_node(child, previous, loop_node, next_var_at, errors);
		break;
	}
	case STK_WHILE: {
		analyze_one_node(child, previous, loop_node, NULL, errors);
		child = child->next;
		analyze_one_node(child, previous, loop_node, next_var_at, errors);
		break;
	}
	case STK_DO: {
		analyze_one_node(child, previous, loop_node, next_var_at, errors);
		child = child->next;
		analyze_one_node(child, previous, loop_node, NULL, errors);
		break;
	}
	default: {
		fprintf(stderr, "al: Unknown loop: %s\n", to_string(node->type));
	}
	}
}

void analyze_loop_keyw(ast_node_t* node, ast_node_t* inloop, int *errors) {
	if (!inloop) {
		semantic_error_1("Keyword %s outside of the loop",
				(node->type == STK_CONTINUE ?
						"continue" :
						(node->type == STK_BREAK ? "break" : "unknown")), node,
				errors);
		return;
	}

	ast_node_t* new = create_new_node(META_LOOP);
	new->value.child = inloop;

	node->value.child = new;
}

void analyze_proccall(ast_node_t* node, ast_node_t** previous, int *errors) {
	SEMANTER_LOG("Starting to analyze procedure call");

	ast_node_t* proc_var = node->value.child;
	if (proc_var->type == JST_VARIABLE) {
		analyze_identifier_use(proc_var, previous, errors);
		YYSTYPE proc_var_decl_val = find_value_of_meta(proc_var,
				META_DECLARATION);
		if (proc_var_decl_val.child) {
			ast_node_t* proc_var_decl = proc_var_decl_val.child;
			ast_node_t* proc_var_proc = proc_var_decl->value.child->next;
			if (proc_var_proc->type == JST_PROCEDURE) {
				ast_node_t* proc_name_node = proc_var_proc->value.child;
				char* name = (
						proc_name_node->type == JST_VARIABLE ?
								proc_name_node->value.child->value.string :
								(proc_name_node->type == STK_LAMBDA ?
										"anonymous" : "?"));

				//finally check the procedure.
				long args_count = lenght_ignore_meta(
						proc_name_node->next->value.child);
				long params_count = lenght_ignore_meta(
						node->value.child->next->value.child);

				if (args_count != params_count) {
					semantic_error_3(
							"Call of procedure %s failed. Expected %d args, but given %d.",
							name, (void*) args_count, (void*) params_count,
							node, errors);
				} else {
					SEMANTER_LOG("Call of procedure %s is OK", name);
					return;
				}

			}
		}
	}

	SEMANTER_LOG("Analyze of procedure call completed unsuccessfully (cannot determine target of call)");
}

void analyze_procedure(ast_node_t* node, ast_node_t** previous, int *errors) {
	//previous, needs to be firstly
	YYSTYPE proc_prev;
	proc_prev.child = *previous;
	append_child(node, META_PREVIOUS, proc_prev);

	// name
	ast_node_t* name = node->value.child;
	if (name->type == JST_VARIABLE) {
		SEMANTER_LOG("Starting to analyze procedure %s", name->value.child->value.string);
	} else if (name->type == STK_LAMBDA) {
		SEMANTER_LOG("Starting to analyze anonymous procedure", node->uid);
	} else {
		fprintf(stderr, "ap: Unknown procedure name type %s\n", name->type);
		return;
	}

	//params list
	ast_node_t* params = name->next;
	YYSTYPE params_prev;
	params_prev.child = node;
	append_child(params, META_PREVIOUS, params_prev);

	ast_node_t* param = params->value.child;
	ast_node_t* prev_param = params;
	int next_param_at = -FRAME_STUFF_SIZE;
	while (param && !is_meta(param->type)) {
		analyze_variable_decl(param, &prev_param, &next_param_at, 0, errors);
		prev_param = param;
		param = param->next;
	}

	// body
	ast_node_t* body = params->next;
	int next_var_at = 0;

	ast_node_t* body_prev = prev_param;
	analyze_one_node(body, &body_prev, NULL, &next_var_at, errors);

	SEMANTER_LOG("Procedure analyze complete, its local variables takes %d cells on stack", next_var_at);
}

void analyze_variable_decl(ast_node_t* node, ast_node_t** previous,
		int* next_var_at, int next_to_plus, int *errors) {

	ast_node_t* var = node->value.child;
	char* name = var->value.child->value.string;

	SEMANTER_LOG("Analyzing declaration of variable %s", name);

	ast_node_t* existing = find_var_decl(*previous, name);
	if (existing) {
		semantic_error_1("Variable %s yet declared", name, node, errors);//TODO FIXME test
	}

	ast_node_t* value = var->next;

	YYSTYPE addr;
	addr.number = *next_var_at;
	append_child(node, META_ADRESS, addr);

	YYSTYPE prev;
	prev.child = *previous;
	append_child(node, META_PREVIOUS, prev);

	if (value) {
		ast_node_t* new_prev = node;
		analyze_one_node(value, &new_prev, NULL, next_var_at, errors);
	}

	if (next_to_plus) {
		(*next_var_at)++;	//TODO implement arrays
	} else {
		(*next_var_at)--;
	}
	(*previous) = node;

	SEMANTER_LOG("Declaration analyzed");
}

void analyze_container(ast_node_t* node, ast_node_t** previous,
		ast_node_t* inloop, int* next_var_at, int *errors) {

	ast_node_t* children = node->value.child;

	if (node->type == CNT_STATEMENTS) {

		YYSTYPE prev;
		prev.child = *previous;
		append_child(node, META_PREVIOUS, prev);

		ast_node_t* new_previous = node;
		analyze_nodes(children, &new_previous, inloop, next_var_at, errors);

	} else {
		analyze_nodes(children, previous, inloop, next_var_at, errors);
	}

}

#endif
