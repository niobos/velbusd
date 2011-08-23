#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x04\xfe\x81\x83\x30\xbf\x04" | ./Parser )"
test "$OUTPUT" == "MemoryData from 0x01: @0x8183 = 0x30"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
