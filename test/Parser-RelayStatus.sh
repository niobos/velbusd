#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x02\x08\xfb\x01\x00\x00\x00\x00\x00\x00\xf0\x04" | ./Parser )"
test "$OUTPUT" == "RelayStatus of 0x02 / 0 : relay=off status=normal LED=off timer=0"
