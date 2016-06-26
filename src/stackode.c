#ifndef _STACKODE_C_
#define _STACKODE_C_

#include <stdlib.h>
#include <stdio.h>

#include "semanter.h"
#include "stackode.h"
#include "tokens.h"

/*********************************************************/
sk_program_t* ast_to_stackode(ast_node_t* root) {
	sk_program_t *program = create_empty_program();

	list_to_stackode(program, root);

	return program;
}

void list_to_stackode(sk_program_t* program, ast_node_t* nodes) {
	while (nodes) {
		if (!is_meta(nodes->type)) {
			single_node_to_stackode(program, nodes);
		}
		nodes = nodes->next;
	}
}

void single_node_to_stackode(sk_program_t* program, ast_node_t* node) {
	STACKODE_LOG("Generating stackode of node %d %s", node->uid, to_string(node->type));

	switch (node->type) {
	case ATT_NUMBER:
		number_to_stackode(program, node);
		break;
	case STK_ASSIGNMENT:
		assignment_to_stackode(program, node);
		break;
	case STK_SIZEOF:
		sizeof_to_stackode(program, node);
		break;
	case STK_IF:
		if_to_stackode(program, node);
		break;
	case STK_FOR:
		for_to_stackode(program, node);
		break;
	case STK_WHILE:
		while_to_stackode(program, node);
		break;
	case STK_DO:
		do_to_stackode(program, node);
		break;
	case STK_RETURN:
		return_to_stackode(program, node);
		break;
	case STK_BREAK:
		break_to_stackode(program, node);
		break;
	case STK_CONTINUE:
		continue_to_stackode(program, node);
		break;
	case STK_LAMBDA:
		lambda_to_stackode(program, node);
		break;
	case JST_VARIABLE:
		variable_to_stackode(program, node);
		break;
	case JST_ARRAY:
		array_to_stackode(program, node);
		break;
	case JST_PROCEDURE:
		procedure_to_stackode(program, node);
		break;
	case JST_PROCCALL:
		proccall_to_stackode(program, node);
		break;
	case JST_VARIABLE_DECL:
		var_decl_to_stackode(program, node);
		break;
	default:
		if (is_meta(node->type)) {
			//ignore
		} else if (is_unary_operator(node->type)) {
			unary_op_to_stackode(program, node);
		} else if (is_binary_operator(node->type)) {
			binary_op_to_stackode(program, node);
		} else if (is_container(node->type)) {
			list_to_stackode(program, node->value.child);
		} else {
			fprintf(stderr, "snts: Unknown node type %s\n",
					to_string(node->type));
		}
	}
}

void node_to_stackode_comment(sk_program_t* program, ast_node_t* node) {
	char* text = (char*) malloc(100 * sizeof(char));
	if (!text) {
		fprintf(stderr, "ntsc: Cannot alocate memory\n");
	}

	sprintf(text, "\"Node %d, %s\"", node->uid, to_string(node->type));

	sk_instruction_t* instr = create_instruct_with_str(SKI_COMMENT, text);
	add_instruction(program, instr);
}

void number_to_stackode(sk_program_t * program, ast_node_t * node) {
	sk_instruction_t* instr = create_instruct_with_num(SKI_PUSH_CONSTANT,
			node->value.number);
	add_instruction(program, instr);
}

void assignment_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child->next);
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* instr = create_instruction(SKI_ASSIGN);
	add_instruction(program, instr);
}

void sizeof_to_stackode(sk_program_t * program, ast_node_t * node) {
	long value = sizeof(long);
	sk_instruction_t* instr = create_instruct_with_num(SKI_PUSH_CONSTANT,
			value);	//TODO sizeof, what to do?
	add_instruction(program, instr);
}

