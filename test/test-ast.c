#include <stdio.h>
#include "../src/ast.h"
#include "../src/ast-exporter.h"

ast_node_t* create_arithmetic_tree(void);
ast_node_t* create_some_fn(void);

int main(int argc, char **argv) {
	printf("Running ast tests: \n");

	printf("=== TREE 1: === \n");
	ast_node_t* tree1 = create_arithmetic_tree();
	ast_export_root(stdout, tree1);

	printf("=== TREE 2: === \n");
	ast_node_t* tree2 = create_some_fn();
	ast_export_root(stdout, tree2);

	//TODO test arrays?

	printf("Done.\n");

	return 0;
}

ast_node_t* create_arithmetic_tree(void) {
	ast_node_t* num1 = create_number(42l);
	ast_node_t* num2 = create_number(99);
	ast_node_t* id1 = create_identifier("foo");

	ast_node_t* oper1 = create_unary(OPT_MINUS, num1);
	ast_node_t* oper2 = create_binary(OPT_TIMES, id1, num2);
	ast_node_t* oper3 = create_binary(OPT_MODULO, oper1, oper2);

	return oper3;
}

ast_node_t* create_some_fn(void) {

	//some ternary op
	ast_node_t* cond1A = create_binary(OPT_LESS_THAN, //
			create_identifier("q"), create_number(0));
	ast_node_t* pos1A = create_unary(OPT_MINUS, create_number(1));
	ast_node_t* neg1A = create_unary(OPT_PLUS, create_number(1));

	ast_node_t* tern1 = //
			create_ternar_op(cond1A, pos1A, neg1A);

	// first if
	ast_node_t* cond1 = //
			create_unary(OPT_NOT, create_identifier("q"));
	ast_node_t* asg1 = //
			create_assignment_with_op(OPT_TIMES, create_identifier("q"), tern1);
	ast_node_t* pos1 = create_block(asg1);
	ast_node_t* if1 = create_if(cond1, pos1);

	// second if
	ast_node_t* cond2AA = //
			create_binary(OPT_MODULO, create_identifier("q"), create_number(2));
	ast_node_t* cond2A = //
			create_binary(OPT_EQUAL, cond2AA, create_number(1));
	ast_node_t* cond2 = //
			create_binary(OPT_BITWISE_AND, cond2A, create_identifier("x"));
	/// the second if's body statements
	ast_node_t* kw1 = //
			create_keyword(STK_CONTINUE);
	ast_node_t* kw2 = //
			create_return(create_identifier("q"));
	ast_node_t* inc1 = //
			create_unary(OPT_POST_INCREMENT, create_identifier("q"));
	ast_node_t* args1 = //
			create_expressions(create_identifier("q"));
	ast_node_t* pc1 = //
			create_proccall(create_identifier("print"), args1);

	ast_node_t* pos2 = create_block( //
			prepend(inc1, prepend(kw1, NULL)));
	ast_node_t* neg2 = create_block( //
			prepend(pc1, prepend(kw2, NULL)));
	ast_node_t* if2 = create_if_else(cond2, pos2, neg2);

	// init, cond and inc of for loop

	ast_node_t* asg3 = //
			create_assignment(create_identifier("i"), create_number(0));
	ast_node_t* cond3 = //
			create_binary(OPT_LESS_THAN, create_identifier("i"),
					create_identifier("count"));
	ast_node_t* inc3 = //
			create_unary(OPT_POST_INCREMENT, create_identifier("i"));

	/// the body of for loop
	ast_node_t* args3 = //
			prepend(create_identifier("i"),
					prepend(create_identifier("count"), NULL));
	ast_node_t* pc3 = //
			create_proccall(create_identifier("progress"), args3);
	ast_node_t* body3 = //
			create_block(prepend(if1, prepend(if2, prepend(pc3, NULL))));

	ast_node_t* for3 = create_for(asg3, cond3, inc3, body3);

	// the whole procedure
	/// parameters list
	ast_node_t* params4 = create_parameters(	//
			prepend(create_identifier("x"), //
			prepend(create_identifier("count"), //
			NULL)));

	/// initial variables declaration
	ast_node_t* decl4A = create_declaration( //
			create_identifier("q"), create_number(1));
	ast_node_t* decl4B = create_declaration( //
			create_identifier("i"), NULL);

	ast_node_t* decl4 = //
			create_variables_decl( //
					prepend(decl4A, //
							prepend(decl4B, //
									NULL)));
	ast_node_t* body4 = create_block( //
			prepend(decl4, prepend(for3, NULL)));
	ast_node_t* proc = //
			create_procedure(create_identifier("some_proc"), params4, body4);

	return proc;

}
