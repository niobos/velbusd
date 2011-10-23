#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x85\x02\x01\x02\x6f\x04" | ./Parser )"
test "$OUTPUT" == "SwitchRelayOff to 0x85: Relay=2"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xf8\x85\x02\x02\x10\x60\x04" | ./Parser )"
test "$OUTPUT" == "SwitchRelayOn to 0x85: Relay=5"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
