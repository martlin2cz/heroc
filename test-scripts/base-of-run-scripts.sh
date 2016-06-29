#!/bin/bash
# declares function to run tests ...


#args: [OUTPUT_LANG] [BINARY_TO_RUN] [OUT_SUFFIX] [FILTER_LOG]
function run_tests {
	make --eval="OUTPUTLANG=$1" tests

	mkdir -p tmp/out/examples

	for F in examples/*.heroc;	do
		echo "-- " $F:
		$2 < $F 2> tmp/out/"$F".log > tmp/out/"$F".$3
		grep "$4" tmp/out/"$F".log
	done
}
