#!/bin/bash
# compiles and runs generated (compiled) gas code
# Martin Jasek, VII/2016

#timeout (in secs) to test run
TIMEOUT=100

make tests --eval="OUTPUTLANG=gas" --eval="MACROS= "

for F in $@; do

	echo "compiling file: $F into gas"
	./test-bin/test-compile-to < $F > tmp/compiled.s
	echo "compiled"

	echo "compiling gas"
	gcc lib/herocio.c tmp/compiled.s -o tmp/to-run.bin
	echo "compiled"


	echo "================"
	timeout $TIMEOUT ./tmp/to-run.bin
	echo "================"
done
