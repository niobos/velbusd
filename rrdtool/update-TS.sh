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

DATA="$( wget --quiet --header="Accept: text/plain" -O- "http://localhost/domotica/api/TempStatus.php/$ADDR" )"
if [ $? -ne 0 ]; then
	echo "Error wget-ing data:"
	echo "$DATA"
	exit 1
fi

TEMP="$(   echo "$DATA" | grep "^temp:"        | sed 's/^[^:]*://' )" 
TTEMP="$(  echo "$DATA" | grep "^target_temp:" | sed 's/^[^:]*://' )"
HEATER="$( echo "$DATA" | grep "^heater:"      | sed 's/^[^:]*://' )"
BOOST="$(  echo "$DATA" | grep "^boost:"       | sed 's/^[^:]*://' )"
DAY="$(    echo "$DATA" | grep "^day:"         | sed 's/^[^:]*://' )"
COOLER="$( echo "$DATA" | grep "^cooler:"      | sed 's/^[^:]*://' )"
LOW="$(    echo "$DATA" | grep "^low_alarm:"   | sed 's/^[^:]*://' )"
HIGH="$(   echo "$DATA" | grep "^high_alarm:"  | sed 's/^[^:]*://' )"

rrdtool update "$FILENAME" --template \
	"temperature:set_temperature:heater:boost:day:cooler:low_alarm:high_alarm" \
	"N:${TEMP:-U}:${TTEMP:-U}:${HEATER:-U}:${BOOST:-U}:${DAY:-U}:${COOLER:-U}:${LOW:-U}:${HIGH:-U}"
