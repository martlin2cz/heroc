#ifndef _TOKEN_H
#define _TOKEN_H

#define TYPE_DOUBLE 0
#define TYPE_OPER   1

#define OP_ADD 1
#define OP_DIF 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_INV 5
#define OP_POW 6

/* definice struktury udrzujici pro atributy */
typedef struct _YYSTYPE {
    int type;  /* typ semanticke hodnoty */
    union {
	double dvalue;  /* konstanta typu double */
	struct oper_t {
	    int op;
	    struct _YYSTYPE *left, *right;
	} oper; /* operace a jeji argumenty */
    } value;
} _YYSTYPE;

typedef _YYSTYPE *_YYSTYPE_PTR;

/* definice typu hodnoty na semantickem zasobniku */
#define YYSTYPE _YYSTYPE_PTR

#endif
