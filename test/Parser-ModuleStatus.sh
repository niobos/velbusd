#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x05\xed\x01\x00\x00\x00\x7e\x04" | ./Parser )"
test "$OUTPUT" == "ModuleStatus from 0x85: inputs=0b00000001 LEDs: cont=0b00000000 slow=0b00000000 fast=0b00000000"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xfb\x85\x07\xed\x00\x03\xff\x00\x00\x00\x7b\x04" | ./Parser )"
test "$OUTPUT" == "ModuleStatus from 0x85: inputs=0b00000000 enabled=0b00000011 noninverted=0b11111111 locked=0b00000000 progdisabled=0b00000000 program=None Alarms: 1(off local) 2(off local)  Sunrise disabled Sunset disabled"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
