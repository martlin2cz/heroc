## heroc
## Martin Jasek
## IV - V 2016
## KMI UP, inf.upol.cz
## vychodil.inf.upol.cz


## programs and params
CC	 = gcc
LEX	 = flex
YACC = bison

#-D LEXER_VERBOSE 
MACROS  = -D SYNTAXER_VERBOSE
#-Wall 
CFLAGS	= -ansi -pedantic -std=c11 $(MACROS)
LIBS	= -lfl -lm



## files specification

GRAMMAR	= src/syntaxer.y
LEXER	= src/lexer.l
SOURCE	= gen/syntaxer.c gen/lexer.c src/token.c src/tokens.c src/ast.c src/ast-basic-exporter.c src/misc.c #src/main.c
OBJECTS = obj/syntaxer.o obj/lexer.o obj/token.o obj/tokens.o obj/ast.o obj/ast-basic-exporter.o obj/misc.o #obj/main.o
TARGET	= bin/compiler

TESTSRC = test/test-lexer.c test/test-syntaxer.c test/test-ast.c   
TESTOBJ =  obj/test-lexer.o  obj/test-syntaxer.o  obj/test-ast.o
TESTTGT	= bin/test-lexer text-syntaxer
  

####### vzory
#
#.SUFFIXES: .y
#.y.c:
#	$(YACC) $(YFLAGS) -o $@ $<
#
#.SUFFIXES: .l
#.l.c:
#	$(LEX) -o$@ $<
#
#.SUFFIXES: .c
#.c.o:
#	$(CC) -c $(CFLAGS) $<
#

###########################
all: compiler tests

compiler: prepare $(LEXER) $(GRAMMAR) $(SOURCE) $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)
	strip $(TARGET)

tests: prepare $(GRAMMAR) $(LEXER) $(TESTSRC) $(TESTOBJ)
	$(CC) $(CFLAGS) -o bin/test-syntaxer  obj/test-syntaxer.o $(OBJECTS)
	$(CC) $(CFLAGS) -o bin/test-lexer  obj/test-lexer.o $(OBJECTS)
	$(CC) $(CFLAGS) -o bin/test-ast  obj/test-ast.o $(OBJECTS)
	

prepare:
	mkdir -p gen obj bin

clean:
	@rm -rf gen obj bin

	
###########################
gen/lexer.c: src/lexer.l
	$(LEX) -o$@ $<
	@rm -f src/lexer.c src/lexer.h #lexer dumps code in src folder as well, bug or feature?

gen/syntaxer.c: src/syntaxer.y
	$(YACC) -o$@ $<

###########################
### compile libs
obj/syntaxer.o: gen/syntaxer.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	

obj/lexer.o: gen/lexer.c
	${CC} -c ${CFLAGS} -o $@ -c $<


obj/token.o: src/token.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast.o: src/ast.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/tokens.o: src/tokens.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast-basic-exporter.o: src/ast-basic-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/misc.o: src/misc.c
	${CC} -c ${CFLAGS} -o $@ -c $<
		
#obj/main.o: src/main.c
#	${CC} -c ${CFLAGS} -o $@ -c $<
### compile tests
	
obj/test-lexer.o: test/test-lexer.c
	${CC} -c ${CFLAGS} -o $@ -c $<	
	
obj/test-syntaxer.o: test/test-syntaxer.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	
obj/test-ast.o: test/test-ast.c
	${CC} -c ${CFLAGS} -o $@ -c $<		