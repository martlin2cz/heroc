#ifndef _TOKENS_C_
#define _TOKENS_C_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokens.h"

/******************************************************************************/
/* TYPES */
int is_atomic(TOKEN_TYPE_T type) {
	return type > 1000 && type < 1100;
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
		return one_char_str_to_oper(str, str[0]);
	case 2:
		return two_char_str_to_oper(str, str[0], str[1]);
	default:
		UNSUPPORTED_STR(str)
	}
}

TOKEN_TYPE_T one_char_str_to_oper(char* str, char c1) {
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
	case '!':
		return NTO_NOT;
	case '|':
		return NTO_BITWISE_OR;
	case '&':
		return NTO_BITWISE_AND;
	case '~':
		return NTO_BITWISE_NOT;
	case '^':
		return NTO_BITWISE_XOR;
	case '<':
		return NTO_LESS_THAN;
	case '>':
		return NTO_GREATER_THAN;
	default:
		UNSUPPORTED_STR(str)
	}
}
TOKEN_TYPE_T two_char_str_to_oper(char* str, char c1, char c2) {
	if (c1 == '|' && c2 == '|') {
		return NTO_OR;
	}
	if (c1 == '&' && c2 == '&') {
		return NTO_AND;
	}

	if (c1 == '=' && c2 == '=') {
		return NTO_EQUAL;
	}
	if (c1 == '!' && c2 == '=') {
		return NTO_NOT_EQUAL;
	}
	if (c1 == '<' && c2 == '=') {
		return NTO_LESS_OR_EQUAL;
	}
	if (c1 == '>' && c2 == '=') {
		return NTO_GREATER_OR_EQUAL;
	}
	if (c1 == '>' && c2 == '>') {
		return NTO_SHIFT_LEFT;
	}
	if (c1 == '<' && c2 == '<') {
		return NTO_SHIFT_RIGHT;
	}

	UNSUPPORTED_STR(str)
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
	case NTO_POST_INCREMENT:
	case NTO_PRE_INCREMENT:
		return "++";
	case NTO_POST_DECREMENT:
	case NTO_PRE_DECREMENT:
		return "--";
	case NTO_OR:
		return "||";
	case NTO_AND:
		return "&&";
	case NTO_NOT:
		return "!";
	case NTO_BITWISE_OR:
		return "||";
	case NTO_BITWISE_AND:
		return "&&";
	case NTO_BITWISE_NOT:
		return "~";
	case NTO_BITWISE_XOR:
		return "^";
	case NTO_EQUAL:
		return "==";
	case NTO_NOT_EQUAL:
		return "!=";
	case NTO_LESS_THAN:
		return "<";
	case NTO_GREATER_THAN:
		return ">";
	case NTO_LESS_OR_EQUAL:
		return "<=";
	case NTO_GREATER_OR_EQUAL:
		return ">=";
	case NTO_SHIFT_LEFT:
		return "<<";
	case NTO_SHIFT_RIGHT:
		return ">>";
	default:
		UNSUPPORTED_TYPE(oper)
	}
}

/******************************************************************************/
/* MULTIOPERATOR */

TOKEN_TYPE_T str_to_multioper(char* str) {
	switch (strlen(str)) {
	case 1:
		return one_char_str_to_moper(str, str[0]);
	case 2:
		return two_char_str_to_moper(str, str[0], str[1]);
	default:
		UNSUPPORTED_STR(str)
	}
}

TOKEN_TYPE_T one_char_str_to_moper(char* str, char c1) {
	switch (c1) {
	case '-':
		return TT_MINUS;
	case '+':
		return TT_PLUS;
	case '*':
		return TT_STAR;
	default:
		UNSUPPORTED_STR(str)

	}
}

TOKEN_TYPE_T two_char_str_to_moper(char* str, char c1, char c2) {
	if (c1 == '+' && c2 == '+') {
		return TT_INCREMENT;
	}
	if (c1 == '-' && c2 == '-') {
		return TT_DECREMENT;
	}

	UNSUPPORTED_STR(str)
}

char* multioper_to_str(TOKEN_TYPE_T moper) {
	switch (moper) {
	case TT_MINUS:
		return "-";
	case TT_PLUS:
		return "+";
	case TT_STAR:
		return "*";
	case TT_INCREMENT:
		return "++";
	case TT_DECREMENT:
		return "--";
	default:
		UNSUPPORTED_TYPE(moper)
	}

}

/******************************************************************************/
/* ASSIGNMENT OPERATOR */
TOKEN_TYPE_T str_to_asgoper(char* str) {
	switch (strlen(str)) {
	case 1:
		return one_char_str_to_asgoper(str, str[0]);
	case 2:
		return two_char_str_to_asgoper(str, str[0], str[1]);
	case 3:
		return three_char_str_to_asgoper(str, str[0], str[1], str[2]);
	default:
		UNSUPPORTED_STR(str)
	}
}

TOKEN_TYPE_T one_char_str_to_asgoper(char* str, char c1) {
	if (c1 == '=') {
		return NTS_ASSIGNMENT;
	}

	UNSUPPORTED_STR(str)
}
TOKEN_TYPE_T two_char_str_to_asgoper(char* str, char c1, char c2) {
	if (c2 != '=') {
		UNSUPPORTED_STR(str);
	}
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
	case '&':
		return NTO_BITWISE_AND;
	case '|':
		return NTO_BITWISE_OR;
	case '^':
		return NTO_BITWISE_XOR;
	case '~':
		return NTO_BITWISE_NOT;
	case '!':
		return NTO_NOT;
	default:
		UNSUPPORTED_STR(str)
	}

}
TOKEN_TYPE_T three_char_str_to_asgoper(char* str, char c1, char c2, char c3) {
	if (c3 != '=') {
		UNSUPPORTED_STR(str);
	}
	if (c1 == '<' && c2 == '<') {
		return str_to_oper("<<");
	}

	if (c1 == '>' && c2 == '>') {
		return str_to_oper(">>");
	}

	UNSUPPORTED_STR(str)
}

