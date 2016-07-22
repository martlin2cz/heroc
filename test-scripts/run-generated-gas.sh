#!/bin/bash
# compiles and runs generated (compiled) gas code
# Martin Jasek, VII/2016

#timeout (in secs) to test run
TIMEOUT=20

#run in gdb? comment out or set to empty if not
#GDB=gdbtui

make tests --eval="OUTPUTLANG=gas" --eval="MACROS= "

for F in $@; do

	echo "compiling file: $F into gas"
	./test-bin/test-compile-to < $F > tmp/compiled.s
	echo "compiled"

	echo "compiling gas"
	gcc --debug lib/herocio.c tmp/compiled.s -o tmp/to-run.bin
	echo "compiled"


	echo "================"
	# choose one:
	#timeout $TIMEOUT ./tmp/to-run.bin
	$GDB ./tmp/to-run.bin
	echo "================"
done
