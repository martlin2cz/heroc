#!/bin/bash
# runs test-compile-to (with linked scheme) on all examples
# 29.6.2016

source test-scripts/base-of-run-scripts.sh

run_tests scheme ./test-bin/test-compile-to scm "Scheme error"

exit 0
