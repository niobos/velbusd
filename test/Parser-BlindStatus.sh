#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x08\xec\x03\x01\x02\x10\x01\x80\x80\x66\x04" | ./Parser )"
test "$OUTPUT" == "BlindStatus from 0x85: Blind=1 Timeout=30s Status1=down Status2=off Led=down very fast blink Timer=98432"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
