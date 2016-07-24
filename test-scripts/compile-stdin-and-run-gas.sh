#!/bin/bash
# reads stdin, compiles to gas (hopefully) file, created gas file to binary, binary executes and outputs to stdout

./test-bin/test-compile-to >  tmp/compiled.s
gcc lib/herocio.c tmp/compiled.s -o tmp/binary.bin
./tmp/binary.bin

exit 0
