#include <stdio.h>
#include "../src/stackode.h"
#include "../src/ast-stackode-exporter.h"

void test_program_1(void);

int main(int argc, char **argv) {
	printf("Generating stackode: \n");

	test_program_1();

	printf("Done.\n");

	return 0;
}

void test_program_1(void) {
	sk_program_t* program = create_empty_program();

	add_instruction(program, //
			create_instruct_with_num(SKI_PUSH_NUMBER, 20));

	 add_instruction(program, //
			create_instruct_with_str(SKI_PUSH_ADDRES_OF, "x"));

	add_instruction(program, //
			create_instruction(SKI_ASSIGN));

	add_instruction(program, //
			create_instruct_with_str(SKI_PUSH_VALUE_OF, "x"));

	add_instruction(program, //
			create_instruct_with_str(SKI_PUSH_VALUE_OF, "print"));

	add_instruction(program, //
			create_instruction(SKI_CALL));

	printf("export to stackode not supported\n");
//	export_stackode(stdout, program);

}
