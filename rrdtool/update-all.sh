#!/bin/bash

: ${TOOLPATH:=$(cd $(dirname "$0"); echo $PWD)}

cd ${RRDDIR:-/var/lib/rrd/} || exit 1

for f in *-TS.rrd; do
	ADDR="${f%%-TS.rrd}"
	echo "Updating TS $ADDR"
	"$TOOLPATH/update-TS.sh" "$ADDR"
done

for f in *-RELAY.rrd; do
	ADDR="${f%%-RELAY.rrd}"
	echo "Updating RELAY $ADDR"
	"$TOOLPATH/update-RELAY.sh" "$ADDR"
done
