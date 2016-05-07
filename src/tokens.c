#ifndef _TOKENS_C_
#define _TOKENS_C_
#include <stdlib.h>
#include "tokens.h"





TOKEN_TYPE_T str_to_oper(char* str) {
	return str[0];
}

char* oper_to_str(TOKEN_TYPE_T oper){
	char* arr = (char*) malloc(2);
	arr[0] = oper;
	arr[1] = '\0';
	return arr;
}

TOKEN_TYPE_T str_to_bracket(char* str) {
	return str[0];
}

char* bracket_to_str(TOKEN_TYPE_T bra){
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
