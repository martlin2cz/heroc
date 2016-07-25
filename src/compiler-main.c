#ifndef _COMPILER_MAIN_C_
#define _COMPILER_MAIN_C_

#include <stdio.h>
#include <string.h>
#include "compiler-main.h"
#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "semanter.h"
#include "ast-exporter.h"

void print_help() {
	printf("compiler [SRC] [DEST]\n");
	printf("If SRC or DEST not specified stdin/stdout is used\n");
	printf("Compiler built-in specification:\n");
#ifdef OUTPUTLANG
	printf(" output language:  %s\n", OUTPUTLANG);
#endif

	printf(" verbosity specifiers:  ");
#ifdef LEXER_VERBOSE
	printf("lexer verbose, ");
#endif
#ifdef  SYNTAXER_VERBOSE
	printf("syntaxer verbose, ");
#endif
#ifdef  SEMANTER_VERBOSE
	printf("semanter verbose, ");
#endif
#ifdef  STACKODE_VERBOSE
	printf("stackode verbose, ");
#endif
#ifdef  GAS_VERBOSE
	printf("gas verbose, ");
#endif
	printf("\b\b. \n"); //m@rtlinova finta
}

void print_version() {
	printf("heroc compiler\n");
	printf("version 1.0, built at 25. 07. 2016\n");
	printf("Martin Jasek\n");
}

FILE* argv_to_file_or_not(int argc, char* argv[], int index, char* flags,
		FILE* dflt) {
	if (index >= argc) {
		return dflt;
	}

	char* name = argv[index];
	FILE* file = fopen(name, flags);
	if (!file) {
		fprintf(stderr, "File %s cannot be opened.\n", name);
		return NULL;
	}

	return file;
}

int main(int argc, char* argv[]) {

	if (argc == 2
			&& (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
		print_help();
		return 0;
	}

	if (argc == 2
			&& (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)) {
		print_version();
		return 0;
	}

	//TODO handle read from file
	FILE* src = argv_to_file_or_not(argc, argv, 1, "r+", stdin);
	FILE* dest = argv_to_file_or_not(argc, argv, 2, "w+", stdout);

	if (!src || !dest) {
		fprintf(stderr, "Aborting.\n");
		return 1;
	}

	yyin = src;

	struct ast_node_t* root = NULL;
	yyparse(&root);
	if (!root) {
		fprintf(stderr, "Aborting.\n");
		return 2;
	}

	int semantic_errors = analyze_tree(root);

	if (semantic_errors) {
		fprintf(stderr, "Aborting.\n");
		return 3;
	}

	ast_export_root(dest, root);

	if (src != stdin) {
		fclose(src);
	}
	if (dest != stdout) {
		fclose(dest);
	}

	return 0;
}

#endif
