#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x02\xf9\x01\xf9\x04" | ./Parser )"
test "$OUTPUT" == "VFastBlinkLeds to 0x01: 00000001"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
