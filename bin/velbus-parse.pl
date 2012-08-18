#!/usr/bin/perl

use strict;
use warnings;
use Data::ParseBinary;
use Data::Dumper;


sub binary ($) {
	return join '', unpack "B8", pack "C", $_[0];
}

sub enum {
	my ($key, %hash) = @_;
	return $hash{$key} if defined $hash{$key};
	return sprintf "Unknown[0x%02x]", $key;
}

my @parser;

push @parser, sub { # Module Type Request {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 1;
	return undef unless @data == 0;
	return sprintf("ModuleTypeRequest to 0x%02x", $addr);
}; # }}}

push @parser, sub { # Push Button Status {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0x00;

	my $just_pressed = $data[1];
	my $just_released = $data[2];
	my $long_pressed = $data[3];

	return sprintf("PushButtonStatus from 0x%02x: on=%s long=%s off=%s",
			$addr, binary($just_pressed), binary($just_released),
			binary($long_pressed));
}; # }}}

push @parser, sub { # Push Button Status {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0x00;

	my $just_pressed = $data[1];
	my $just_released = $data[2];
	my $long_pressed = $data[3];

	return sprintf("PushButtonStatus from 0x%02x: on=%s long=%s off=%s",
			$addr, binary($just_pressed), binary($just_released),
			binary($long_pressed));
}; # }}}

push @parser, sub { # Blind Status {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 8;
	return undef unless $data[0] == 0xec;

	my $channel = enum(  $data[1], 0x03=> "1", 0x0c => "2" );
	my $timeout = enum(  $data[2], 0=>"15s", 1=>"30s", 2=>"1m", 3=>"2m");
	my $status1 = enum(  $data[3]&0x03,        0=>"off", 1=>"up", 2=>"down");
	my $status2 = enum( ($data[3]&0x0c) >> 2,  0=>"off", 1=>"up", 2=>"down");
	my $led = enum( $data[4],
				0 => "off",
				0x80 => "down on", 0x40 => "down slow blink",
				0x20 => "down fast blink", 0x10 => "down very fast blink",
				0x08 => "up on", 0x04 => "up slow blink",
				0x02 => "up fast blink", 0x01 => "up very fast blink"
			);
	my $timer = ($data[5] << 16) + ($data[6] << 8) + $data[7];

	return sprintf("BlindStatus from 0x%02x: Blind=%s Timeout=%s " .
			"Status1=%s Status2=%s Led=%s Timer=%d",
			$addr, $channel, $timeout, $status1, $status2, $led, $timer);
}; # }}}





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
	$data = join '', map { chr($_); } @data;

	for my $p (@parser) {
		my $temp = &{$p}($prio, $addr, $rtr, $data, @data);
		if( defined $temp ) {
			$_ = "$timestamp $source : $temp\n";
			last; # `for` iteration
		}
	}

} continue {
	print $_;
}

# vim: set foldmethod=marker:
