#!/bin/bash

cd /mnt/data/rrdtool

for f in *-TS.rrd; do
	ADDR="${f%%-TS.rrd}"
	echo "Updating TS $ADDR"
	./update-TS.sh "$ADDR"
done

for f in *-RELAY.rrd; do
	ADDR="${f%%-RELAY.rrd}"
	echo "Updating RELAY $ADDR"
	./update-RELAY.sh "$ADDR"
done