void if_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	char* else_label = generate_label("else", "if", node->uid);
	char* after_label = generate_label("after", "if", node->uid);

	//condition
	single_node_to_stackode(program, node->value.child);
	sk_instruction_t* jmp2else = create_instruct_with_str(SKI_JUMP_ON_ZERO,
			else_label);
	add_instruction(program, jmp2else);

	//positive
	single_node_to_stackode(program, node->value.child->next);
	sk_instruction_t* jmp2end = create_instruct_with_str(SKI_JUMP_TO,
			after_label);
	add_instruction(program, jmp2end);

	//negative
	sk_instruction_t* lbl_else = create_instruct_with_str(SKI_LABEL,
			else_label);
	add_instruction(program, lbl_else);
	if (node->value.child->next->next) {
		single_node_to_stackode(program, node->value.child->next->next);
	} else {
		sk_instruction_t* no_else = create_instruct_with_str(SKI_COMMENT,
				"\"no negative branch\"");
		add_instruction(program, no_else);
	}

	sk_instruction_t* lbl_after = create_instruct_with_str(SKI_LABEL,
			after_label);
	add_instruction(program, lbl_after);
}

void for_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	char* start_label = generate_label("start", "for", node->uid);
	char* after_label = generate_label("after", "loop", node->uid);

	//init
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* lbl_start = create_instruct_with_str(SKI_LABEL,
			start_label);
	add_instruction(program, lbl_start);

	//condition
	single_node_to_stackode(program, node->value.child->next);
	sk_instruction_t* jmp2after = create_instruct_with_str(SKI_JUMP_ON_ZERO,
			after_label);
	add_instruction(program, jmp2after);

	//body
	single_node_to_stackode(program, node->value.child->next->next->next);

	//inc
	single_node_to_stackode(program, node->value.child->next->next);

	sk_instruction_t* jmp2start = create_instruct_with_str(SKI_JUMP_TO,
			start_label);
	add_instruction(program, jmp2start);

	sk_instruction_t* lbl_after = create_instruct_with_str(SKI_LABEL,
			after_label);
	add_instruction(program, lbl_after);
}

void while_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	char* start_label = generate_label("start", "for", node->uid);
	char* after_label = generate_label("after", "loop", node->uid);

	sk_instruction_t* lbl_start = create_instruct_with_str(SKI_LABEL,
			start_label);
	add_instruction(program, lbl_start);

	//condition
	single_node_to_stackode(program, node->value.child);
	sk_instruction_t* jmp2after = create_instruct_with_str(SKI_JUMP_ON_ZERO,
			after_label);
	add_instruction(program, jmp2after);

	//body
	single_node_to_stackode(program, node->value.child->next);

	sk_instruction_t* jmp2start = create_instruct_with_str(SKI_JUMP_TO,
			start_label);
	add_instruction(program, jmp2start);

	sk_instruction_t* lbl_after = create_instruct_with_str(SKI_LABEL,
			after_label);
	add_instruction(program, lbl_after);
}

void do_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	char* start_label = generate_label("start", "for", node->uid);
	char* after_label = generate_label("after", "loop", node->uid);

	sk_instruction_t* lbl_start = create_instruct_with_str(SKI_LABEL,
			start_label);
	add_instruction(program, lbl_start);

	//body
	single_node_to_stackode(program, node->value.child);

	//condition
	single_node_to_stackode(program, node->value.child->next);
	sk_instruction_t* jmp2start = create_instruct_with_str(SKI_JUMP_ON_NON_ZERO,
			start_label);
	add_instruction(program, jmp2start);

	sk_instruction_t* lbl_after = create_instruct_with_str(SKI_LABEL,
			after_label);
	add_instruction(program, lbl_after);
}

