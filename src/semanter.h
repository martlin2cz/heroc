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

// frame pointer + return adress
#define FRAME_STUFF_SIZE 2

#define SEMANTIC_ERROR_BODY(...) \
	fprintf(stderr, "Semantic error: "); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, " near to:\n", message); \
	ast_display_root(stderr, context); \
	\
	(*errors)++; \

typedef enum var_type_t {
	VT_LOCAL = 1, VT_PARAM = 2, VT_GLOBAL = 3
} var_type_t;

int analyze_tree(ast_node_t* root);
void add_invoke_main_and_stuff(ast_node_t* root, ast_node_t** previous,
		int* next_var_at, int* errors);
void add_decl_predefineds(ast_node_t* root, ast_node_t** previous, int* next_var_at,
		int* errors);

void analyze_nodes(ast_node_t* node, ast_node_t* parent, ast_node_t** previous,
		ast_node_t* inloop, int* next_var_at, int *errors);
void analyze_one_node(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, ast_node_t* inloop, int* next_var_at,
		int *errors);
void analyze_whole_program_node(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, int* next_var_at, int *errors);

void analyze_assignment(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, int* next_var_at, int *errors);
void analyze_identifier_use(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, int *errors);
void analyze_identifier_ref(ast_node_t* var, ast_node_t* parent,
		ast_node_t** previous, int *errors);

void analyze_index_oper(ast_node_t* index, ast_node_t* parent,
		ast_node_t** previous, int *errors);
void analyze_reference(ast_node_t* ref, ast_node_t* parent,
		ast_node_t** previous, int *errors);
void analyze_dereference(ast_node_t* deref, ast_node_t* parent,
		ast_node_t** previous, int *errors);

void analyze_loop(ast_node_t* node, ast_node_t* parent, ast_node_t** previous,
		int* next_var_at, int *errors);
void analyze_loop_keyw(ast_node_t* node, ast_node_t* parent, ast_node_t* inloop,
		int *errors);
void analyze_proccall(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, int* next_var_at, int *errors);
void analyze_procedure(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, int *errors);
void analyze_variable_decl(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, int* next_var_at, int next_to_plus, int *errors);
void analyze_array(ast_node_t* node, ast_node_t* parent, ast_node_t** previous,
		int* next_var_at, int* errors);

void analyze_pre_increm_decrem(ast_node_t* node, ast_node_t* parent,
TOKEN_TYPE_T replace_with_op, ast_node_t** previous, int *errors);
void analyze_post_increm_decrem(ast_node_t* node, ast_node_t* parent,
TOKEN_TYPE_T replace_with_op, ast_node_t** previous, int *errors);

void analyze_container(ast_node_t* node, ast_node_t* parent,
		ast_node_t** previous, ast_node_t* inloop, int* next_var_at,
		int *errors);


struct ast_node_t* create_predefined_proc_decl(char* name, char* arg1_name);

YYSTYPE find_value_of_meta(struct ast_node_t* child, TOKEN_TYPE_T meta);
struct ast_node_t* find_first_nondecl(ast_node_t* identifier);

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
