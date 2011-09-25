#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x91\x07\xca\x81\x83\x12\x34\x56\x80\x74\x04" | ./Parser )"
test "$OUTPUT" == "MemoryBlockWrite to 0x91: @0x8183 <= 12 34 56 80"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
