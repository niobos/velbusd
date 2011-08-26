#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x05\x03\xff\xfe\xfc\xf5\x04" | ./Parser )"
test "$OUTPUT" == "ModuleType from 0x05: Unknown module [0xfe] Data[1]: fc"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xfb\x05\x05\xff\x09\x00\x0b\x05\xd4\x04" | ./Parser )"
test "$OUTPUT" == "ModuleType from 0x05: VMB2BL  Timeout blind 1=15sTimeout blind 2=15s Build 11-W5"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
