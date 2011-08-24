#!/bin/bash

OUTPUT="$( echo -ne "garbage" | ./Parser )"
test "$OUTPUT" == "FormError: data[0] != 0x0f"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi