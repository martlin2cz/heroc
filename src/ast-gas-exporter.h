#ifndef _AST_STACKODE_EXPORTER_H_
#define _AST_STACKODE_EXPORTER_H_

#include <stdio.h>
#include "ast-exporter.h"
#include "stackode.h"

#ifdef GAS_VERBOSE
#define GAS_LOG(...) \
	{ \
		fprintf(stderr, "[GAS] "); \
		fprintf(stderr, __VA_ARGS__); \
		fprintf(stderr, "\n"); \
	}
#else
#define GAS_LOG(...)
#endif

void ast_export_root(FILE* dest, struct ast_node_t* root);
void export_stackode_to_gas(FILE* dest, sk_program_t* program);
void stackode_instr_to_gas(FILE* dest, sk_instruction_t* instr);

void export_gas_preamble(FILE* dest);
void export_gas_footer(FILE* dest);

void sk_comment_to_gas(FILE* dest, char* comment);
void sk_label_to_gas(FILE* dest, char* label);
void sk_call_to_gas(FILE* dest);
void sk_return_to_gas(FILE* dest);
void sk_cleanup_after_call_to_gas(FILE* dest, long params_count);
void sk_jump_on_non_zero_to_gas(FILE* dest, char* label);
void sk_jump_on_zero_to_gas(FILE* dest, char* label);
void sk_jump_to_to_gas(FILE* dest, char* label);
void sk_load_to_gas(FILE* dest);
void sk_assign_to_gas(FILE* dest);
void sk_declare_atomic_to_gas(FILE* dest);
void sk_declare_array_to_gas(FILE* dest, long size);
void sk_push_constant_to_gas(FILE* dest, long value);
void sk_push_label_adress_to_gas(FILE* dest, char* label);
void sk_push_relative_adress_to_gas(FILE* dest, long adress);
void sk_push_absolute_adress_to_gas(FILE* dest, long adress);
void sk_pop_to_gas(FILE* dest);
void sk_duplicate_to_gas(FILE* dest);
void sk_unary_operation_to_gas(FILE* dest, TOKEN_TYPE_T oper);
void sk_binary_operation_to_gas(FILE* dest, TOKEN_TYPE_T oper);


void gas_new_instructiction(FILE* dest, char* comment);
void gas_add_instr(FILE* dest, char* name);
void gas_add_instr_r(FILE* dest, char* name, char* reg);
void gas_add_instr_n(FILE* dest, char* name, long value);
void gas_add_instr_l(FILE* dest, char* name, char* label);
void gas_add_instr_rr(FILE* dest, char* name, char* reg1, char* reg2);
void gas_add_instr_nr(FILE* dest, char* name, long number, char* reg);

#endif
