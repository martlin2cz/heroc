#!/bin/bash
# runs test-compile-to (with linked stackode) on all examples
# 29.6.2016

source test-scripts/base-of-run-scripts.sh

run_tests stackode ./test-bin/test-compile-to scm "Stackode error"

exit 0
