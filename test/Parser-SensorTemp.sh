#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x07\xe6\x2e\xe0\x2b\x80\x2f\x20\x7c\x04" | ./Parser )"
test "$OUTPUT" == "SensorTemp from 0x85: Current=23.44C Min=21.75C Max=23.56C"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
