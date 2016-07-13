#ifndef _AST_STACKODE_EXPORTER_C_
#define _AST_STACKODE_EXPORTER_C_

#include <stdio.h>
#include "ast-gas-exporter.h"
#include "stackode.h"

void ast_export_root(FILE* dest, struct ast_node_t* root) {
	sk_program_t* sk = ast_to_stackode(root);
	export_stackode_to_gas(dest, sk);

}

void export_stackode_to_gas(FILE* dest, sk_program_t* program) {
	int i;
	GAS_LOG("exporting %d instructions", program->count);

	export_gas_preamble(dest);

	for (i = 0; i < program->count; i++) {
		sk_instruction_t* instr = program->instructions[i];

		GAS_LOG("exporting instruction %x with value %x", instr->type, instr->value.number);
		stackode_instr_to_gas(dest, instr);
	}

	export_gas_footer(dest);

	GAS_LOG("exported all %d instructions", program->count);
}

void export_gas_preamble(FILE* dest) {
	fprintf(dest, "## TODO ...\n");
}

void export_gas_footer(FILE* dest) {
	fprintf(dest, "## end of TODO ...\n");
}

void stackode_instr_to_gas(FILE* dest, sk_instruction_t* instr) {
	switch (instr->type) {

	case SKI_COMMENT:
		sk_comment_to_gas(dest, instr->value.string);
		break;
	case SKI_LABEL:
		sk_label_to_gas(dest, instr->value.string);
		break;
	case SKI_CALL:
		sk_call_to_gas(dest);
		break;
	case SKI_RETURN:
		sk_return_to_gas(dest);
		break;
	case SKI_CLEANUP_AFTER_CALL:
		sk_cleanup_after_call_to_gas(dest, instr->value.number);
		break;
	case SKI_JUMP_ON_ZERO:
		sk_jump_on_zero_to_gas(dest, instr->value.string);
		break;
	case SKI_JUMP_ON_NON_ZERO:
		sk_jump_on_non_zero_to_gas(dest, instr->value.string);
		break;
	case SKI_JUMP_TO:
		sk_jump_to_to_gas(dest, instr->value.string);
		break;
	case SKI_LOAD:
		sk_load_to_gas(dest);
		break;
	case SKI_ASSIGN:
		sk_assign_to_gas(dest);
		break;
	case SKI_DECLARE_ATOMIC:
		sk_declare_atomic_to_gas(dest);
		break;
	case SKI_DECLARE_ARRAY:
		sk_declare_array_to_gas(dest, instr->value.number);
		break;
	case SKI_PUSH_CONSTANT:
		sk_push_constant_to_gas(dest, instr->value.number);
		break;
	case SKI_PUSH_LABEL_ADRESS:
		sk_push_label_adress_to_gas(dest, instr->value.string);
		break;
	case SKI_PUSH_RELATIVE_ADRESS:
		sk_push_relative_adress_to_gas(dest, instr->value.number);
		break;
	case SKI_PUSH_ABSOLUTE_ADRESS:
		sk_push_absolute_adress_to_gas(dest, instr->value.number);
		break;
	case SKI_POP:
		sk_pop_to_gas(dest);
		break;
	case SKI_DUPLICATE:
		sk_duplicate_to_gas(dest);
		break;
	case SKI_UNARY_OPERATION:
		sk_unary_operation_to_gas(dest, instr->value.number);
		break;
	case SKI_BINARY_OPERATION:
		sk_binary_operation_to_gas(dest, instr->value.number);
		break;
	default:
		fprintf(stderr, "sitg: Unknown stackode instruction %x\n", instr->type);
	}
}

void sk_comment_to_gas(FILE* dest, char* comment) {
	gas_new_instructiction(dest, comment);
}

