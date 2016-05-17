#ifndef _AST_STACKODE_EXPORTER_C_
#define _AST_STACKODE_EXPORTER_C_

#include <stdio.h>
#include "ast-stackode-exporter.h"
#include "tokens.h"

void ast_export_root(FILE* dest, struct ast_node_t* root) {
	sk_program_t *stackode = ast_to_stackode(root);
	export_stackode(dest, stackode);
}

void export_stackode(FILE* dest, sk_program_t *program) {
	int i;
	for (i = 0; i < (program->count); i++) {
		export_instruction(dest, program->instructions[i]);
	}
}

void export_instruction(FILE* dest, sk_instruction_t *instruction) {
	switch (instruction->type) {

	case SKI_LABEL:
		print_instr_str(dest, "label", instruction->value.string);
		break;
	case SKI_PUSH_NO_VALUE:
		print_instr(dest, "push-no-value");
		break;
	case SKI_PUSH_NUMBER:
		print_instr_num(dest, "push-number", instruction->value.number);
		break;
	case SKI_PUSH_VALUE_OF:
		print_instr_str(dest, "push-value", instruction->value.string);
		break;
	case SKI_PUSH_ADDRES_OF:
		print_instr_str(dest, "push-address", instruction->value.string);
		break;
	case SKI_ASSIGN:
		print_instr(dest, "assign");
		break;
	case SKI_CALL:
		print_instr(dest, "call");
		break;
	case SKI_RETURN:
		print_instr(dest, "return");
		break;
	case SKI_JUMP_TO:
		print_instr(dest, "jump-to");
		break;
	case SKI_JUMP_IF:
		print_instr(dest, "jump-if");
		break;
	case SKI_BINARY_OP:
		print_instr_oper(dest, "binary", instruction->value.number);
		break;
	case SKI_UNARY_OP:
		print_instr_oper(dest, "unary", instruction->value.number);
		break;
	default:
		fprintf(dest, "Unknown stackode instruction %d\n", instruction->type);
	}
}

void print_instr(FILE* dest, char* name) {
	fprintf(dest, "(%s)\n", name);
}

void print_instr_str(FILE* dest, char* name, char* str) {
	fprintf(dest, "(%s %s)\n", name, str);
}

void print_instr_num(FILE* dest, char* name, long num) {
	fprintf(dest, "(%s %ld)\n", name, num);
}

void print_instr_oper(FILE* dest, char* name, TOKEN_TYPE_T oper) {
	const char* op_str = to_string(oper);
	fprintf(dest, "(%s-%s)\n", name, op_str);
}

#endif
