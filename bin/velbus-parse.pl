#!/usr/bin/perl

use strict;
use warnings;
use Data::ParseBinary;


sub binary ($) {
	return join '', unpack "B8", pack "C", $_[0];
}

my @parser;

push @parser, sub { my ($prio, $addr, $rtr, @data) = @_;
	# Module Type Request
	return undef unless $rtr  == 1;
	return undef unless @data == 0;
	return sprintf("ModuleTypeRequest to 0x%02x", $addr);
};

push @parser, sub { my ($prio, $addr, $rtr, @data) = @_;
	# Push Button Status
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0x00;

	my $just_pressed = $data[1];
	my $just_released = $data[2];
	my $long_pressed = $data[3];

	return sprintf("PushButtonStatus from 0x%02x: on=%s long=%s off=%s",
			$addr, binary($just_pressed), binary($just_released),
			binary($long_pressed));
};


$| = 1; # disable buffering

while(<>) {
	# 2011-08-30T09:29:46+0200 SERIAL : Unknown message: Prio=3 Addr=0x2 RTR=0 data[8]=fb 01 00 00 00 00 00 00
	#               2011-08-30T09:29:46+0200                     SERIAL : Unknown message: Prio=3   Addr=0x2              RTR=0    data [ 8 ]=fb 01 00 00 00 00 00 00
	#              1                                             2                             3           4                  5               6
	next unless m/^(\d\d\d\d-\d\d-\d\dT\d\d:\d\d:\d\d\+\d\d\d\d) (.*?) : Unknown message: Prio=(\d) Addr=0x([0-9a-fA-F]+) RTR=(\d) data\[\d\]=(.*)$/;
	my ($timestamp, $source, $prio, $addr, $rtr, $data) = ($1, $2, $3, $4, $5, $6);
	$prio = int $prio;
	$addr = hex $addr;
	$rtr = ( $rtr eq "1" ? 1 : 0 );
	my @data = map { hex $_ } split / /, $data;

	for my $p (@parser) {
		my $temp = &{$p}($prio, $addr, $rtr, @data);
		if( defined $temp ) {
			$_ = "$timestamp $source : $temp\n";
			last; # `for` iteration
		}
	}

} continue {
	print $_;
}
