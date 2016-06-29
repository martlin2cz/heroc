#!/bin/bash
# runs test-compile-to (with linked gas) on all examples
# 29.6.2016

source test-scripts/base-of-run-scripts.sh

run_tests gas ./test-bin/test-compile-to gas "GAS error"

exit 0
