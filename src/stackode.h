#ifndef _STACKODE_H_
#define _STACKODE_H_

#include "ast.h"


#define SK_INCREASE_SIZE 3

/*********************************************************/

typedef enum sk_instr_type_t {
	SKI_LABEL			= 0x001,
	SKI_PUSH_NO_VALUE	= 0x002,
	SKI_PUSH_NUMBER		= 0x003,
	SKI_PUSH_VALUE_OF	= 0x004,
	SKI_PUSH_ADDRES_OF	= 0x005,
	SKI_ASSIGN			= 0x006,
	SKI_CALL			= 0x007,
	SKI_RETURN			= 0x008,
	SKI_JUMP_TO			= 0x009,
	SKI_JUMP_IF			= 0x00a,
	SKI_BINARY_OP		= 0x00b,
	SKI_UNARY_OP		= 0x00c,
} sk_instr_type_t;

typedef union sk_instr_value_t {
	long number;
	char *string;
} sk_instr_value_t;

typedef struct sk_instruction_t {
	sk_instr_type_t type;
	sk_instr_value_t value;
} sk_instruction_t;

typedef struct sk_program_t {
	long length;
	long count;
	sk_instruction_t** instructions;
} sk_program_t;

/*********************************************************/
sk_program_t* ast_to_stackode(ast_node_t* root);

sk_program_t* create_empty_program(void);

void add_instruction(sk_program_t* program, sk_instruction_t* instruction);

void check_and_increase_size(sk_program_t* program);

void add_instruction(sk_program_t* program, sk_instruction_t* instruction);

sk_instruction_t* create_instruction(sk_instr_type_t type);
sk_instruction_t* create_instruct_with_str(sk_instr_type_t type, char* str);
sk_instruction_t* create_instruct_with_num(sk_instr_type_t type, long num);

#endif
