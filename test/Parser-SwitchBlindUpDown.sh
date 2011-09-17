#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x05\x05\x05\x03\x00\x00\x00\xe7\x04" | ./Parser )"
test "$OUTPUT" == "SwitchBlindUp to 0x05: Blind=1 Timeout=default"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xf8\x05\x05\x06\x0c\x81\x82\x83\x57\x04" | ./Parser )"
test "$OUTPUT" == "SwitchBlindDown to 0x05: Blind=2 Timeout=8487555s"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xf8\x05\x05\x06\x0c\xff\xff\xff\xe0\x04" | ./Parser )"
test "$OUTPUT" == "SwitchBlindDown to 0x05: Blind=2 Timeout=permanent"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
