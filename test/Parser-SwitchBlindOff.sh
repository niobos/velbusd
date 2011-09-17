#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x05\x02\x04\x03\xeb\x04" | ./Parser )"
test "$OUTPUT" == "SwitchBlindOff to 0x05: Blind=1"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
