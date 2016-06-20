#!/bin/bash
# runs generated (compiled) scheme code with the scheme interpreter
# Martin Jasek, V/2016

#interpretter
SCHEME=scheme-r5rs

make --quiet tests --eval="EXPORT=scheme" --eval="MACROS= "

F=$1
#examples-me/to-scheme-01.heroc


echo "compiling file: $F"
./bin/compiler < $F > tmp/compiled.scm
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

