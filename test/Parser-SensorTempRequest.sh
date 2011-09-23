#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x02\xe5\x00\x8a\x04" | ./Parser )"
test "$OUTPUT" == "SensorTempRequest to 0x85: AutoSend disabled"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
