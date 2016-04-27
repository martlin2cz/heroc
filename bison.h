/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_NUMBER = 42,
     TK_STRING = 4168,
     TK_VARIABLE = 4169,
     TK_COMMENT = 4170,
     TK_COMA = 4171,
     TK_SEMICOLON = 4172,
     TK_CURLY_BRACKET_OPEN = 4173,
     TK_CURLY_BRACKET_CLOSE = 4174,
     TK_ROUND_BRACKET_OPEN = 4175,
     TK_ROUND_BRACKET_CLOSE = 4176,
     TK_OPERATOR_PLUS = 4113,
     TK_OPERATOR_MINUS = 4114,
     TK_OPERATOR_TIMES = 4115,
     TK_OPERATOR_DIVIDE = 4116,
     TK_OPERATOR_MODULO = 4117,
     TK_OPERATOR_INCREMENT = 4118,
     TK_OPERATOR_DECREMENT = 4119,
     TK_OPERATOR_EQUAL = 4129,
     TK_OPERATOR_NOT_EQUAL = 4130,
     TK_OPERATOR_LESS_THAN = 4131,
     TK_OPERATOR_GREATER_THAN = 4132,
     TK_OPERATOR_LESS_EQUAL = 4133,
     TK_OPERATOR_GREATER_EQUAL = 4134,
     TK_OPERATOR_NOT = 4145,
     TK_OPERATOR_AND = 4146,
     TK_OPERATOR_OR = 4147,
     TK_OPERATOR_BIT_NOT = 4161,
     TK_OPERATOR_BIT_AND = 4162,
     TK_OPERATOR_BIT_OR = 4163,
     TK_OPERATOR_BIT_XOR = 4164,
     TK_OPERATOR_SHIFT_LEFT = 4165,
     TK_OPERATOR_SHIFT_RIGHT = 4166,
     TK_OPERATOR_ASSIGNMENT = 4096,
     TK_KEYW_IF = 4177,
     TK_KEYW_ELSE = 4178,
     TK_KEYW_FOR = 4179,
     TK_KEYW_DO = 4180,
     TK_KEYW_WHILE = 4181,
     TK_KEYW_CONTINUE = 4182,
     TK_KEYW_BREAK = 4183,
     TK_KEYW_PRINT = 4184,
     TK_KEYW_SCAN = 4185,
     TK_MISSING = 4186,
     TK_UNKNOWN = 4187
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


