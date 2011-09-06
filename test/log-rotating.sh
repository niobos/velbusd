#!/bin/bash

PORT=61234

./VMB1RS-sim > tty.$$ &
BUSSIM=$!

sleep 1

../src/velbusd -f -s $(<tty.$$) -b [::1]:[${PORT}] -l log.$$ >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

mv log.$$ log.$$.2
kill -HUP $PID_VELBUSD

sleep 1

RV=0

kill -INT $PID_VELBUSD
kill $BUSSIM
rm -f tty.$$

if ! wait $PID_VELBUSD; then
	echo "program exited uncleanly"
	RV=1
fi

if [ -z "$(< log.$$.2 )" ]; then
	echo "old log is empty"
	RV=1
fi
if [ -z "$(< log.$$ )" ]; then
	echo "new log is empty"
	RV=1
fi
rm -f log.$$ log.$$.2

exit $RV
