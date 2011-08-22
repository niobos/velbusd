#!/bin/bash

OUTPUT="$( echo -ne "garbage" | ./Parser )"
test "$OUTPUT" == "FormError"
