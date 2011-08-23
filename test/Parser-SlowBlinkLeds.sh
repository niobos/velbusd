#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x02\xf7\x01\xfb\x04" | ./Parser )"
test "$OUTPUT" == "SlowBlinkLeds to 0x01: 00000001"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
