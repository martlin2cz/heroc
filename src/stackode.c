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

	single_node_to_stackode(program, root);

	sk_instruction_t* end_instr = create_instruction(SKI_END);
	add_instruction(program, end_instr);

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
	STACKODE_LOG("Generating stackode of node %d %s", node->uid,
			to_string(node->type));

	switch (node->type) {
	case JST_PROGRAM:
		program_to_sackode(program, node);
		break;
	case ATT_NUMBER:
		number_to_stackode(program, node);
		break;
	case STK_ASSIGNMENT:
		assignment_to_stackode(program, node);
		break;
	case JST_EXPRESSION:
		expression_to_stackode(program, node);
		break;
	case STK_SIZEOF:
		sizeof_to_stackode(program, node);
		break;
	case STK_IF:
		if_to_stackode(program, node, "if");
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

		//special operators
	case OPT_AND:
		and_to_stackode(program, node);
		break;
	case OPT_OR:
		or_to_stackode(program, node);
		break;
	case OPT_TERNARY:
		ternary_to_stackode(program, node);
		break;
	case OPT_DEREFERENCE:
		dereference_to_stackode(program, node);
		break;
	case OPT_REFERENCE:
		reference_to_stackode(program, node);
		break;
	case OPT_PRE_INCREMENT:
	case OPT_PRE_DECREMENT:
	case OPT_POST_INCREMENT:
	case OPT_POST_DECREMENT:
		inc_dec_to_stackode(program, node);
		break;
	case JST_INVOKE_EXTERNAL:
		invoke_external_to_stackode(program, node);
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

void program_to_sackode(sk_program_t* program, ast_node_t* node) {
	ast_node_t* decls = node->value.child;
	list_to_stackode(program, decls);
}

void node_to_stackode_comment(sk_program_t* program, ast_node_t* node) {
	char* text = (char*) malloc(100 * sizeof(char));
	if (!text) {
		fprintf(stderr, "ntsc: Cannot allocate memory\n");
	}

	sprintf(text, "\"node %d, %s\"", node->uid, to_string(node->type));

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

	sk_instruction_t* instr = create_instruction(SKI_PUSH_CELL_SIZE);
	add_instruction(program, instr);
}

void if_to_stackode(sk_program_t * program, ast_node_t * node, char* label_base) {
	node_to_stackode_comment(program, node);

	char* else_label = generate_label("else", label_base, node->uid);
	char* after_label = generate_label("after", label_base, node->uid);

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

	char* start_label = generate_label("start", "loop", node->uid);
	char* body_end_label = generate_label("body_end", "loop", node->uid);
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

	sk_instruction_t* lbl_body_end = create_instruct_with_str(SKI_LABEL,
			body_end_label);
	add_instruction(program, lbl_body_end);

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

	char* start_label = generate_label("start", "loop", node->uid);
	char* body_end_label = generate_label("body_end", "loop", node->uid);
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

	sk_instruction_t* lbl_body_end = create_instruct_with_str(SKI_LABEL,
			body_end_label);
	add_instruction(program, lbl_body_end);

	sk_instruction_t* jmp2start = create_instruct_with_str(SKI_JUMP_TO,
			start_label);
	add_instruction(program, jmp2start);

	sk_instruction_t* lbl_after = create_instruct_with_str(SKI_LABEL,
			after_label);
	add_instruction(program, lbl_after);
}

void do_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	char* start_label = generate_label("start", "loop", node->uid);
	char* body_end_label = generate_label("body_end", "loop", node->uid);
	char* after_label = generate_label("after", "loop", node->uid);

	sk_instruction_t* lbl_start = create_instruct_with_str(SKI_LABEL,
			start_label);
	add_instruction(program, lbl_start);

	//body
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* lbl_body_end = create_instruct_with_str(SKI_LABEL,
			body_end_label);
	add_instruction(program, lbl_body_end);

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
	if (node->value.child) {
		single_node_to_stackode(program, node->value.child);
	} else {
		sk_instruction_t* empty = create_instruction(SKI_DECLARE_ATOMIC);
		add_instruction(program, empty);
	}

	sk_instruction_t* ret = create_instruction(SKI_RETURN);
	add_instruction(program, ret);
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

	char* start_label = generate_label("body_end", "loop", loop_node->uid);
	sk_instruction_t* jmp2start = create_instruct_with_str(SKI_JUMP_TO,
			start_label);
	add_instruction(program, jmp2start);

}
void variable_to_stackode(sk_program_t * program, ast_node_t * node) {
	YYSTYPE decl = find_value_of_meta(node, META_DECLARATION);
	ast_node_t* init_val = decl.child->value.child->next;

	/*if (init_val && init_val->type == JST_PROCEDURE
	 && init_val->value.child->type == JST_VARIABLE) {
	 char* label = label_of_proc(init_val);

	 sk_instruction_t* pa = create_instruct_with_str(SKI_PUSH_LABEL_ADRESS,
	 label);
	 add_instruction(program, pa);
	 } else {
	 */
	YYSTYPE type = find_value_of_meta(decl.child, META_VAR_TYPE);
	YYSTYPE addr = find_value_of_meta(decl.child, META_ADRESS);

	sk_instr_type_t instr_type;
	if (type.number == VT_GLOBAL) {
		instr_type = SKI_PUSH_ABSOLUTE_ADRESS;
	} else {
		instr_type = SKI_PUSH_RELATIVE_ADRESS;
	}

	sk_instruction_t* pa = create_instruct_with_num(instr_type, addr.number);
	add_instruction(program, pa);

	//sk_instruction_t* load = create_instruction(SKI_LOAD);
	//add_instruction(program, load);
	/*
	 }
	 */
}

