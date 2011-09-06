#!/bin/bash

PORT=61234
RV=0

mkfifo pipe.$$

../src/velbusd -s pipe.$$ -f -b [::1]:[$PORT] >/dev/null 2>&1 &
PID=$!
sleep 1
kill -INT $PID
wait $PID
if [ $? -ne 0 ]; then
	echo "Forground run failed"
	RV=1
fi

../src/velbusd -s pipe.$$ -f -p pid.$$ -b [::1]:[$PORT] >/dev/null 2>&1 &
PID=$!
sleep 1
if [ "$(<pid.$$)" -ne "$PID" ]; then
	echo "PID and PIDfile does not match"
	RV=1
fi
kill -INT $PID
wait $PID
if [ $? -ne 0 ]; then
	echo "Forground run failed"
	RV=1
fi

../src/velbusd -s pipe.$$ -p pid.$$ -b [::1]:[$PORT] >/dev/null 2>&1
if [ $? -ne 0 ]; then
	echo "Background spawn failed"
	RV=1
fi
sleep 1
kill -INT $(<pid.$$)

sleep 1
if [ -e pid.$$ ]; then
	echo "Failed to remove pid-file"
	RV=1
fi

rm -f pipe.$$ pid.$$
