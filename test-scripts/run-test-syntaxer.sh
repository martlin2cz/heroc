#!/bin/bash
# runs test-syntaxer on all examples
# 29.6.2016

source test-scripts/base-of-run-scripts.sh

run_tests basic ./test-bin/test-syntaxer txt "Syntax error"

exit 0
