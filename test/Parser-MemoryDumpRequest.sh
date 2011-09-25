#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x91\x01\xcb\x99\x04" | ./Parser )"
test "$OUTPUT" == "MemoryDumpRequest to 0x91"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
