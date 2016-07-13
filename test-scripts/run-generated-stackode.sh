#!/bin/bash
# runs generated (compiled) stackode code (in scheme sintax) with the in-scheme interpreter
# Martin Jasek, VI/2016

#interpreter of scheme
SCHEME=scheme-r5rs

#timeout (in secs) to test run
TIMEOUT=1000

make tests --eval="OUTPUTLANG=stackode" --eval="MACROS= "

for F in $@; do

	echo "compiling file: $F"
	./test-bin/test-compile-to < $F > tmp/compiled.scm
	echo "compiled"

	cat \
		test-scripts/stackode-evaluator.scm \
		test-scripts/define-program.scm \
		tmp/compiled.scm \
		test-scripts/close-define.scm \
		test-scripts/run-sc.scm \
		> tmp/to-eval.scm

	echo "================"
	timeout $TIMEOUT $SCHEME tmp/to-eval.scm
	echo "================"
done
