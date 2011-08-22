#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x00\x01\x09\xef\x04" | ./Parser )"
test "$OUTPUT" == "BusOff from 0x00"
