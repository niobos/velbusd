#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x02\x40\xb4\x04" | ./Parser )"
test "$OUTPUT" == "ModuleTypeRequest to 0x02"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
