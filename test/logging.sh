#!/bin/bash

PORT=61234

./VMB1RS-sim > tty.$$ &
BUSSIM=$!

sleep 1

../src/velbusd -f -s $(<tty.$$) -b [::1]:[${PORT}] -l log.$$ >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" | socat - TCP6:localhost:$PORT > /dev/null

RV=0

kill -INT $PID_VELBUSD
kill $BUSSIM
rm -f tty.$$

if ! wait $PID_VELBUSD; then
	echo "program exited uncleanly"
	RV=1
fi

if [ -z "$(< log.$$ )" ]; then
	echo "log is empty"
	RV=1
fi
rm -f log.$$

exit $RV