void return_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* instr = create_instruction(SKI_RETURN);
	add_instruction(program, instr);
}
void break_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	YYSTYPE loop = find_value_of_meta(node, META_LOOP);
	ast_node_t* loop_node = loop.child;

	char* after_label = generate_label("after", "loop", loop_node->uid);
	sk_instruction_t* jmp2after = create_instruct_with_str(SKI_JUMP_TO,
			after_label);
	add_instruction(program, jmp2after);

}
void continue_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	YYSTYPE loop = find_value_of_meta(node, META_LOOP);
	ast_node_t* loop_node = loop.child;

	char* start_label = generate_label("start", "loop", loop_node->uid);
	sk_instruction_t* jmp2start = create_instruct_with_str(SKI_JUMP_TO,
			start_label);
	add_instruction(program, jmp2start);

}
void lambda_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);
	//XXX, no?
}
void variable_to_stackode(sk_program_t * program, ast_node_t * node) {
	sk_instruction_t* xxxx = create_instruct_with_str(SKI_COMMENT,
			"TODO: variable here ...");
	add_instruction(program, xxxx);
	//TODO

}
void array_to_stackode(sk_program_t * program, ast_node_t * node) {
	//TODO
}
void procedure_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	//labels
	char* proc_label;
	if (node->value.child->type == STK_LAMBDA) {
		proc_label = generate_label("start", "anonymous", node->uid);
	} else {
		proc_label = node->value.child->value.child->value.string;
	}
	char* after_label = generate_label("after", "procedure", node->uid);

	//heading
	sk_instruction_t* skip_decl = create_instruct_with_str(SKI_JUMP_TO,
			after_label);
	add_instruction(program, skip_decl);

	sk_instruction_t* label = create_instruct_with_str(SKI_LABEL, proc_label);
	add_instruction(program, label);

	//body
	single_node_to_stackode(program, node->value.child->next->next);

	//after body
	sk_instruction_t* push_retval = create_instruction(SKI_DECLARE_ATOMIC);
	add_instruction(program, push_retval);

	sk_instruction_t* ret = create_instruction(SKI_RETURN);
	add_instruction(program, ret);

	sk_instruction_t* after = create_instruct_with_str(SKI_LABEL, after_label);
	add_instruction(program, after);
}

void proccall_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	single_node_to_stackode(program, node->value.child);

	list_to_stackode(program, node->value.child->next->value.child);

	sk_instruction_t* call = create_instruction(SKI_CALL);
	add_instruction(program, call);

	int argc = lenght_of(node->value.child->next->value.child);

	sk_instruction_t* cleanup = create_instruct_with_num(SKI_CLEANUP_AFTER_CALL,
			argc);
	add_instruction(program, cleanup);
}

void var_decl_to_stackode(sk_program_t * program, ast_node_t * node) {
	if (node->value.child->next
			&& node->value.child->next->type == JST_PROCEDURE) {

		procedure_to_stackode(program, node->value.child->next);
	} else {

		sk_instruction_t* instr = create_instruction(SKI_DECLARE_ATOMIC);//TODO if array ...
		add_instruction(program, instr);

		if (node->value.child->next
				&& !is_meta(node->value.child->next->type)) {

			ast_node_t* asg = create_with_1_children(STK_ASSIGNMENT,
					node->value.child);

			assignment_to_stackode(program, asg);
		}
	}

}
void unary_op_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* instr = create_instruct_with_op(SKI_UNARY_OPERATION,
			node->type);
	add_instruction(program, instr);
}
void binary_op_to_stackode(sk_program_t * program, ast_node_t * node) {
	//TODO FIXME check operands order
	single_node_to_stackode(program, node->value.child);
	if (node->value.child->next) {
		single_node_to_stackode(program, node->value.child->next);
	} else {
		sk_instruction_t* zero = create_instruct_with_num(SKI_PUSH_CONSTANT, 0);
		add_instruction(program, zero);

		sk_instruction_t* cmt = create_instruct_with_str(SKI_COMMENT,
				"no second operand specified");
		add_instruction(program, cmt);
	}

	sk_instruction_t* instr = create_instruct_with_op(SKI_BINARY_OPERATION,
			node->type);
	add_instruction(program, instr);
}

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
			fprintf(stderr, "cais: Memory allocation failed\n");
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

sk_instruction_t* create_instruct_with_op(sk_instr_type_t type,
TOKEN_TYPE_T oper) {
	return create_instruct_with_num(type, oper);
}

char* generate_label(char* prefix, char* name, int uid) {
	char* label = (char*) malloc(100 * sizeof(char));

	sprintf(label, "_%s_of_%s_%d", prefix, name, uid);

	return label;
}

#endif
