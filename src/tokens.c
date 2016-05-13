#ifndef _TOKENS_C_
#define _TOKENS_C_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokens.h"

#define TOKENT_NOT_FOUND 0

#define JUST_LEXICALS_COUNT 100
#define COMMONS_COUNT 200
#define JUST_SEMANTICS_COUNT 300
#define OPERATORS_COUNT 400
#define ASSIGNMENTS_OPS_COUNT 500

const token_str_tuple_t just_lexicals[JUST_LEXICALS_COUNT] = //
		{ //

		/* special characters */
		{ JLT_COMMA, "," },	//
				{ JLT_SEMICOLON, ";" },	//
				{ JLT_QUESTION_MARK, "?" },	//
				{ JLT_COLON, ":" },	//
				{ JLT_AMPERSAND, "&" },	//
				{ JLT_ASSIGNMENT, "=" },	//

				/* brackets */
				{ JLT_NORMAL_LEFT_BRA, "(" },	//
				{ JLT_NORMAL_RIGHT_BRA, ")" },	//
				{ JLT_BLOCK_LEFT_BRA, "{" },	//
				{ JLT_BLOCK_RIGHT_BRA, "}" },	//
				{ JLT_INDEX_LEFT_BRA, "[" },	//
				{ JLT_INDEX_RIGHT_BRA, "]" },	//

				/* multi-used symbols */
				{ JLT_MINUS, "-" },	//
				{ JLT_PLUS, "+" },	//
				{ JLT_STAR, "*" },	//
				{ JLT_INCREMENT, "++" },	//
				{ JLT_DECREMENT, "--" }	//
		};

const token_str_tuple_t commons[COMMONS_COUNT] =	//
		{ //
		/* atomic values' tokens */
		{ ATT_NUMBER, "(number)" },	//
				{ ATT_STRING, "(string)" },	//
				{ ATT_IDENTIFIER, "(identifier)" },	//

				/* keywords and their particular control statements */
				{ STK_ASSIGNMENT, "=" },	//
				{ STK_TYPE, "long" },	//
				{ STK_SIZEOF, "sizeof" },	//
				{ STK_IF, "if" },	//
				{ STK_ELSE, "else" },	//
				{ STK_FOR, "for" },	//
				{ STK_WHILE, "while" },	//
				{ STK_DO, "do" },	//
				{ STK_RETURN, "return" },	//
				{ STK_BREAK, "break" },	//
				{ STK_CONTINUE, "continue" },	//
				{ STK_GOTO, "goto" },	//
				{ STK_LAMBDA, "lambda?" }	//

		};

const token_str_tuple_t just_semantics[JUST_SEMANTICS_COUNT] = //
		{ //
		/* just semantic tokens */
		{ JST_PROCEDURE, "(procedure)" },	//
				{ JST_PROCCALL, "(procedure call)" },	//
				{ JST_ATOMIC_VARIABLE_DECL, "(atomic variable)" },	//
				{ JST_ARRAY_VARIABLE_DECL, "(array variable)" },	//

				/* containers */
				{ CNT_STATEMENTS, "(block of statements)" },	//
				{ CNT_VARS_DECLS, "(variables declarations)" },	//
				{ CNT_NUMBERS, "(numbers)" },	//
				{ CNT_EXPRESSIONS, "(expressions)" },	//
				{ CNT_PARAMETERS, "(parameters list)" }	//

		};

