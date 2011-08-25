#!/bin/bash

PORT=61234
TIME=15
BANDWIDTH=100000

echo "Testing behaviour when a slow client is connected for $TIME seconds..."

./VMB1RS-sim > tty.$$ &
BUSSIM=$!
sleep 1

../src/velbusd -f -s $(<tty.$$) -b [::1]:[${PORT}] >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

socat TCP6:localhost:$PORT,rcvbuf=1024 EXEC:"sleep 15" &
PID_SLOW=$!

# Spit 1.4MB/second
END=$(( $(date +%s) + 15 ))
( while [ $(date +%s) -lt $END ]; do sleep 1; i=0; while [ $i -lt $BANDWIDTH ]; do echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04"; i=$(($i+1)); done; done ) |
	./NBcat | socat - TCP6:localhost:$PORT > /dev/null
RV=$?

kill $PID_VELBUSD
kill $PID_SLOW
kill $BUSSIM
rm -f tty.$$

exit $RV
