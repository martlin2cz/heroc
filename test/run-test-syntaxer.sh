#!/bin/bash
# runs test-lexer on all examples

mkdir -p tmp
rm -rf tmp/log.log

for F in examples-vychodil/*.heroc counterexamples/*.heroc;	do
	echo $F:
	echo $F: >> tmp/log.log
	cat $F | ./bin/test-syntaxer # >> tmp/log.log
done

