#!/bin/bash

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xfb\x85\x02\xe5\x00\x8a\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : SensorTempRequest to 0x85: AutoSend disabled"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
