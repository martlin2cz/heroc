# Martin Jašek
# KMI UP
# LS 2016
# compiler of heroc language
# see vychodil.inf.upol.cz for more info

#-D LEXER_VERBOSE
#-D SYNTAXER_VERBOSE
MACROS=-D LEXER_VERBOSE -D SYNTAXER_VERBOSE


#std of compilation
STD=c99

# flags of compiler
CFLAGS=-std=${STD} ${MACROS}

#resulting heroc compiler binary name
BIN=heroc


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

compile: bin/ast.o bin/lexer.o
	echo "Compiling"
	
	
compile-tests: bin/test-lexer bin/test-syntaxer
	echo "Compiling tests"

#################################
bin/ast.o: src/ast.c
	${CC} ${CFLAGS} -o $@ -c $<

bin/lexer.o: gen/lexer.c bin/syntaxer.o
	${CC} ${CFLAGS} -o $@ -c $<

bin/syntaxer.o: gen/syntaxer.c
	${CC} ${CFLAGS} -o $@ -c $<

#################################
bin/test-lexer: bin/lexer.o bin/ast.o test/test-lexer.c  
	${CC} ${CFLAGS} -o $@ $^

bin/test-syntaxer: bin/lexer.o bin/syntaxer.o bin/ast.o test/test-syntaxer.c  
	${CC} ${CFLAGS} -o $@ $^


############################
#mkdir -p gen
#
#bison src/bison.yy
#mkdir -p bin
#gcc -std=${STD} -o bin/${BIN} src/main.c

