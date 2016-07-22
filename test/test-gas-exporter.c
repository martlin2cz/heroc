#include <stdio.h>
#include "../src/stackode.h"
#include "../src/ast-gas-exporter.h"

void test_program_1(void);

int main(int argc, char **argv) {
	fprintf(stderr, "Generating gas: \n");

	test_program_1();

	fprintf(stderr, "Done.\n");

	return 0;
}

void test_program_1(void) {
	/*
	 case SKI_COMMENT: x
	 case SKI_LABEL: x
	 case SKI_CALL: x
	 case SKI_RETURN: x
	 case SKI_CLEANUP_AFTER_CALL: x
	 case SKI_JUMP_ON_ZERO: y
	 case SKI_JUMP_ON_NON_ZERO: y
	 case SKI_JUMP_TO: x
	 case SKI_LOAD:  x
	 case SKI_ASSIGN: x
	 case SKI_DECLARE_ATOMIC: x
	 case SKI_DECLARE_ARRAY: -
	 case SKI_PUSH_CONSTANT: x
	 case SKI_PUSH_LABEL_ADRESS:
	 case SKI_PUSH_RELATIVE_ADRESS: x
	 case SKI_PUSH_ABSOLUTE_ADRESS: -
	 case SKI_POP: x
	 case SKI_DUPLICATE: x
	 case SKI_UNARY_OPERATION: -
	 case SKI_BINARY_OPERATION:  x
	 */
	sk_program_t* program = create_empty_program();

	add_instruction(program, //
			create_instruct_with_str(SKI_JUMP_TO, "after_foo"));

	add_instruction(program, //
			create_instruct_with_str(SKI_LABEL, "foo"));

	add_instruction(program, //
			create_instruct_with_str(SKI_COMMENT, "declare local variable X"));

	add_instruction(program, //
			create_instruction(SKI_DECLARE_ATOMIC));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_RELATIVE_ADRESS, -3));

	add_instruction(program, //
			create_instruction(SKI_LOAD));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_RELATIVE_ADRESS, 0));

	add_instruction(program, //
			create_instruction(SKI_ASSIGN));

	add_instruction(program, //
			create_instruction(SKI_POP));

	add_instruction(program, //
			create_instruct_with_str(SKI_COMMENT, "'condition' (X modulo 2)"));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_RELATIVE_ADRESS, 0));

	add_instruction(program, //
			create_instruction(SKI_LOAD));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_CONSTANT, 2));

	add_instruction(program, //
			create_instruct_with_op(SKI_BINARY_OPERATION, OPT_MODULO));

	add_instruction(program, //
			create_instruction(SKI_DUPLICATE));

	add_instruction(program, //
			create_instruct_with_str(SKI_JUMP_ON_NON_ZERO, "bar"));

	add_instruction(program, //
			create_instruct_with_str(SKI_COMMENT, "positive branch"));

	//TODO -->
	add_instruction(program, //
			create_instruction(SKI_POP));
	//TODO <--

	add_instruction(program, //
			create_instruct_with_str(SKI_JUMP_TO, "baz"));

	add_instruction(program, //
			create_instruct_with_str(SKI_COMMENT, "negative branch"));

	add_instruction(program, //
			create_instruct_with_str(SKI_LABEL, "bar"));

	//TODO -->
	add_instruction(program, //
			create_instruction(SKI_POP));
	//TODO <--

	add_instruction(program, //
			create_instruct_with_str(SKI_LABEL, "baz"));

	add_instruction(program, //
			create_instruct_with_str(SKI_COMMENT, "after if"));

	add_instruction(program, //
			create_instruction(SKI_POP));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_CONSTANT, 42));

	add_instruction(program, //
			create_instruction(SKI_RETURN));

	add_instruction(program, //
			create_instruct_with_str(SKI_LABEL, "after_foo"));

	add_instruction(program, //
			create_instruct_with_str(SKI_JUMP_TO, "after_main"));

	add_instruction(program, //
			create_instruct_with_str(SKI_LABEL, "main"));

	add_instruction(
			program, //
			create_instruct_with_str(SKI_COMMENT,
					"after procedure foo (invoke foo)"));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_CONSTANT, 13));

	add_instruction(program, //
			create_instruct_with_str(SKI_PUSH_LABEL_ADRESS, "foo"));

	add_instruction(program, //
			create_instruction(SKI_CALL));

	add_instruction(program, //
			create_instruct_with_num(SKI_CLEANUP_AFTER_CALL, 1));

	add_instruction(program, //
			create_instruction(SKI_POP));

	add_instruction(program, //
			create_instruct_with_str(SKI_LABEL, "after_main"));

	//export_stackode_to_gas(stdout, program);
	//export_stackode(stdout, program);
	fprintf(stderr, "Link stackode source and uncomment at line %d\n",
			(__LINE__ - 1));

}
