#!/bin/bash

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xfb\x91\x01\xd9\x8b\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : BusErrorCounterRequest to 0x91"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
