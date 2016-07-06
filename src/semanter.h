#ifndef _SEMANTER_H_
#define _SEMANTER_H_

#include "ast.h"

#ifdef SEMANTER_VERBOSE
#define SEMANTER_LOG(...) \
	{ \
		fprintf(stderr,"[SEM] "); \
		fprintf(stderr,__VA_ARGS__); \
		fprintf(stderr,"\n"); \
	}
#else
#define SEMANTER_LOG(...)
#endif

typedef enum var_type_t {
	VT_LOCAL = 1, VT_PARAM = 2, VT_GLOBAL = 3
} var_type_t;

int analyze_tree(ast_node_t* root);

void analyze_nodes(ast_node_t* node, ast_node_t** previous, ast_node_t* inloop,
		int* next_var_at, int *errors);
void analyze_one_node(ast_node_t* node, ast_node_t** previous,
		ast_node_t* inloop, int* next_var_at, int *errors);

void analyze_assignment(ast_node_t* node, ast_node_t** previous,
		int* next_var_at, int *errors);
ast_node_t* wrap_place_with_ref(ast_node_t* node);
void analyze_identifier_use(ast_node_t* node, ast_node_t** previous,
		int *errors);
void analyze_loop(ast_node_t* node, ast_node_t** previous, int* next_var_at,
		int *errors);
void analyze_loop_keyw(ast_node_t* node, ast_node_t* inloop, int *errors);
void analyze_proccall(ast_node_t* node, ast_node_t** previous, int *errors);
void analyze_procedure(ast_node_t* node, ast_node_t** previous, int *errors);
void analyze_variable_decl(ast_node_t* node, ast_node_t** previous,
		int* next_var_at, int next_to_plus, int *errors);
void analyze_array(ast_node_t* node, ast_node_t** previous, int* next_var_at,
		int* errors);

void analyze_pre_increm_decrem(ast_node_t* node, TOKEN_TYPE_T replace_with_op,
		ast_node_t** previous, int *errors);
void analyze_post_increm_decrem(ast_node_t* node, TOKEN_TYPE_T replace_with_op,
		ast_node_t** previous, int *errors);

void analyze_container(ast_node_t* node, ast_node_t** previous,
		ast_node_t* inloop, int* next_var_at, int *errors);

void append_child(struct ast_node_t* node, TOKEN_TYPE_T type, YYSTYPE value);
struct ast_node_t* create_predefined_proc_decl(char* name, char* arg1_name);

YYSTYPE find_value_of_meta(struct ast_node_t* child, TOKEN_TYPE_T meta);
struct ast_node_t* find_first_nondecl(ast_node_t* identifier);

struct ast_node_t* find_predefined_var(char* name);
struct ast_node_t* find_var_decl(ast_node_t* previous, char* name, int totally);
var_type_t typeof_var(ast_node_t* identifier);

void semantic_error_0(char* message, struct ast_node_t* context, int *errors);
void semantic_error_1(char* message, void* arg1, struct ast_node_t* context,
		int *errors);
void semantic_error_2(char* message, void* arg1, void* arg2,
		struct ast_node_t* context, int *errors);
void semantic_error_3(char* message, void* arg1, void* arg2, void* arg3,
		struct ast_node_t* context, int *errors);

#endif
