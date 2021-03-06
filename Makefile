## heroc
## Martin Jasek
## IV - VII 2016
## KMI UP, inf.upol.cz
## vychodil.inf.upol.cz
###########################################################
## programs and params
CC	 = gcc
LEX	 = flex
YACC = bison

## verbocity of compiler components
# -D LEXER_VERBOSE -D SYNTAXER_VERBOSE  -D SEMANTER_VERBOSE  -D STACKODE_VERBOSE -D GAS_VERBOSE
VERBOSEDS ?=

## output language, one of following:
# basic|scheme|stackode|gas
OUTPUTLANG ?= gas

###########################################################

MACROS	?= $(VERBOSEDS) -D OUTPUTLANG="\"$(OUTPUTLANG)"\"

#-Wall -d
CFLAGS	= -ansi -pedantic -std=c11 $(MACROS)

LIBS	= -lfl -lm

###########################################################
## files specification

GRAMMAR	= src/syntaxer.y
LEXER	= src/lexer.l
SOURCE	= gen/syntaxer.c gen/lexer.c src/token.c src/tokens.c src/ast.c src/ast-displayer.c src/semanter.c src/stackode.c src/ast-${OUTPUTLANG}-exporter.c src/misc.c
OBJECTS = obj/syntaxer.o obj/lexer.o obj/token.o obj/tokens.o obj/ast.o obj/ast-displayer.o obj/semanter.o obj/stackode.o obj/ast-${OUTPUTLANG}-exporter.o obj/misc.o 
TARGET	= bin/compiler

CMPSRC	=  src/compiler-main.c
CMPOBJ	=  obj/compiler-main.o  
CMPTGT	=  bin/compiler 

TESTSRC = test/test-lexer.c test/test-syntaxer.c test/test-semanter.c test/test-ast.c test/test-stackode-exporter.c test/test-gas-exporter.c   test/test-compile-to.c      
TESTOBJ =  obj/test-lexer.o  obj/test-syntaxer.o obj/test-semanter.o  obj/test-ast.o  obj/test-stackode-exporter.o obj/test-gas-exporter.o     obj/test-compile-to.o
TESTTGT	=  test-bin/test-lexer test-bin/text-syntaxer test-bin/text-semanter test-bin/test-ast test-bin/test-stackode-exporter test-bin/test-compile-to
  
###########################################################
all: compiler tests

compiler: prepare $(LEXER) $(GRAMMAR) $(SOURCE) $(OBJECTS) $(CMPSRC) $(CMPOBJ)
	$(CC) -o $(CMPTGT) $(OBJECTS) $(CMPOBJ) $(LIBS)
	strip $(TARGET)

tests: prepare $(LEXER) $(GRAMMAR) $(SOURCE) $(OBJECTS) $(TESTSRC) $(TESTOBJ)
	$(CC) $(CFLAGS) -o test-bin/test-ast  obj/test-ast.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-lexer  obj/test-lexer.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-syntaxer  obj/test-syntaxer.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-semanter  obj/test-semanter.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-stackode-exporter  obj/test-stackode-exporter.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-gas-exporter  obj/test-gas-exporter.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test-bin/test-compile-to  obj/test-compile-to.o $(OBJECTS)
	

prepare:
	mkdir -p gen obj bin test-bin tmp

clean:
	@rm -rf gen obj test-bin tmp

	

###########################################################

syntaxgraph: prepare gen/syntaxer.dot
	dot -Tpng gen/syntaxer.dot -o gen/syntaxer.png

doc:
	cd doc
	make
	
###########################################################
gen/lexer.c: src/lexer.l
	$(LEX) -o$@ $<
	@rm -f src/lexer.c src/lexer.h #lexer dumps code in src folder as well, bug or feature?

gen/syntaxer.c: src/syntaxer.y
	$(YACC) -Wall -o$@ $<

gen/syntaxer.dot: src/syntaxer.y
	$(YACC) -Wall --graph -v $<
	
###########################################################
### compile libs
obj/syntaxer.o: gen/syntaxer.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	
	

obj/lexer.o: gen/lexer.c
	${CC} -c ${CFLAGS} -o $@ -c $<


obj/token.o: src/token.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/ast.o: src/ast.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	
obj/ast-displayer.o: src/ast-displayer.c
	${CC} -c ${CFLAGS} -o $@ -c $<		

obj/tokens.o: src/tokens.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	
obj/semanter.o: src/semanter.c
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
		
###########################################################
### compile tests
	
obj/test-lexer.o: test/test-lexer.c
	${CC} -c ${CFLAGS} -o $@ -c $<	
	
obj/test-syntaxer.o: test/test-syntaxer.c
	${CC} -c ${CFLAGS} -o $@ -c $<

obj/test-semanter.o: test/test-semanter.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	
obj/test-ast.o: test/test-ast.c
	${CC} -c ${CFLAGS} -o $@ -c $<
	
obj/test-stackode-exporter.o: test/test-stackode-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	

obj/test-gas-exporter.o: test/test-gas-exporter.c
	${CC} -c ${CFLAGS} -o $@ -c $<	


obj/test-compile-to.o: test/test-compile-to.c
	${CC} -c ${CFLAGS} -o $@ -c $<	
	
		