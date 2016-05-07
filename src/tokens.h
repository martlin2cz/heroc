#ifndef _TOKENS_H_
#define _TOKENS_H_

#define TOKEN_TYPE_T int
/*
// basic (leaves with additional values) tokens
#define NT_UNSPECIFIED	11
#define NT_NUMBER 		12
#define NT_STRING 		13
#define NT_IDENTIFIER 	14
#define NT_FUNCTION 	15

// control symbols
#define NT_COMMA 		111
#define NT_SEMICOLON	112
#define NT_STAR			113

// parenthesis (brackets)
#define NTB_NORMAL_OPEN 	121
#define NTB_NORMAL_CLOSE 	122
#define NTB_BLOCK_OPEN 		123
#define NTB_BLOCK_CLOSE 	124
#define NTB_INDEX_OPEN 		125
#define NTB_INDEX_CLOSE 	126

//TODO for, if, break, ...

// operators (common)
#define NTO_ASSIGMENT 	1001
#define NTO_REFERENCE 	1002
#define NTO_DEREFERENCE 	1003
#define NTO_TERNARY_THEN 	1004
#define NTO_TERNARY_ELSE 	1005

// basic arithmetic operators
#define NTO_PLUS 	1101
#define NTO_MINUS 	1102
#define NTO_TIMES 	1103
#define NTO_DIVIDE 	1104
#define NTO_MODULO 	1105
// incementation and decrementation
#define NTO_INCREMENT 	1201
#define NTO_DECREMENT 	1202
#define NTO_PRE_INCREMENT 	1203
#define NTO_PRE_DECREMENT 	1204
#define NTO_POST_INCREMENT 	1205
#define NTO_POST_DECREMENT 	1206

// binary operators
#define NTO_OR 		1301
#define NTO_AND 	1302
#define NTO_NOT 	1303
// bitwise operators
#define NTO_BITOR 	1401
#define NTO_BITAND 	1402
#define NTO_BITNOT 	1403
#define NTO_BITXOR 	1404
//shift and rotations
#define NTO_ROTATE_LEFT 	1501
#define NTO_ROTATE_RIGHT 	1502
#define NTO_SHIFT_LEFT 		1503
#define NTO_SHIFT_RIGHT 	1504
//relational operators
#define NTO_LESS_THAN 	1601
#define NTO_LESS_EQUAL 	1602
#define NTO_GRATER_THAN 	1603
#define NTO_GRATER_EQUAL 	1604
#define NTO_EQUAL 		1605
#define NTO_NOT_EQUAL	1606
*/
TOKEN_TYPE_T str_to_oper(char* str);
char* oper_to_str(TOKEN_TYPE_T oper);

TOKEN_TYPE_T str_to_bracket(char* str);
char* bracket_to_str(TOKEN_TYPE_T bra);


TOKEN_TYPE_T str_to_construct(char* str);
char* construct_to_str(TOKEN_TYPE_T construct);

#endif
