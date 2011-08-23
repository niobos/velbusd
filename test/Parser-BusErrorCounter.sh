#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x04\xda\x01\x02\x80\x94\x04" | ./Parser )"
test "$OUTPUT" == "BusErrorCounter from 0x01: Tx=1 Rx=2 Bus off=128"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
