#!/bin/bash

echo "Checking Read only socket..."

PORT=61234
PORTRO=61235

mkfifo pipe.$$ # Will loop all input to output

../src/velbusd -s pipe.$$ -b [::1]:[${PORT}] -r [::1]:[${PORTRO}] >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

echo -n "" > input.bin.$$ # should be empty
( echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04"; sleep 1) | \
	socat - TCP6:localhost:$PORTRO > output.bin.$$

RV=0
if ! diff input.bin.$$ output.bin.$$; then
	echo "Read only socket echoed something back"
	RV=1
fi

kill -INT $PID_VELBUSD
rm -f pipe.$$ input.bin.$$ output.bin.$$

if ! wait $PID_VELBUSD; then
	echo "program exited uncleanly"
	RV=1
fi

exit $RV
