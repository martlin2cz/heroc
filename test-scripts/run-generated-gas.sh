#!/bin/bash
# compiles and runs generated (compiled) gas code
# Martin Jasek, VII/2016

#timeout (in secs) to test run (if enabled, see next)
TIMEOUT=5

#run specifier (run in gdb, run with timeout, run normally), uncomment one
#RUN_SPEC=gdbtui
#RUN_SPEC="timeout $TIMEOUT"
#RUN_SPEC=


make tests --eval="OUTPUTLANG=gas" --eval="MACROS= "

for F in $@; do

	echo "compiling file: $F into gas"
	./test-bin/test-compile-to < $F > tmp/compiled.s
	echo "compiled"

	echo "compiling gas"
	gcc --debug lib/herocio.c tmp/compiled.s -o tmp/to-run.bin
	echo "compiled"


	echo "================"
	$RUN_SPEC ./tmp/to-run.bin
	echo "================"
done
