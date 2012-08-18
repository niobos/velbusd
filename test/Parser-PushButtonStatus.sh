#!/bin/bash

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x01\x04\x00\x01\x00\x00\xf3\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : PushButtonStatus from 0x01: on=00000001 long=00000000 off=00000000"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
