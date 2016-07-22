#ifndef _STACKODE_H_
#define _STACKODE_H_

#include "ast.h"

#define SK_INCREASE_SIZE 3

/*********************************************************/

#ifdef STACKODE_VERBOSE
#define STACKODE_LOG(...) \
	{ \
		fprintf(stderr, "[SCG] "); \
		fprintf(stderr, __VA_ARGS__); \
		fprintf(stderr, "\n"); \
	}
#else
#define STACKODE_LOG(...)
#endif

typedef enum sk_instr_type_t {
	SKI_COMMENT = 0x111,
	SKI_LABEL = 0x112,

	SKI_CALL = 0x121,
	SKI_RETURN = 0x122,
	SKI_CLEANUP_AFTER_CALL = 0x123,
	SKI_JUMP_ON_ZERO = 0x131,
	SKI_JUMP_ON_NON_ZERO = 0x132,
	SKI_JUMP_TO = 0x133,

	SKI_LOAD = 0x211,
	SKI_ASSIGN = 0x212,
	SKI_DECLARE_ATOMIC = 0x213,
	SKI_DECLARE_ARRAY = 0x214,

	SKI_PUSH_CELL_SIZE = 0x221,
	SKI_PUSH_CONSTANT = 0x222,
	SKI_PUSH_LABEL_ADRESS = 0x223,
	SKI_PUSH_RELATIVE_ADRESS = 0x224,
	SKI_PUSH_ABSOLUTE_ADRESS = 0x225,

	SKI_POP = 0x231,
	SKI_DUPLICATE = 0x232,

	SKI_UNARY_OPERATION = 0x311,
	SKI_BINARY_OPERATION = 0x312,

	SKI_INVOKE_EXTERNAL = 0x411,
	SKI_END = 0x412,
} sk_instr_type_t;

typedef union sk_instr_value_t {
	long number;
	char *string;
} sk_instr_value_t;

typedef struct sk_instruction_t {
	sk_instr_type_t type;
	sk_instr_value_t value;
	sk_instr_value_t value2;
} sk_instruction_t;

typedef struct sk_program_t {
	long length;
	long count;
	sk_instruction_t** instructions;
} sk_program_t;

/*********************************************************/
sk_program_t* ast_to_stackode(ast_node_t* root);

void list_to_stackode(sk_program_t* program, ast_node_t* nodes);
void single_node_to_stackode(sk_program_t* program, ast_node_t* node);

void node_to_stackode_comment(sk_program_t* program, ast_node_t* node);

void program_to_sackode(sk_program_t* program, ast_node_t* node);
void number_to_stackode(sk_program_t * program, ast_node_t * node);
void assignment_to_stackode(sk_program_t * program, ast_node_t * node);
void sizeof_to_stackode(sk_program_t * program, ast_node_t * node);
void if_to_stackode(sk_program_t * program, ast_node_t * node, char* label_base);
void for_to_stackode(sk_program_t * program, ast_node_t * node);
void while_to_stackode(sk_program_t * program, ast_node_t * node);
void do_to_stackode(sk_program_t * program, ast_node_t * node);
void return_to_stackode(sk_program_t * program, ast_node_t * node);
void break_to_stackode(sk_program_t * program, ast_node_t * node);
void continue_to_stackode(sk_program_t * program, ast_node_t * node);
void variable_to_stackode(sk_program_t * program, ast_node_t * node);
void array_to_stackode(sk_program_t * program, ast_node_t * node);
void procedure_to_stackode(sk_program_t * program, ast_node_t * node);
void proccall_to_stackode(sk_program_t * program, ast_node_t * node);
void var_decl_to_stackode(sk_program_t * program, ast_node_t * node);
void unary_op_to_stackode(sk_program_t * program, ast_node_t * node);
void binary_op_to_stackode(sk_program_t * program, ast_node_t * node);

void and_to_stackode(sk_program_t * program, ast_node_t * node);
void or_to_stackode(sk_program_t * program, ast_node_t * node);
void ternary_to_stackode(sk_program_t * program, ast_node_t * node);
void dereference_to_stackode(sk_program_t * program, ast_node_t * node);
void reference_to_stackode(sk_program_t * program, ast_node_t * node);
void inc_dec_to_stackode(sk_program_t * program, ast_node_t * node);
void expression_to_stackode(sk_program_t * program, ast_node_t * node);
void invoke_external_to_stackode(sk_program_t * program, ast_node_t * node);

/*********************************************************/
sk_program_t* create_empty_program(void);

void add_instruction(sk_program_t* program, sk_instruction_t* instruction);

void check_and_increase_size(sk_program_t* program);

void add_instruction(sk_program_t* program, sk_instruction_t* instruction);
void remove_last_instr(sk_program_t* program);

sk_instruction_t* create_instruction(sk_instr_type_t type);
sk_instruction_t* create_instruct_with_str(sk_instr_type_t type, char* str);
sk_instruction_t* create_instruct_with_num(sk_instr_type_t type, long num);
sk_instruction_t* create_instruct_with_op(sk_instr_type_t type,
TOKEN_TYPE_T oper);
sk_instruction_t* create_instruct_with_str_num(sk_instr_type_t type, char* str,
		long num);

char* generate_label(char* prefix, char* name, int uid);

char* label_of_proc(ast_node_t* proc);
#endif