const token_str_tuple_t operators[OPERATORS_COUNT] = //
		{ //
		{ OPT_PLUS, "+" },	//
				{ OPT_MINUS, "-" },	//
				{ OPT_TIMES, "*" },	//
				{ OPT_DIVIDE, "/" },	//
				{ OPT_MODULO, "%" },	//
				{ OPT_PRE_INCREMENT, "pre++" },	//
				{ OPT_PRE_DECREMENT, "pre--" },	//
				{ OPT_POST_INCREMENT, "post++" },	//
				{ OPT_POST_DECREMENT, "post++" },	//

				{ OPT_OR, "||" },	//
				{ OPT_AND, "&&" },	//
				{ OPT_NOT, "!" },	//
				{ OPT_BITWISE_OR, "|" },	//
				{ OPT_BITWISE_AND, "&" },	//
				{ OPT_BITWISE_NOT, "~" },	//
				{ OPT_BITWISE_XOR, "^" },	//
				{ OPT_SHIFT_LEFT, "<<" },	//
				{ OPT_SHIFT_RIGHT, ">>" },	//

				{ OPT_EQUAL, "==" },	//
				{ OPT_NOT_EQUAL, "!=" },	//
				{ OPT_LESS_THAN, "<" },	//
				{ OPT_GREATER_THAN, ">" },	//
				{ OPT_LESS_OR_EQUAL, "<=" },	//
				{ OPT_GREATER_OR_EQUAL, ">=" },	//

				{ OPT_TERNARY, "?" },	//
				{ OPT_REFERENCE, "&" },	//
				{ OPT_DEREFERENCE, "*" },	//
				{ OPT_INDEX, "[]" }	//
		};

const token_str_tuple_t asssignments_ops[ASSIGNMENTS_OPS_COUNT] = //
		{ //
		{ STK_ASSIGNMENT, "=" },	//
				{ OPT_PLUS, "+=" },	//
				{ OPT_MINUS, "-=" },	//
				{ OPT_TIMES, "*=" },	//
				{ OPT_DIVIDE, "/=" },	//
				{ OPT_MODULO, "%=" },	//

				{ OPT_OR, "|=" },	//
				{ OPT_AND, "&=" },	//
				{ OPT_BITWISE_NOT, "~=" },	//
				{ OPT_BITWISE_XOR, "^=" },	//
				{ OPT_SHIFT_LEFT, "<<=" },	//
				{ OPT_SHIFT_RIGHT, ">>=" }	//
		};

TOKEN_TYPE_T find_tok_in(char* str, const token_str_tuple_t tokens[], int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (strcmp(str, tokens[i].str) == 0) {
			return tokens[i].token;
		}
	}
	return TOKENT_NOT_FOUND;
}

const char* find_str_in(TOKEN_TYPE_T tok, const token_str_tuple_t tokens[],
		int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (tok == tokens[i].token) {
			return tokens[i].str;
		}
	}
	return NULL;
}

TOKEN_TYPE_T find_lexical(char* str) {
	TOKEN_TYPE_T result;

	result = find_tok_in(str, just_lexicals, JUST_LEXICALS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}
	result = find_tok_in(str, commons, COMMONS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}

	result = find_tok_in(str, operators, OPERATORS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}
	result = find_tok_in(str, asssignments_ops, ASSIGNMENTS_OPS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}

	fprintf(stderr, "Token %s not found\n", str);
	return TOKENT_NOT_FOUND;
}

TOKEN_TYPE_T find_semantic(char* str) {
	TOKEN_TYPE_T result;

	result = find_tok_in(str, just_semantics, JUST_SEMANTICS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}
	result = find_tok_in(str, commons, COMMONS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}

	result = find_tok_in(str, operators, OPERATORS_COUNT);
	if (result != TOKENT_NOT_FOUND) {
		return result;
	}

	fprintf(stderr, "Token %s not found\n", str);
	return TOKENT_NOT_FOUND;
}

const char* to_string(TOKEN_TYPE_T tok) {
	const char* result;
	result = find_str_in(tok, just_semantics, JUST_SEMANTICS_COUNT);
	if (result != NULL) {
		return result;
	}

	result = find_str_in(tok, commons, COMMONS_COUNT);
	if (result != NULL) {
		return result;
	}

	result = find_str_in(tok, operators, OPERATORS_COUNT);
	if (result != NULL) {
		return result;
	}

	fprintf(stderr, "Token %d not found\n", tok);
	return NULL;
}

int is_atomic(TOKEN_TYPE_T tok) {
	return tok > 2100 && tok < 2200;
}
#endif
