#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x91\x03\xfd\x81\x83\x61\x04" | ./Parser )"
test "$OUTPUT" == "MemoryDataRequest to 0x91: Address 0x8183"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
