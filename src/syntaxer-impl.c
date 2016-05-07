#ifndef _SYNTAXER_IMPL_C
#define _SYNTAXER_IMPL_C

#include "syntaxer-headers.h"

int
yyerror (char *s)
{
    printf ("%s\n", s);
    return 0;
}


YYSTYPE
make_op (int op, YYSTYPE left, YYSTYPE right)
{
    YYSTYPE dst;
    dst = malloc (sizeof (_YYSTYPE));
    dst-> type = TYPE_OPER;
    dst-> value. oper. op = op;
    dst-> value. oper. left = left;
    dst-> value. oper. right = right;
    return dst;
}

#define RUN_OP(__OP) \
  eval_tree (node-> value. oper. left) __OP \
  eval_tree (node-> value. oper. right)

double
eval_tree (YYSTYPE node)
{
    if (node-> type == TYPE_DOUBLE) {
	return node-> value. dvalue;
    }
    if (node-> type != TYPE_OPER) {
	fprintf (stderr, "Unknown element type in ADT.\n");
	exit (1);
    }
    switch (node-> value. oper. op) {
    case OP_ADD:
	return RUN_OP (+);
    case OP_DIF:
	return RUN_OP (-);
    case OP_MUL:
	return RUN_OP (*);
    case OP_DIV:
	return RUN_OP (/);
    case OP_INV:
	return - eval_tree (node-> value. oper. left);
    case OP_POW:
    	fprintf(stderr, "Pow not supported, go damn!\n");
    	exit(1);
    default:
	fprintf (stderr, "Unknown operation code.\n");
	exit (1);
    }
}

char opname [6] = {'+', '-', '*', '/', '-', '^'};

void
print_tree (YYSTYPE node)
{
    if (node == NULL) {
	return;
    }
    if (node-> type == TYPE_DOUBLE) {
	printf ("%g", node-> value. dvalue);
	return;
    }
    if (node-> type == TYPE_OPER) {
	printf ("(%c ", opname [node-> value. oper. op - 1]);
	print_tree (node-> value. oper. left);
	if (node-> value. oper. right) {
	    printf (" ");
	    print_tree (node-> value. oper. right);
	}
	printf (")");
	return;
    }
    fprintf (stderr, "Unknown element type in ADT.\n");
    exit (1);
}

void
free_tree (YYSTYPE node)
{
    if (node == NULL) {
	return;
    }
    if (node-> type == TYPE_OPER) {
	free_tree (node-> value. oper. left);
	free_tree (node-> value. oper. right);
    }
    free (node);
}
//XXX
//int
//main (int argc, char **argv)
//{
//    yyparse ();
//    return 0;
//}

#endif
