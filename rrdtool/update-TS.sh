#!/bin/bash

ADDR="$1"
if [ -z "$ADDR" ]; then
	echo "Usage: $0 address"
	exit 1
fi
FILENAME="$ADDR-TS.rrd"

if [ ! -e "$FILENAME" ]; then
	echo "File \`$FILENAME\` does not exist"
	exit 1
fi
if [ ! -w "$FILENAME" ]; then
	echo "File \`$FILENAME\` not writable"
	exit 1
fi

DATA="$( wget --quiet -O- "http://localhost:8080/control/temp/$ADDR" )"
if [ $? -ne 0 ]; then
	echo "Error wget-ing data:"
	echo "$DATA"
	exit 1
fi

DATA="$( echo "$DATA" | ./json-to-lines.js )"

TEMP="$(   echo "$DATA" | grep "current temperature" | sed 's/^[^=]* = //' )"
TTEMP="$(  echo "$DATA" | grep "target temperature"  | sed 's/^[^=]* = //' )"

OUTPUT="$( echo "$DATA" | grep "output / " )"
HEATER="$( echo "$OUTPUT" | grep "heater"     | sed 's/^[^=]* = //' | sed 's/off/0/;s/on/1/' )"
BOOST="$(  echo "$OUTPUT" | grep "boost"      | sed 's/^[^=]* = //' | sed 's/off/0/;s/on/1/' )"
DAY="$(    echo "$OUTPUT" | grep "day"        | sed 's/^[^=]* = //' | sed 's/off/0/;s/on/1/' )"
COOLER="$( echo "$OUTPUT" | grep "cooler"     | sed 's/^[^=]* = //' | sed 's/off/0/;s/on/1/' )"
LOW="$(    echo "$OUTPUT" | grep "low alarm"  | sed 's/^[^=]* = //' | sed 's/off/0/;s/on/1/' )"
HIGH="$(   echo "$OUTPUT" | grep "high alarm" | sed 's/^[^=]* = //' | sed 's/off/0/;s/on/1/' )"

rrdtool update "$FILENAME" --template \
	"temperature:set_temperature:heater:boost:day:cooler:low_alarm:high_alarm" \
	"N:${TEMP:-U}:${TTEMP:-U}:${HEATER:-U}:${BOOST:-U}:${DAY:-U}:${COOLER:-U}:${LOW:-U}:${HIGH:-U}"
