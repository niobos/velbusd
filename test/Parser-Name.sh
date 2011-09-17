#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x05\x08\xf0\x03Test\xff\xff\x58\x04" | ./Parser )"
test "$OUTPUT" == "Name from 0x05: Item 00000011 pt0/3 \"Test\""
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
