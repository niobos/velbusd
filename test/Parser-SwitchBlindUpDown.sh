#!/bin/bash

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x05\x05\x05\x03\x00\x00\x00\xe7\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : SwitchBlindUp to 0x05: Blind=1 Timeout=default"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x05\x05\x06\x0c\x81\x82\x83\x57\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : SwitchBlindDown to 0x05: Blind=2 Timeout=8487555s"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x05\x05\x06\x0c\xff\xff\xff\xe0\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : SwitchBlindDown to 0x05: Blind=2 Timeout=permanent"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
