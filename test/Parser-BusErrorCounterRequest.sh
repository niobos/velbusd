#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x91\x01\xd9\x8b\x04" | ./Parser )"
test "$OUTPUT" == "BusErrorCounterRequest to 0x91"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
