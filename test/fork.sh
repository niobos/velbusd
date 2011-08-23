#!/bin/bash

RV=0

mkfifo pipe.$$

../src/velbusd -s pipe.$$ -f >/dev/null 2>&1 &
PID=$!
sleep 1
kill -INT $PID
wait $PID
if [ $? -ne 0 ]; then
	echo "Forground run failed"
	RV=1
fi

../src/velbusd -s pipe.$$ -f -p pid.$$ >/dev/null 2>&1 &
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

../src/velbusd -s pipe.$$ -p pid.$$ >/dev/null 2>&1
if [ $? -ne 0 ]; then
	echo "Background spawn failed"
	RV=1
fi
sleep 1
kill -INT $(<pid.$$)

rm -f pipe.$$ pid.$$
