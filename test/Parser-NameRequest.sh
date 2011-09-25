#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x02\xef\x03\x7d\x04" | ./Parser )"
test "$OUTPUT" == "NameRequest to 0x85: Blind 1"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
