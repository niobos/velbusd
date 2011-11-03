#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x00\x04\xd8\x03\xb\x02\x0a\x04" | ./Parser )"
test "$OUTPUT" == "RTCstatus to 0x00: Thursday 11:02"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
