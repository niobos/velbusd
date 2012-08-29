#!/bin/bash


INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xfb\x08\x03\xe4\x00\x22\xe5\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : SetTemperature to 0x08: Current temperature set <= 17ºC"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
