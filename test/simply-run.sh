#!/bin/bash

SECONDS=2
echo "Simply running the daemon for $SECONDS seconds..."

PORT=61234

mkfifo pipe.$$

../src/velbusd -f -s pipe.$$ -b [::1]:[${PORT}] >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep $SECONDS

kill -INT $PID_VELBUSD
rm -f pipe.$$

wait $PID_VELBUSD
