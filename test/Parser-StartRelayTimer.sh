#!/bin/bash

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x85\x05\x03\x02\x80\x81\x82\xe7\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : StartRelayTimer to 0x85: Relay=2 Timeout=8421762s"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x85\x05\x0d\x02\x83\x82\x81\xda\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : StartRelayIntervalTimer to 0x85: Relay=2 Timeout=8618625s"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi

INPUT="2012-01-01T00:00:00+0000 SERIAL : $( echo -ne "\x0f\xf8\x85\x05\x03\x02\xff\xff\xff\x6d\x04" | ./Parser )"
OUTPUT="$( echo "$INPUT" | ../bin/velbus-parse.pl )"
test "$OUTPUT" == "2012-01-01T00:00:00+0000 SERIAL : StartRelayTimer to 0x85: Relay=2 Timeout=permanent"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
