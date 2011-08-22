#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x00\x01\x0a\xee\x04" | ./Parser )"
test "$OUTPUT" == "BusActive from 0x00"
