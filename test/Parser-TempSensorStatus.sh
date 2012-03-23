#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x08\x08\xea\x01\x00\x40\x44\x0a\x00\x00\x6d\x04" | ./Parser )"
test "$OUTPUT" == "TempSensorStatus from 0x08: PB=locked Mode=run AutoSend=off Mode=safe Heater Unjam={no_pump no_valve} Program={no_all no_zone no_sensor} ProgramStep=safe Output={no_valve no_boost no_day no_cooler no_pump no_low_alarm high_alarm} Temp=34.0C Target=5.0C Timer=0"
if [ $? -ne 0 ]; then
	echo "Wrong output: $OUTPUT"
	exit 1
fi
