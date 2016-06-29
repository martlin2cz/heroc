#!/bin/bash
# runs test-semanter on all examples


source test-scripts/base-of-run-scripts.sh

run_tests basic ./test-bin/test-semanter txt "Semantic error"

exit 0
