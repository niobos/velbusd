#!/bin/bash

ADDR="$1"
if [ -z "$ADDR" ]; then
	echo "Usage: $0 address"
	exit 1
fi
FILENAME="$ADDR-RELAY.rrd"

if [ ! -e "$FILENAME" ]; then
	echo "File \`$FILENAME\` does not exist"
	exit 1
fi
if [ ! -w "$FILENAME" ]; then
	echo "File \`$FILENAME\` not writable"
	exit 1
fi

DATA="$( wget --quiet --header="Accept: text/plain" -O- "http://localhost/domotica/api/RelayStatus.php/$ADDR" )"
if [ $? -ne 0 ]; then
	echo "Error wget-ing data:"
	echo "$DATA"
	exit 1
fi

STATE="$( echo "$DATA" | grep "^state:" | sed 's/^[^:]*://' )" 
if [ "$STATE" == "on" ]; then STATE="1"
elif [ "$STATE" == "off" ]; then STATE="0"
else STATE=""
fi

rrdtool update "$FILENAME" --template \
	"state" \
	"N:${STATE:-U}"