void array_to_stackode(sk_program_t * program, ast_node_t * node) {
	//HACK: the array is stackoded by pushing its address preceded by its items

	long size = node->value.child->value.number;
	ast_node_t* items_or_meta = node->value.child->next;

	if (items_or_meta && !is_meta(items_or_meta->type)) {
		ast_node_t* items =  items_or_meta->value.child;
		ast_node_t* reversed = reverse(items);
		list_to_stackode(program, reversed);
	} else {
		sk_instruction_t* alloc = create_instruct_with_num(SKI_DECLARE_ARRAY,
				size);
		add_instruction(program, alloc);
	}

	YYSTYPE var_addr = find_value_of_meta(node, META_ADRESS);
	YYSTYPE var_type = find_value_of_meta(node, META_VAR_TYPE);

	sk_instr_type_t pav_instr_type;
	if (var_type.number == VT_GLOBAL) {
		pav_instr_type = SKI_PUSH_ABSOLUTE_ADRESS;
	} else {
		pav_instr_type = SKI_PUSH_RELATIVE_ADRESS;
	}

	long addr = var_addr.number + size - 1;
	sk_instruction_t* pav = create_instruct_with_num(pav_instr_type,
			addr);

	add_instruction(program, pav);
}

void procedure_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	//labels
	char* proc_label = label_of_proc(node);

	sk_instruction_t* push_addr = create_instruct_with_str(
			SKI_PUSH_LABEL_ADRESS, proc_label);
	add_instruction(program, push_addr);

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
	sk_instruction_t* ret_cmt = create_instruct_with_str(SKI_COMMENT,
			"\"implicit return\"");
	add_instruction(program, ret_cmt);

	sk_instruction_t* push_retval = create_instruction(SKI_DECLARE_ATOMIC);
	add_instruction(program, push_retval);

	sk_instruction_t* ret = create_instruction(SKI_RETURN);
	add_instruction(program, ret);

	sk_instruction_t* after = create_instruct_with_str(SKI_LABEL, after_label);
	add_instruction(program, after);

	if (node->value.child->type == STK_LAMBDA) {
		sk_instruction_t* push_label = create_instruct_with_str(
				SKI_PUSH_LABEL_ADRESS, proc_label);
		add_instruction(program, push_label);
	}
}

void proccall_to_stackode(sk_program_t * program, ast_node_t * node) {
	node_to_stackode_comment(program, node);

	list_to_stackode(program, node->value.child->next->value.child);

	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* call = create_instruction(SKI_CALL);
	add_instruction(program, call);

	int argc = lenght_of(node->value.child->next->value.child);

	sk_instruction_t* cleanup = create_instruct_with_num(SKI_CLEANUP_AFTER_CALL,
			argc);
	add_instruction(program, cleanup);
}

