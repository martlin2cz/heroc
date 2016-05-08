#include <stdio.h>
#include "../src/ast.h"
#include "../src/ast-exporter.h"

ast_node_t* create_arithmetic_tree();

int main(int argc, char **argv) {
	printf("Running ast tests: \n");

	printf("=== TREE 1: === \n");
	ast_node_t* tree1 = create_arithmetic_tree();
	ast_export_root(stdout, tree1);

	printf("Done.\n");

	return 0;
}


ast_node_t* create_arithmetic_tree() {
	ast_node_t* num1 = create_number(42l);
	ast_node_t* num2 = create_number(99);
	ast_node_t* id1 = create_identifier("foo");

	ast_node_t* oper1 = create_unary(NTO_MINUS, num1);
	ast_node_t* oper2 = create_binary(NTO_TIMES, id1, num2);
	ast_node_t* oper3 = create_binary(NTO_MODULO, oper1, oper2);

	return oper3;
}