char* asgoper_to_str(TOKEN_TYPE_T asgoper) {
	switch (asgoper) {
	case NTO_PLUS:
		return "+=";
	case NTO_MINUS:
		return "-=";
	case NTO_TIMES:
		return "*=";
	case NTO_DIVIDE:
		return "/=";
	case NTO_MODULO:
		return "%=";
	case NTO_BITWISE_AND:
		return "&=";
	case NTO_BITWISE_OR:
		return "|=";
	case NTO_BITWISE_XOR:
		return "^=";
	case NTO_BITWISE_NOT:
		return "~=";
	case NTO_NOT:
		return "!=";
	case NTO_SHIFT_LEFT:
		return "<<=";
	case NTO_SHIFT_RIGHT:
		return ">>=";
	default:
		UNSUPPORTED_TYPE(asgoper)
	}
}

/******************************************************************************/
/* BRACKETS */

TOKEN_TYPE_T str_to_bracket(char* str) {
	if (strlen(str) != 1) {
		UNSUPPORTED_STR(str);
	}

	switch (str[0]) {
	case '(':
		return TT_NORMAL_LEFT_BRA;
	case ')':
		return TT_NORMAL_RIGHT_BRA;
	case '{':
		return TT_BLOCK_LEFT_BRA;
	case '}':
		return TT_BLOCK_RIGHT_BRA;
	case '[':
		return TT_INDEX_LEFT_BRA;
	case ']':
		return TT_INDEX_RIGHT_BRA;
	default:
		UNSUPPORTED_STR(str)
	}
}

char* bracket_to_str(TOKEN_TYPE_T bra) {
	switch (bra) {
	case TT_NORMAL_LEFT_BRA:
		return "(";
	case TT_NORMAL_RIGHT_BRA:
		return ")";
	case TT_BLOCK_LEFT_BRA:
		return "{";
	case TT_BLOCK_RIGHT_BRA:
		return "}";
	case TT_INDEX_LEFT_BRA:
		return "[";
	case TT_INDEX_RIGHT_BRA:
		return "]";
	default:
		UNSUPPORTED_TYPE(bra)
	}
}

/******************************************************************************/
/* SPECIAL CHARACTERS */

TOKEN_TYPE_T str_to_specsym(char* str) {
	if (strlen(str) != 1) {
		UNSUPPORTED_STR(str);
	}

	switch (str[0]) {
	case ',':
		return TT_COMMA;
	case ';':
		return TT_SEMICOLON;
	case '?':
		return TT_QUESTION_MARK;
	case ':':
		return TT_COLON;
	case '&':
		return TT_AMPERSAND;
	default:
		UNSUPPORTED_STR(str)
	}
}

char* specsym_to_str(TOKEN_TYPE_T specsym) {
	switch (specsym) {
	case TT_COMMA:
		return ",";
	case TT_SEMICOLON:
		return ";";
	case TT_QUESTION_MARK:
		return "?";
	case TT_COLON:
		return ":";
	case TT_AMPERSAND:
		return "&";
	default:
		UNSUPPORTED_TYPE(specsym)
	}
}

/******************************************************************************/
/* KEYWORDS AND SYNTACTIC CONSTRUCTS */

TOKEN_TYPE_T str_to_keyword(char* keyw) {
	if (strcmp(keyw, "long") == 0) {
		return NTS_TYPEDEF;
	}
	if (strcmp(keyw, "sizeof") == 0) {
		return NTS_SIZEOF;
	}
	if (strcmp(keyw, "if") == 0) {
		return NTS_IF;
	}
	if (strcmp(keyw, "else") == 0) {
		return NTS_ELSE;
	}
	if (strcmp(keyw, "for") == 0) {
		return NTS_FOR;
	}
	if (strcmp(keyw, "while") == 0) {
		return NTS_WHILE;
	}
	if (strcmp(keyw, "do") == 0) {
		return NTS_DO;
	}
	if (strcmp(keyw, "return") == 0) {
		return NTS_RETURN;
	}
	if (strcmp(keyw, "break") == 0) {
		return NTS_BREAK;
	}
	if (strcmp(keyw, "continue") == 0) {
		return NTS_CONTINUE;
	}
	if (strcmp(keyw, "goto") == 0) {
		return NTS_GOTO;
	}
	if (strcmp(keyw, "lambda") == 0) {
		return NTS_LAMBDA;
	}

	UNSUPPORTED_STR(keyw)
}
char* keyword_to_str(TOKEN_TYPE_T keyw) {
	switch (keyw) {
	case NTS_TYPEDEF:
		return "long";
	case NTS_SIZEOF:
		return "sizeof";
	case NTS_IF:
		return "if";
	case NTS_ELSE:
		return "else";
	case NTS_FOR:
		return "for";
	case NTS_WHILE:
		return "while";
	case NTS_DO:
		return "do";
	case NTS_RETURN:
		return "return";
	case NTS_BREAK:
		return "break";
	case NTS_CONTINUE:
		return "continue";
	case NTS_GOTO:
		return "goto";
	case NTS_LAMBDA:
		return "lambda";
	default:
		UNSUPPORTED_TYPE(keyw)
	}
}

#endif
