#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x02\xf5\x01\xfd\x04" | ./Parser )"
test "$OUTPUT" == "ClearLeds to 0x01: 00000001"
