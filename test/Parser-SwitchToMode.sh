#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x04\x03\xdc\xff\x00\x14\x04" | ./Parser )"
test "$OUTPUT" == "SwitchToMode to 0x04: switch to day; Timer: Program step"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xfb\x0a\x03\xdb\xff\xff\x10\x04" | ./Parser )"
test "$OUTPUT" == "SwitchToMode to 0x0a: switch to comfort; Timer: Manual"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xfb\xaa\x03\xdd\x00\x00\x6c\x04" | ./Parser )"
test "$OUTPUT" == "SwitchToMode to 0xaa: switch to night; Timer: Cancel"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xfb\xfa\x03\xde\xf0\xf0\x3b\x04" | ./Parser )"
test "$OUTPUT" == "SwitchToMode to 0xfa: switch to safe; Timer: 61680sec"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
