#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x00\x01\x0e\xea\x04" | ./Parser )"
test "$OUTPUT" == "IntStatusRequest to 0x00"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
