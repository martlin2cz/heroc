#!/bin/bash
# runs generated (compiled) scheme code with the scheme interpreter
# Martin Jasek, V/2016

#interpreter
SCHEME=scheme-r5rs

make --eval="OUTPUTLANG=scheme" --eval="MACROS= "

for F in $@; do

	echo "compiling file: $F"
	./test-bin/test-compile-to < $F > tmp/compiled.scm
	echo "compiled"

	cat \
		test-scripts/eval-generated.scm \
		lib/herocio.scm \
		tmp/compiled.scm \
		test-scripts/run-main.scm \
		> tmp/to-eval.scm

	echo "================"
	$SCHEME tmp/to-eval.scm
	echo "================"

done
