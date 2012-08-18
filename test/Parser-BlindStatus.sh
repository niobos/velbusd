#!/bin/bash

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xfb\x85\x08\xec\x03\x01\x02\x10\x01\x80\x80\x66\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : BlindStatus from 0x85: Blind=1 Timeout=30s Status1=down Status2=off Led=down very fast blink Timer=98432"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
