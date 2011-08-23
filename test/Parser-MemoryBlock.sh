#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x07\xcc\x81\x83\x12\x34\x56\x80\x02\x04" | ./Parser )"
test "$OUTPUT" == "MemoryBlock from 0x01: @0x8183 = 12 34 56 80"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
