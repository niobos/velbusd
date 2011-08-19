#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x02\xf6\x01\xfc\x04" | ./Parser )"
test "$OUTPUT" == "SetLeds to 0x01: 00000001"
