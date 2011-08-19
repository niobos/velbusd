#!/bin/bash

PORT=61234

mkfifo pipe.$$

../src/velbusd -s pipe.$$ -b [::1]:[${PORT}] >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

socat TCP6:localhost:$PORT,rcvbuf=1024 EXEC:"sleep 15" &
PID_SLOW=$!

# Spit 1.4MB/second
( while sleep 1; do i=0; while [ $i -lt 100000 ]; do echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04"; i=$(($i+1)); done; done ) |
	./NBcat | socat - TCP6:localhost:$PORT > /dev/null
RV=$?

kill $PID_VELBUSD
kill $PID_SLOW
rm -f pipe.$$

exit $RV
