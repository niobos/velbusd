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

DATA="$( wget --quiet -O- "http://localhost:8080/control/relay/$ADDR/status" )"
if [ $? -ne 0 ]; then
	echo "Error wget-ing data:"
	echo "$DATA"
	exit 1
fi

if [ "$DATA" == "true" ]; then STATE="1"
elif [ "$DATA" == "false" ]; then STATE="0"
else STATE=""
fi

rrdtool update "$FILENAME" --template \
	"state" \
	"N:${STATE:-U}"
