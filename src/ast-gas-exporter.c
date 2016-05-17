#ifndef _AST_STACKODE_EXPORTER_C_
#define _AST_STACKODE_EXPORTER_C_

#include <stdio.h>
#include "ast-gas-exporter.h"
#include "stackode.h"

void ast_export_root(FILE* dest, struct ast_node_t* root) {
	sk_program_t* sk = ast_to_stackode(root);
	export_stackode_to_gas(dest, sk);

}

void export_stackode_to_gas(FILE* dest, sk_program_t* program) {
	fprintf("exporting stackode as a assembler...\n");
	//TODO
}


#endif

