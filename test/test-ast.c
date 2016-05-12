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

	printf("Done.\n");

	return 0;
}

ast_node_t* create_arithmetic_tree(void) {
	ast_node_t* num1 = create_number(42l);
	ast_node_t* num2 = create_number(99);
	ast_node_t* id1 = create_identifier("foo");

	ast_node_t* oper1 = create_unary(NTO_MINUS, num1);
	ast_node_t* oper2 = create_binary(NTO_TIMES, id1, num2);
	ast_node_t* oper3 = create_binary(NTO_MODULO, oper1, oper2);

	return oper3;
}
//
//
//	procedure (count, q):
//		x := 1
//		for i := 0, i < count, i++ do
//			if x = 10 then
//				++idq
//			else
//				break
//			end
//			if q mod 2 != 0 then
//				return 1
//			end
//			print(i, q);
//		end
//	end
//
ast_node_t* create_some_fn(void) {
	ast_node_t* params = //
			prepend(create_identifier("x"), //
			prepend(create_identifier("count"), //
			NULL));

	// initial variables declaration
	ast_node_t* decl1A = create_decl_assignment( //
			create_identifier("q"), create_number(1));
	ast_node_t* decl1B = create_decl_assignment( //
			create_identifier("i"), NULL);

	ast_node_t* decl1 = //
			prepend(decl1A, //
					prepend(decl1B, //
							NULL));

	//some ternary op
	ast_node_t* cond1A = create_binary(NTO_LESS_THAN, //
			create_identifier("q"), create_number(0));
	ast_node_t* pos1A = create_unary(NTO_MINUS, create_number(1));
	ast_node_t* neg1A = create_unary(NTO_PLUS, create_number(1));

	ast_node_t* tern1 = //
			create_ternar_op(cond1A, pos1A, neg1A);

	//first if
	ast_node_t* cond1 = //
			create_unary(NTO_NOT, create_identifier("q"));
	ast_node_t* pos1 = //
			create_assignment_with_op(NTO_TIMES, create_identifier("q"), tern1);
	ast_node_t* if1 = create_if(cond1, pos1);


	// init, cond and inc of for loop
	ast_node_t* asg1 = //
			create_assignment(create_identifier("i"), create_number(0));
	ast_node_t* cond3 = //
			create_binary(NTO_LESS_THAN, create_identifier("i"),
					create_identifier("count"));
	ast_node_t* inc1 = //
			create_unary(NTO_POST_INCREMENT, create_identifier("i"));
	ast_node_t* forb = //
			create_block(create_keyword(NTS_CONTINUE));

	ast_node_t* fors = create_for(asg1, cond3, inc1, forb);

	return tern1;

	/*
	 return create_procedure(
	 //params
	 prepend(create_identifier("x"),//
	 prepend(create_identifier("count"),//
	 NULL)),

	 //body
	 create_block(//
	 prepend(create_decl_assignment(//
	 create_identifier("x"), create_number(1)),//
	 prepend(create_keyword(NTS_BREAK),//
	 prepend(create_for(//
	 NULL, NULL, NULL, NULL),

	 NULL))));
	 */
	/*
	 ast_node_t* cmp2 = create_binary(NTO_EQUAL, create_identifier("x"), create_number(-1));
	 ast_node_t* incb2 = create_unary(NTO_PRE_INCREMENT, create_identifier("q"));
	 ast_node_t* kwb1 = create_keyword(NTS_BREAK);
	 ast_node_t* if1 = create_if_else(cmp2, incb2, kwb1);

	 ast_node_t* cmp3 = create_binary(NTO_NOT_EQUAL,
	 create_binary(NTO_MODULO, create_identifier("q"), create_number(2)), create_number(0));
	 ast_node_t* retb1 = create_return(create_number(-1));
	 ast_node_t* if2 = create_if(cmp3, retb1);

	 ast_node_t* asg2 = create_decl_assignment(create_identifier("i"), create_number(0));

	 ast_node_t* cmp1 = create_binary(NTO_LESS_THAN, create_identifier("i"), create_identifier("count"));
	 ast_node_t* inc1 = create_unary(NTO_POST_INCREMENT, create_identifier("i"));
	 ast_node_t* args1 = prepend(create_identifier("i"), prepend(create_identifier("q"), NULL));
	 ast_node_t* fc1 = create_funcall(create_identifier("print"), args1);
	 ast_node_t* body1 = prepend(if1, prepend(if2, prepend(fc1, NULL)));
	 ast_node_t* for1 = create_for(asg2, cmp1, inc1, body1);

	 ast_node_t* params1 = prepend(create_identifier("count"), prepend(create_identifier("q"), NULL));

	 ast_node_t* asg1 = create_decl_assignment(create_identifier("x"), create_number(1));
	 ast_node_t* body2 = prepend(asg1, prepend(for1, NULL));

	 ast_node_t* fn = create_procedure(params1, body2);

	 return asg2;
	 */
}
