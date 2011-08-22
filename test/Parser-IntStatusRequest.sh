#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x00\x01\x0e\xea\x04" | ./Parser )"
test "$OUTPUT" == "IntStatusRequest to 0x00"
