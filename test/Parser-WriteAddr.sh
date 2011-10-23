#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x07\x6a\x11\x81\x82\x86\x83\x84\x5f\x04" | ./Parser )"
test "$OUTPUT" == "WriteAddr to 0x85: Current S/N: 0x8182  New addr: 0x86  New S/N: 0x8384"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
