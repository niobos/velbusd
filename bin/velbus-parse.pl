#!/usr/bin/perl

use strict;
use warnings;

while(<>) {
	# 2011-08-30T09:29:46+0200 SERIAL : Unknown message: Prio=3 Addr=0x2 RTR=0 data[8]=fb 01 00 00 00 00 00 00
	#               2011-08-30T09:29:46+0200                    SERIAL : Unknown message: Prio=3   Addr=0x2              RTR=0    data[8]=  fb 01 00 00 00 00 00 00
	#              1                                            2                             3           4                  5             6
	#next unless m/^(\d\d\d\d-\d\d-\d\dT\d\d:\d\d:\d\d\+\d\d\d\d) (.*?) : Unknown message: Prio=(\d) Addr=0x([0-9a-fA-F]+) RTR=(\d) data[\d]=(.*)$/;
	next unless m/^(\d\d\d\d-\d\d-\d\dT\d\d:\d\d:\d\d\+\d\d\d\d) (.*?) : Unknown message: Prio=(\d) Addr=0x([0-9a-fA-F]+) RTR=(\d) data\[\d\]=(.*)/;
	my ($timestamp, $source, $prio, $addr_hex, $rtr, $data) = ($1, $2, $3, $4, $5, $6);

	my @data = map { hex $_ } split / /, $data;

	$_ = "";

	# Parse known messages
	if( $rtr eq "1" && $data eq "" ) {
		$_ = "Module Type Request for 0x$addr_hex";

	}

	if( $_ eq "" ) { # Default
		$_ = "Unknown message: Prio=$prio Addr=0x$addr_hex RTR=$rtr data[]=$data";
	}
	$_ = "$timestamp $source : $_\n";

} continue {
	print $_;
}
