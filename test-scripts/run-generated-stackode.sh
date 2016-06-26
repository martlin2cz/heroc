#!/bin/bash
# runs generated (compiled) stackode code (in scheme sintax) with the in-scheme interpreter
# Martin Jasek, VI/2016

#interpretter
SCHEME=scheme-r5rs

make --quiet tests --eval="EXPORT=stackode" --eval="MACROS= "

F=$1
#examples-me/to-scheme-01.heroc


echo "compiling file: $F"
./bin/compiler < $F > tmp/compiled.scm
echo "compiled"

cat \
	test-scripts/stackode-evaluator.scm \
	test-scripts/define-program.scm \
	tmp/compiled.scm \
	test-scripts/close-define.scm \
	test-scripts/run-sc.scm \
	> tmp/to-eval.scm

echo "================"
$SCHEME tmp/to-eval.scm
echo "================"

