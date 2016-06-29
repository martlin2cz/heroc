#!/bin/bash
# runs test-lexer on all examples
# 29.6.2016

source test-scripts/base-of-run-scripts.sh

run_tests basic ./test-bin/test-lexer txt "Lexical error"

exit 0
