#ifndef _TOKENS_C_
#define _TOKENS_C_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokens.h"

/******************************************************************************/
/* TYPES */
int is_atomic(TOKEN_TYPE_T type) {
	return type > 1000 && type < 2000;
}
int is_operator(TOKEN_TYPE_T type) {
	return type > 2000 && type < 3000;
}
int is_construct(TOKEN_TYPE_T type) {
	return type > 3000 && type < 4000;
}

/******************************************************************************/
/* OPERATOR */

TOKEN_TYPE_T str_to_oper(char* str) {
	switch (strlen(str)) {
	case 1:
		return one_char_str_to_oper(str[0]);
	case 2:
		return two_char_str_to_oper(str[0], str[1]);
	case 3:
		return three_char_str_to_oper(str[0], str[1], str[2]);
	default:
		fprintf(stderr, "Unsupported operator's lenght: %s\n", str);
		return 0;
	}
}

TOKEN_TYPE_T one_char_str_to_oper(char c1) {
	switch (c1) {
	case '+':
		return NTO_PLUS;
	case '-':
		return NTO_MINUS;
	case '*':
		return NTO_TIMES;
	case '/':
		return NTO_DIVIDE;
	case '%':
		return NTO_MODULO;
	default:
		fprintf(stderr, "Unsupported operator of 1 char: %c\n", c1);
		return 0;
	}
}
TOKEN_TYPE_T two_char_str_to_oper(char c1, char c2) {
	fprintf(stderr, "Not yet implemented two_char_str_to_oper \n");
	return 0;
}
TOKEN_TYPE_T three_char_str_to_oper(char c1, char c2, char c3) {
	fprintf(stderr, "Not yet implemented two_char_str_to_oper \n");
	return 0;
}

char* oper_to_str(TOKEN_TYPE_T oper) {
	switch (oper) {
	case NTO_PLUS:
		return "+";
	case NTO_MINUS:
		return "-";
	case NTO_TIMES:
		return "*";
	case NTO_DIVIDE:
		return "/";
	case NTO_MODULO:
		return "%";
	default:
		fprintf(stderr, "Unsupported operator of type: %d\n", oper);
		return NULL;
	}
}

/******************************************************************************/
/* MULTIOPERATOR */

TOKEN_TYPE_T str_to_multioper(char* str) {
	switch (strlen(str)) {
	case 1:
		return one_char_str_to_moper(str[0]);
	case 2:
		return two_char_str_to_moper(str[0], str[1]);
	default:
		fprintf(stderr, "Unsupported operator's lenght: %s\n", str);
		return 0;
	}
}

TOKEN_TYPE_T one_char_str_to_moper(char c1) {
	switch (c1) {
	case '-':
		return TT_MINUS;
	case '*':
		return TT_STAR;
	default:
		fprintf(stderr, "Unsupported operator of 1 char: %c\n", c1);
		return 0;
	}
}

TOKEN_TYPE_T two_char_str_to_moper(char c1, char c2) {
	if (c1 == '+' && c2 == '+') {
		return TT_INCREMENT;
	}
	if (c1 == '-' && c2 == '-') {
		return TT_DECREMENT;
	}

	fprintf(stderr, "Not supported operator: %c%c\n", c1, c2);
	return 0;
}

char* multioper_to_str(TOKEN_TYPE_T moper) {
	char* arr = (char*) malloc(2);
	arr[0] = moper;
	arr[1] = '\0';
	return arr;

}

TOKEN_TYPE_T str_to_bracket(char* str) {
	return str[0];
}

char* bracket_to_str(TOKEN_TYPE_T bra) {
	char* arr = (char*) malloc(2);
	arr[0] = bra;
	arr[1] = '\0';
	return arr;
}

TOKEN_TYPE_T str_to_construct(char* str) {
	return -1;	//TODO
}
char* construct_to_str(TOKEN_TYPE_T construct) {
	return NULL;	//TODO
}

#endif
