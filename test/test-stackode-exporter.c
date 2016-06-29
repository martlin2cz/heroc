#include <stdio.h>
#include "../src/stackode.h"
#include "../src/ast-stackode-exporter.h"

void test_program_1(void);

int main(int argc, char **argv) {
	fprintf(stderr, "Generating stackode: \n");

	test_program_1();

	fprintf(stderr, "Done.\n");

	return 0;
}

void test_program_1(void) {
	sk_program_t* program = create_empty_program();

	add_instruction(program, //
			create_instruction(SKI_DECLARE_ATOMIC));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_CONSTANT, 20));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_RELATIVE_ADRESS, +4));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_CONSTANT, 2));

	add_instruction(program, //
			create_instruct_with_op(SKI_BINARY_OPERATION, OPT_PLUS));

	add_instruction(program, //
			create_instruction(SKI_ASSIGN));

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_RELATIVE_ADRESS, +4));

	add_instruction(program, //
			create_instruction(SKI_LOAD));

	add_instruction(program, //
			create_instruct_with_str(SKI_PUSH_LABEL_ADRESS, "print_long"));

	add_instruction(program, //
			create_instruction(SKI_CALL));


	printf("Link stackode source and uncomment at line %d", (__LINE__ + 1));

	//export_stackode(stdout, program); //FIXME cannot compile without stackode linked


}
