#!/bin/bash
# runs test-semanter on all examples


source test-scripts/base-of-run-scripts.sh

run_tests gas ./test-scripts/compile-stdin-and-run-gas.sh txt "./tmp/binary.bin"

exit 0
