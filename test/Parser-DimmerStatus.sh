#!/bin/bash


INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xfb\x08\x08\xee\x03\x01\x20\x82\x34\x56\x80\x48\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : DimmerStatus from 0x08: mode=dimmer with memory dim=1% LED=fast blinking timer=8533078 config=0x80"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
