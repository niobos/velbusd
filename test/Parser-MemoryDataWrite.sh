#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x91\x04\xfc\x81\x83\x30\x31\x04" | ./Parser )"
test "$OUTPUT" == "MemoryDataWrite to 0x91: @0x8183 <= 0x30"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
