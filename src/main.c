/*made by C write 'n' run 2.0*/
  /**
		Martin Jašek
		KMI UP
		LS 2015
	 */

#include <stdio.h>
#include <string.h>

#include "syntax-tree-node.h"
#include "flex-headers.h"
#include "bison-headers.h"

#include "../gen/flex.c"
#include "../gen/bison.c"


#include "eval.h"

#define VERSION "2.1"

FILE* yyin;
int print_tree = 0;

int process(FILE* stream) {
	int result;

	flush();
	flex_init();

	syntax_node_t* root = NULL;
	yyin = stream;
	result = yyparse(&root);

	if (result != 0) {
		fprintf(stderr, "An error %d occured during source code parsing. Aborted.\n", result);
		return 2;
	}

	if (print_tree) {
		printf("--- parsed tree ---\n");
		print_root(root);
		printf("--- end of tree ---\n");
	}

	result = evaluate(root);
	if (result != 0) {
		fprintf(stderr, "An error %d occured during evaluation. Aborted.\n", result);
		return 3;
	}

	printf("Bye.\n");
	return 0;
	
}

void help() {
	printf("mikroC            (read from stdin)\n");
	printf("mikroC [FILE.uc]  (read from file FILE.uc)\n");
	printf("mikroC --help     (print help)\n");
	printf("mikroC --version  (print version)\n");
}

void version() {
	printf("mikroC interpreter %s\n", VERSION);
	printf("Martin Jasek, may-june & july 2015\n");
}

int main (int argc, char* argv[]) {
	verbose_lex = 0;			/* ma byt bison upovidany? */
	print_tree = 0;				/* ma se vypsat zparsovany syntakticky strom? */
	define_unused = 0;		/* pouziti neinicializovane promene ma vest k jeji inicializaci (jinak chyba)? */
	print_each_tree = 0;	/* ma v kazdem kroku vypoctu vypisovat vyhodnocovany uzel? */

	FILE* file = stdin;

	if (argc > 1) {
		if (strcmp(argv[1], "--help") == 0 ) {
			help();
			return 0;
		} else if (strcmp(argv[1], "--version") == 0 ) {
			version();
			return 0;
		} else {
			file = fopen(argv[1], "r");	
			if (!file) {
				fprintf(stderr, "File not opened, Abort.\n");
				return 1;
			}
		}
	}

	int result = process(file);

	if (file != stdin)
		fclose(file);

	return result;
}
