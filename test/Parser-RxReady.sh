#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xf8\x00\x01\x0c\xec\x04" | ./Parser )"
test "$OUTPUT" == "RxReady from 0x00"
