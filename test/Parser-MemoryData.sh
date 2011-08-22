#!/bin/bash

OUTPUT="$( echo -ne "\x0f\xfb\x01\x04\xfe\x01\x23\x30\x9f\x04" | ./Parser )"
test "$OUTPUT" == "MemoryData from 0x01: @0x123 = 0x30"
