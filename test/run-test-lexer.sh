#!/bin/bash
# runs test-lexer on all examples

mkdir -p tmp
rm -rf tmp/log.log

for F in examples-vychodil/*.heroc counterexamples/*.heroc;	do
	echo $F:
	echo $F: >> tmp/log.log
	./bin/test-lexer < $F >> tmp/log.log
done

