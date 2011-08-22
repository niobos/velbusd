#!/bin/bash

echo "Checking logging..."

PORT=61234

mkfifo pipe.$$ # Will loop all input to output

../src/velbusd -s pipe.$$ -b [::1]:[${PORT}] -l log.out.$$ -L 3 > /dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" | socat - TCP6:localhost:$PORT

kill -INT $PID_VELBUSD
rm -f pipe.$$

if ! wait $PID_VELBUSD; then
	echo "program exited uncleanly"
	RV=1
fi

exit $RV
