#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x91\x03\xc9\x81\x83\x95\x04" | ./Parser )"
test "$OUTPUT" == "MemoryBlockRequest to 0x91: start address 0x8183"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
