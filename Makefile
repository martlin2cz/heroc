# Martin Jašek
# KMI UP, inf.upol.cz
# summer semmester 2016
#
# compiler of heroc language
# see vychodil.inf.upol.cz for more info

#-D LEXER_VERBOSE
#-D SYNTAXER_VERBOSE
MACROS=-D LEXER_VERBOSE -D SYNTAXER_VERBOSE


#std of compilation
STD=c99

# flags of compiler
CFLAGS=-std=${STD} ${MACROS} -Wall

#resulting heroc compiler binary name
BIN=heroc

#################################
CC=gcc
#################################
.SUFFIXES: .c
.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
#################################
all: prepare tests compiler

prepare:
	mkdir -p gen
	mkdir -p bin

compiler: build
	
tests: build compile-tests
		
clean:
	rm -rf gen bin
#################################	
build: lexit syntaxit compile
	

lexit:
	flex src/lexer.l
	
syntaxit:
	bison src/syntaxer.yy

compile: src/ast.c src/token.c gen/lexer.c gen/syntaxer.c  bin/ast.o bin/token.o bin/lexer.o bin/syntaxer.o
	$(CC) $(CFLAGS) -o bin/something bin/ast.o bin/token.o bin/lexer.o bin/syntaxer.o -lfl
	
	
#compile: //bin/ast.o bin/lexer.o bin/syntaxer.o
#	//echo "Compiling"
	
compile-tests: bin/test-lexer bin/test-syntaxer
	echo "Compiling tests"

#################################
bin/token.o: src/token.c
	${CC} ${CFLAGS} -o $@ -c $<

bin/ast.o: src/ast.c
	${CC} ${CFLAGS} -o $@ -c $<

bin/lexer.o: gen/lexer.c 
	${CC} ${CFLAGS} -lfl -o $@ -c $<

bin/syntaxer.o: gen/syntaxer.c 
	${CC} ${CFLAGS} -lfl -o $@ -c $<

#################################
bin/test-lexer: bin/token.o bin/ast.o bin/lexer.o  test/test-lexer.c  
	${CC}  ${CFLAGS} -o $@ $^

bin/test-syntaxer: bin/syntaxer.o test/test-syntaxer.c  
	${CC} ${CFLAGS} -o $@ $^


############################
#mkdir -p gen
#bison src/bison.yy
#mkdir -p bin
#gcc -std=${STD} -o bin/${BIN} src/main.c

