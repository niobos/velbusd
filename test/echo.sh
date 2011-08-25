#!/bin/bash

echo "Checking that messages pass through..."

PORT=61234

./VMB1RS-sim > tty.$$ &
BUSSIM=$!

sleep 1

../src/velbusd -f -s $(<tty.$$) -b [::1]:[${PORT}] >/dev/null 2>&1 &
PID_VELBUSD=$!

sleep 1

echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" > input.bin.$$
( cat input.bin.$$; sleep 1) | socat - TCP6:localhost:$PORT > output.bin.$$

RV=0
if ! diff input.bin.$$ output.bin.$$; then
	echo "Echoed data does not match:"
	echo "input:"
	hexdump -C input.bin.$$
	echo "output:"
	hexdump -C output.bin.$$
	RV=1
fi

kill -INT $PID_VELBUSD
kill $BUSSIM
rm -f tty.$$ input.bin.$$ output.bin.$$

if ! wait $PID_VELBUSD; then
	echo "program exited uncleanly"
	RV=1
fi

exit $RV
