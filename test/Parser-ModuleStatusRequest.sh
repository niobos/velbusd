#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x02\xfa\x03\x72\x04" | ./Parser )"
test "$OUTPUT" == "ModuleStatusRequest to 0x85: Blind=1"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xfb\x85\x02\xfa\x02\x73\x04" | ./Parser )"
test "$OUTPUT" == "ModuleStatusRequest to 0x85: Relay=2"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