void var_decl_to_stackode(sk_program_t * program, ast_node_t * node) {
	ast_node_t* init_val = node->value.child->next;

	if (init_val && init_val->type == JST_PROCEDURE
			&& init_val->value.child->type == JST_VARIABLE) {

		procedure_to_stackode(program, init_val);
	} else {

		sk_instruction_t* instr = create_instruction(SKI_DECLARE_ATOMIC);
		add_instruction(program, instr);
	}

}
void unary_op_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* instr = create_instruct_with_op(SKI_UNARY_OPERATION,
			node->type);
	add_instruction(program, instr);
}
void binary_op_to_stackode(sk_program_t * program, ast_node_t * node) {

	if (!(node->value.child->next)) {
		sk_instruction_t* zero = create_instruct_with_num(SKI_PUSH_CONSTANT, 0);
		add_instruction(program, zero);

		sk_instruction_t* cmt = create_instruct_with_str(SKI_COMMENT,
				"no second operand specified");
		add_instruction(program, cmt);

	}

	single_node_to_stackode(program, node->value.child);
	if (node->value.child->next) {
		single_node_to_stackode(program, node->value.child->next);
	}

	sk_instruction_t* instr = create_instruct_with_op(SKI_BINARY_OPERATION,
			node->type);
	add_instruction(program, instr);
}

//specials

void and_to_stackode(sk_program_t * program, ast_node_t * node) {
	//if a then b else 0
	ast_node_t* as_if = create_with_1_children(STK_IF, node->value.child);

	YYSTYPE zero = { 0 };
	append_child(as_if, ATT_NUMBER, zero);

	if_to_stackode(program, as_if, "and");
}

void or_to_stackode(sk_program_t * program, ast_node_t * node) {
	//if a then a else b
	// push a; duplicate a as a'; if a' > goto end; pop a; push b; end:

	char* after_label = generate_label("after", "or", node->uid);

	single_node_to_stackode(program, node->value.child);
	sk_instruction_t* duplicate_a = create_instruction(SKI_DUPLICATE);
	add_instruction(program, duplicate_a);

	sk_instruction_t* jmp = create_instruct_with_str(SKI_JUMP_ON_NON_ZERO,
			after_label);
	add_instruction(program, jmp);

	sk_instruction_t* pop_a = create_instruction(SKI_POP);
	add_instruction(program, pop_a);

	single_node_to_stackode(program, node->value.child->next);

	sk_instruction_t* lbl_after = create_instruct_with_str(SKI_LABEL,
			after_label);
	add_instruction(program, lbl_after);
}

void ternary_to_stackode(sk_program_t * program, ast_node_t * node) {
	if_to_stackode(program, node, "ternary");
}

void dereference_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* load = create_instruction(SKI_LOAD);
	add_instruction(program, load);
}

void reference_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child);
	/*
	 sk_instruction_t* last_instr = program->instructions[program->count - 1];
	 if (last_instr->type == SKI_LOAD) {
	 remove_last_instr(program);

	 } else if (last_instr->type == SKI_PUSH_LABEL_ADRESS) {
	 //ok, no reference needed

	 } else {
	 fprintf(stderr, "rts: Not a load instruction, it's %x\n",
	 last_instr->type);
	 }
	 */
}
void inc_dec_to_stackode(sk_program_t * program, ast_node_t * node) {
	single_node_to_stackode(program, node->value.child);
}

void expression_to_stackode(sk_program_t * program, ast_node_t * node) {
	sk_instruction_t* comment = create_instruct_with_str(SKI_COMMENT,
			"\"-- expression --\"");
	add_instruction(program, comment);

	single_node_to_stackode(program, node->value.child);

	sk_instruction_t* pop = create_instruction(SKI_POP);
	add_instruction(program, pop);
}

void invoke_external_to_stackode(sk_program_t * program, ast_node_t * node) {

	char* name = node->value.child->value.child->value.string;
	YYSTYPE argc = find_value_of_meta(node, META_ARITY_OF_EXTERNAL);

	sk_instruction_t* invoke = create_instruct_with_str_num(SKI_INVOKE_EXTERNAL,
			name, argc.number);
	add_instruction(program, invoke);
}

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

void remove_last_instr(sk_program_t* program) {
	free(program->instructions[program->count - 1]);
	program->instructions[program->count - 1] = NULL;

	program->count--;
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

sk_instruction_t* create_instruct_with_str_num(sk_instr_type_t type, char* str,
		long num) {
	sk_instruction_t* instr = (sk_instruction_t*) malloc(
			sizeof(sk_instr_type_t));

	instr->type = type;
	instr->value.string = str;
	instr->value2.number = num;
	return instr;
}

char* generate_label(char* prefix, char* name, int uid) {
	char* label = (char*) malloc(100 * sizeof(char));

	sprintf(label, "_%s_of_%s_%d", prefix, name, uid);

	return label;
}

char* label_of_proc(ast_node_t* node) {
	if (node->value.child->type == STK_LAMBDA) {
		return generate_label("start", "anonymous", node->uid);
	} else {
		return node->value.child->value.child->value.string;
	}
}

#endif
