#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x02\xf6\x01\xfc\x04" | ./Parser )"
test "$OUTPUT" == "SetLeds to 0x01: 00000001"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
