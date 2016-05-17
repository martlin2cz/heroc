## heroc
## Martin Jasek
## IV - V 2016
## KMI UP, inf.upol.cz
## vychodil.inf.upol.cz


## programs and params
CC	 = gcc
LEX	 = flex
YACC = bison

# -D LEXER_VERBOSE -D SYNTAXER_VERBOSE  
MACROS	?= #-D LEXER_VERBOSE -D SYNTAXER_VERBOSE
# basic|scheme|stackode|gas
EXPORT	?= stackode
#-Wall 
CFLAGS	= -ansi -pedantic -std=c11 $(MACROS)
LIBS	= -lfl -lm



## files specification

GRAMMAR	= src/syntaxer.y
LEXER	= src/lexer.l
SOURCE	= gen/syntaxer.c gen/lexer.c src/token.c src/tokens.c src/ast.c src/stackode.c src/ast-${EXPORT}-exporter.c src/misc.c
OBJECTS = obj/syntaxer.o obj/lexer.o obj/token.o obj/tokens.o obj/ast.o obj/stackode.o obj/ast-${EXPORT}-exporter.o obj/misc.o 
TARGET	= bin/compiler

CMPSRC	=  src/compiler-main.c
CMPOBJ	=  obj/compiler-main.o  
CMPTGT	=  bin/compiler 

TESTSRC = test/test-lexer.c test/test-syntaxer.c test/test-ast.c test/test-stackode.c test/test-gas.c      
TESTOBJ =  obj/test-lexer.o  obj/test-syntaxer.o  obj/test-ast.o  obj/test-stackode.o  obj/test-gas.o
TESTTGT	=  test-bin/test-lexer test-bin/text-syntaxer test-bin/test-ast test-bin/test-stackode test-bin/test-gas
  

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

compiler: prepare $(LEXER) $(GRAMMAR) $(SOURCE) $(CMPSRC) $(OBJECTS) $(CMPOBJ)
	$(CC) -o $(CMPTGT) $(OBJECTS) $(CMPOBJ) $(LIBS)
	strip $(TARGET)

tests: prepare $(GRAMMAR) $(LEXER) $(TESTSRC) $(TESTOBJ)
	$(CC) $(CFLAGS) -o test-bin/test-syntaxer  obj/test-syntaxer.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-lexer  obj/test-lexer.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-ast  obj/test-ast.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-stackode  obj/test-stackode.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-gas  obj/test-gas.o $(OBJECTS)
	

prepare:
	mkdir -p gen obj bin test-bin tmp

clean:
	@rm -rf gen obj bin test-bin tmp

	
###########################
gen/lexer.c: src/lexer.l
	$(LEX) -o$@ $<
	@rm -f src/lexer.c src/lexer.h #lexer dumps code in src folder as well, bug or feature?

gen/syntaxer.c: src/syntaxer.y
	$(YACC) -Wall -o$@ $<

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

obj/stackode.o: src/stackode.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/gas.o: src/gas.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast-basic-exporter.o: src/ast-basic-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast-scheme-exporter.o: src/ast-scheme-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast-stackode-exporter.o: src/ast-stackode-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast-gas-exporter.o: src/ast-gas-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/misc.o: src/misc.c
	${CC} -c ${CFLAGS} -o $@ -c $<
		
obj/compiler-main.o: src/compiler-main.c
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
	
obj/test-stackode.o: test/test-stackode.c
	${CC} -c ${CFLAGS} -o $@ -c $<	
	
obj/test-gas.o: test/test-gas.c
	${CC} -c ${CFLAGS} -o $@ -c $<			