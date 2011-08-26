#!/bin/bash

echo "Checking that software flow control works..."

PORT=61234

./VMB1RS-sim > tty.$$ &
BUSSIM=$!

sleep 1

../src/velbusd -f -s $(<tty.$$) -b [::1]:[${PORT}] &
PID_VELBUSD=$!

sleep 1

(
	echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" # data
	echo -ne "\x0f\xf8\x00\x01\x0b\xed\x04" # rx buff full
	# idle time
	echo -ne "\x0f\xf8\x00\x01\x0c\xec\x04" # rx buff ok
	echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" # data
) > output_should.bin.$$
( 
	echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" # data
	echo -ne "\x0f\xfb\xff\x40\xb7\x04" # cause RxBuffFull for 1 second
	echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" # data
	sleep 2
) | socat - TCP6:localhost:$PORT > output.bin.$$

RV=0
if ! diff output_should.bin.$$ output.bin.$$; then
	echo "Echoed data does not match:"
	echo "output_should:"
	hexdump -C output_should.bin.$$
	echo "output:"
	hexdump -C output.bin.$$
	RV=1
fi

kill -INT $PID_VELBUSD
kill $BUSSIM
rm -f tty.$$ output_should.bin.$$ output.bin.$$

if ! wait $PID_VELBUSD; then
	echo "program exited uncleanly"
	RV=1
fi

exit $RV
