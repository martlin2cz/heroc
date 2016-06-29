#!/bin/bash
# runs test-compile-to (with linked basic) on all examples
# 29.6.2016

source test-scripts/base-of-run-scripts.sh

run_tests basic ./test-bin/test-compile-to txt "Export error"

exit 0
