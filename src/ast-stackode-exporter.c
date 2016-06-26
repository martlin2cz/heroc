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

	export_sk_header(dest);

	for (i = 0; i < (program->count); i++) {
		export_sk_instruction(dest, program->instructions[i]);
	}

	export_sk_footer(dest);
}

void export_sk_header(FILE* dest) {
	fprintf(dest, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
	fprintf(dest, ";; Stackode program, generated \n");
	fprintf(dest, "(sc-program '(\n");
}

void export_sk_footer(FILE* dest) {
	fprintf(dest, "))\n");
	fprintf(dest, ";; end of generated Stackode program\n");
	fprintf(dest, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
}

void export_sk_instruction(FILE* dest, sk_instruction_t *instruction) {
	switch (instruction->type) {
	case SKI_COMMENT:
		print_sk_instr_str(dest, "comment", instruction->value.string);
		break;
	case SKI_LABEL:
		print_sk_instr_str(dest, "label", instruction->value.string);
		break;
	case SKI_CALL:
		print_sk_instr(dest, "call");
		break;
	case SKI_RETURN:
		print_sk_instr(dest, "return");
		break;
	case SKI_CLEANUP_AFTER_CALL:
		print_sk_instr_num(dest, "cleanup-after-call", instruction->value.number);
		break;
	case SKI_JUMP_ON_ZERO:
		print_sk_instr_str(dest, "jump-on-zero", instruction->value.string);
		break;
	case SKI_JUMP_ON_NON_ZERO:
		print_sk_instr_str(dest, "jump-on-non-zero", instruction->value.string);
		break;
	case SKI_JUMP_TO:
		print_sk_instr_str(dest, "jump-to", instruction->value.string);
		break;
	case SKI_LOAD:
		print_sk_instr(dest, "load");
		break;
	case SKI_ASSIGN:
		print_sk_instr(dest, "assign");
		break;
	case SKI_DECLARE_ATOMIC:
		print_sk_instr(dest, "declare-atomic");
		break;
	case SKI_DECLARE_ARRAY:
		print_sk_instr_num(dest, "declare-array", instruction->value.number);
		break;
	case SKI_PUSH_CONSTANT:
		print_sk_instr_num(dest, "push-constant", instruction->value.number);
		break;
	case SKI_PUSH_LABEL_ADRESS:
		print_sk_instr_str(dest, "push-label-adress", instruction->value.string);
		break;
	case SKI_PUSH_RELATIVE_ADRESS:
		print_sk_instr_num(dest, "push-relative-adress", instruction->value.number);
		break;
	case SKI_PUSH_ABSOLUTE_ADRESS:
		print_sk_instr_num(dest, "push-absolute-adress", instruction->value.number);
		break;
	case SKI_POP:
		print_sk_instr_num(dest, "pop", instruction->value.number);
		break;
	case SKI_UNARY_OPERATION:
		print_sk_instr_oper(dest, "unary-operation", instruction->value.number);
		break;
	case SKI_BINARY_OPERATION:
		print_sk_instr_oper(dest, "binary-operation", instruction->value.number);
		break;
	default:
		fprintf(dest, "esi: Unknown stackode instruction %d\n", instruction->type);
	}
}

void print_sk_instr(FILE* dest, char* name) {
	fprintf(dest, "\t(%s)\n", name);
}

void print_sk_instr_str(FILE* dest, char* name, char* str) {
	fprintf(dest, "\t(%s %s)\n", name, str);
}

void print_sk_instr_num(FILE* dest, char* name, long num) {
	fprintf(dest, "\t(%s %ld)\n", name, num);
}

void print_sk_instr_oper(FILE* dest, char* name, TOKEN_TYPE_T oper) {
	const char* op_str = to_string(oper);
	fprintf(dest, "\t(%s %s)\n", name, op_str);
}

#endif
