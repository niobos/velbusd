#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x85\x05\x03\x02\x80\x81\x82\xe7\x04" | ./Parser )"
test "$OUTPUT" == "StartRelayTimer to 0x85: Relay=2 Timeout=8421762s"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

OUTPUT="$( echo -ne "\x0f\xf8\x85\x05\x0d\x02\x83\x82\x81\xda\x04" | ./Parser )"
test "$OUTPUT" == "StartRelayIntervalTimer to 0x85: Relay=2 Timeout=8618625s"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
