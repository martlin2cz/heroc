#ifndef _STACKODE_C_
#define _STACKODE_C_

#include <stdlib.h>
#include <stdio.h>
#include "stackode.h"
#include "tokens.h"

/*********************************************************/
sk_program_t* ast_to_stackode(ast_node_t* root) {
	sk_program_t *program = create_empty_program();

	printf("Converting ast to stackode, ...\n");
	single_node_to_stackode(program, root);

	//TODO use context here...

	return program;
}

void single_node_to_stackode(sk_program_t* program, ast_node_t* node) {

	/*
	if (is_atomic(node->type)) {
		atomic_to_stackode(program, node);
		return;
	}
	if (is_operator(node->type)) {
		operator_to_stackode(program, node);
		return;
	}

	other_to_stackode(program, node);
			return;
			*/
}
/*
void single_node_to_stackode(sk_program_t* program, ast_node_t* node) {

}
*/

/*********************************************************/
sk_program_t* create_empty_program(void) {
	sk_program_t* program = (sk_program_t*) malloc(sizeof(sk_program_t));
	program->instructions = NULL;
	program->count = 0;
	program->length = 0;

	return program;
}

void add_instruction(sk_program_t* program, sk_instruction_t* instruction) {
	check_and_increase_size(program);

	program->instructions[program->count] = instruction;
	program->count++;
}

void check_and_increase_size(sk_program_t* program) {

	if (program->count + 1 >= program->length) {

		long new_len = program->length + SK_INCREASE_SIZE;
		long new_bytes = new_len * sizeof(sk_instruction_t*);

		sk_instruction_t** instrs = program->instructions;

		if (instrs) {
			instrs = (sk_instruction_t**) realloc(instrs, new_bytes);
		} else {
			instrs = (sk_instruction_t**) malloc(new_len);
		}

		if (!instrs) {
			fprintf(stderr, "Memory allocation failed\n");
			return;
		}
		program->instructions = instrs;
		program->length = new_len;
	}
}

sk_instruction_t* create_instruction(sk_instr_type_t type) {
	sk_instruction_t* instr = (sk_instruction_t*) malloc(
			sizeof(sk_instr_type_t));

	instr->type = type;
	instr->value.number = 0;
	return instr;
}
sk_instruction_t* create_instruct_with_str(sk_instr_type_t type, char* str) {
	sk_instruction_t* instr = (sk_instruction_t*) malloc(
			sizeof(sk_instr_type_t));

	instr->type = type;
	instr->value.string = str;
	return instr;
}
sk_instruction_t* create_instruct_with_num(sk_instr_type_t type, long num) {
	sk_instruction_t* instr = (sk_instruction_t*) malloc(
			sizeof(sk_instr_type_t));

	instr->type = type;
	instr->value.number = num;
	return instr;
}

#endif