void sk_label_to_gas(FILE* dest, char* label) {
	gas_new_instructiction(dest, "label");
	fprintf(dest, "%s:\n", label);
}
void sk_call_to_gas(FILE* dest) {
	//TODO pop ax; call ax ?
	gas_new_instructiction(dest, "call");
	gas_add_instr(dest, "call");
}
void sk_return_to_gas(FILE* dest) {
	gas_new_instructiction(dest, "return");
	gas_add_instr(dest, "ret");
}
void sk_cleanup_after_call_to_gas(FILE* dest, long params_count) {
	gas_new_instructiction(dest, "cleanup after call");

	gas_add_instr_r(dest, "pop", "ax");
	gas_add_instr_nr(dest, "add", params_count, "sp");
	gas_add_instr_r(dest, "push", "ax");
}
void sk_jump_on_non_zero_to_gas(FILE* dest, char* label) {
	gas_new_instructiction(dest, "jump on non zero");

	gas_add_instr_r(dest, "pop", "ax");
	gas_add_instr_nr(dest, "cmp", 0, "ax");
	gas_add_instr_l(dest, "jne", label);
}
void sk_jump_on_zero_to_gas(FILE* dest, char* label) {
	gas_new_instructiction(dest, "jump on zero");

	gas_add_instr_r(dest, "pop", "ax");
	gas_add_instr_nr(dest, "cmp", 0, "ax");
	gas_add_instr_l(dest, "je", label);
}
void sk_jump_to_to_gas(FILE* dest, char* label) {
	gas_new_instructiction(dest, "jump to");

	gas_add_instr_l(dest, "jmp", label);
}
void sk_load_to_gas(FILE* dest) {
	gas_new_instructiction(dest, "load");

	fprintf(dest, "#TODOOOOO\n");
	//TODO
}
void sk_assign_to_gas(FILE* dest) {
	gas_new_instructiction(dest, "assign");

	fprintf(dest, "#TODOOOOO\n");
	//TODO
}
void sk_declare_atomic_to_gas(FILE* dest) {
	gas_new_instructiction(dest, "declare atomic");

	gas_add_instr_nr(dest, "add", CELL_SIZE, "sp");
}
void sk_declare_array_to_gas(FILE* dest, long size) {
	gas_new_instructiction(dest, "declare array");
	gas_add_instr_nr(dest, "add", size, "sp");
}
void sk_push_constant_to_gas(FILE* dest, long value) {
	gas_new_instructiction(dest, "push constant");

	gas_add_instr_n(dest, "push", value);
}
void sk_push_label_adress_to_gas(FILE* dest, char* label) {
	gas_new_instructiction(dest, "push label of adress");

	gas_add_instr_l(dest, "push", label);
}
void sk_push_relative_adress_to_gas(FILE* dest, long adress) {
	gas_new_instructiction(dest, "push relative adress");

	gas_add_instr_rr(dest, "mov", "ax", "fp");
	gas_add_instr_nr(dest, "add", adress, "fp");
	gas_add_instr_r(dest, "push", "ax");
}
void sk_push_absolute_adress_to_gas(FILE* dest, long adress) {
	gas_new_instructiction(dest, "push absolute adress");

	gas_add_instr_n(dest, "push", adress);
}
void sk_pop_to_gas(FILE* dest) {
	gas_new_instructiction(dest, "pop");

	gas_add_instr_r(dest, "pop", "ax");
}
void sk_duplicate_to_gas(FILE* dest) {
	gas_new_instructiction(dest, "duplicate");

	gas_add_instr_rr(dest, "mov", "sp", "ax");
	gas_add_instr_r(dest, "push", "ax");
}
void sk_unary_operation_to_gas(FILE* dest, TOKEN_TYPE_T oper) {
	gas_new_instructiction(dest, "unary operation");

	gas_add_instr_r(dest, "pop", "ax");
	gas_add_instr_r(dest, "TODO", "ax"); //TODO
	gas_add_instr_r(dest, "push", "ax");
}
void sk_binary_operation_to_gas(FILE* dest, TOKEN_TYPE_T oper) {
	gas_new_instructiction(dest, "binary operation");

	gas_add_instr_r(dest, "pop", "ax");
	gas_add_instr_r(dest, "pop", "bx");
	gas_add_instr_rr(dest, "TODO", "ax", "bx"); //TODO
	gas_add_instr_r(dest, "push", "ax");
}

void gas_new_instructiction(FILE* dest, char* comment) {
	fprintf(dest, "\n");
	fprintf(dest, "# %s \n", comment);
}
void gas_add_instr(FILE* dest, char* name) {
	fprintf(dest, "\t %s \n", name);
}
void gas_add_instr_r(FILE* dest, char* name, char* reg) {
	fprintf(dest, "\t %s  %s \n", name, reg);
}
void gas_add_instr_n(FILE* dest, char* name, long value) {
	fprintf(dest, "\t %s %d \n", name, value);
}
void gas_add_instr_l(FILE* dest, char* name, char* label) {
	fprintf(dest, "\t %s %s \n", name, label);
}
void gas_add_instr_rr(FILE* dest, char* name, char* reg1, char* reg2) {
	fprintf(dest, "\t %s %s, %s \n", name, reg1, reg2);
}
void gas_add_instr_nr(FILE* dest, char* name, long number, char* reg) {
	fprintf(dest, "\t %s %d, %s \n", name, number, reg);
}

#endif

