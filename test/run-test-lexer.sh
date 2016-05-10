#!/bin/bash
# runs test-lexer on all examples

mkdir -p tmp
rm -rf tmp/log.log

for F in examples-vychodil/*.heroc counterexamples/*.heroc;	do
	echo $F:
	./bin/test-lexer < $F >> tmp/log.log
done

