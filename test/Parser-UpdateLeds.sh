#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x85\x04\xf4\xf1\xaa\xcc\x12\x04" | ./Parser )"
test "$OUTPUT" == "UpdateLeds to 0x85: on=11110001 slowblink=00000010 fastblink=00000100 veryfastblink=00001000"
