#!/usr/bin/env perl

use strict;
use warnings;
use Data::ParseBinary;
use Data::Dumper;


sub binary ($) { # {{{
	return join '', unpack "B8", pack "C", $_[0];
} # }}}

sub enum { # {{{
	my ($key, %hash) = @_;
	return $hash{$key} if defined $hash{$key};
	return sprintf "Unknown[0x%02x]", $key;
} # }}}

sub twos_complement { # {{{
	my (@byte) = @_;
	my $ret = 0;
	if( $byte[0] & 0x80 ) {
		# negative
		while( @byte ) {
			$ret <<= 8;
			$ret |= (shift @byte) ^ 0xff;
		}
		$ret = -($ret + 1);
	} else {
		# positive
		while( @byte ) {
			$ret <<= 8;
			$ret |= shift @byte;
		}
	}
	return $ret;
} # }}}

my @parser;

push @parser, sub { # Module Type Request {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 1;
	return undef unless @data == 0;
	return sprintf("ModuleTypeRequest to 0x%02x", $addr);
}; # }}}

push @parser, sub { # Blind Status (0xec) {{{
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

push @parser, sub { # Bus Error Counter (0xda) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0xda;

	my $tx_errors = $data[1];
	my $rx_errors = $data[2];
	my $bus_off = $data[3];

	return sprintf("BusErrorCounter from 0x%02x: Tx=%d Rx=%d Bus off=%d",
			$addr, $tx_errors, $rx_errors, $bus_off);
}; # }}}

push @parser, sub { # Bus Error Counter Request (0xd9) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 1;
	return undef unless $data[0] == 0xd9;

	return sprintf("BusErrorCounterRequest to 0x%02x", $addr);
}; # }}}

push @parser, sub { # {Clear,Set,SlowBlink,FastBlink,VFastBlink} Leds (0xf5, 0xf6, 0xf7, 0xf8, 0xf9) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 2;

	my $cmd;
	if( $data[0] == 0xf5 ) {
		$cmd = "ClearLeds";
	} elsif( $data[0] == 0xf6 ) {
		$cmd = "SetLeds";
	} elsif( $data[0] == 0xf7 ) {
		$cmd = "SlowBlinkLeds";
	} elsif( $data[0] == 0xf8 ) {
		$cmd = "FastBlinkLeds";
	} elsif( $data[0] == 0xf9 ) {
		$cmd = "VFastBlinkLeds";
	} else {
		return undef;
	}

	my $leds = $data[1];

	return sprintf("$cmd to 0x%02x: %s", $addr, binary($leds) );
}; # }}}

push @parser, sub { # Dimmer Status (0xee) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 8;
	return undef unless $data[0] == 0xee;

	my $mode = enum( $data[1],
			0 => "start/stop timer",
			1 => "staircase timer",
			2 => "dimmer",
			3 => "dimmer with memory",
			4 => "multi step dimmer",
			5 => "slow on dimmer",
			6 => "slow off dimmer",
			7 => "slow on/off dimmer",
		);
	my $dimvalue = $data[2];
	my $led = enum( $data[3],
			0x00 => "off",
			0x80 => "on",
			0x40 => "slow blinking",
			0x20 => "fast blinking",
			0x10 => "very fast blinking",
		);
	my $delay = ($data[4] << 16) | ($data[5] << 8) | ($data[6]);
	my $config = $data[7];

	return sprintf("DimmerStatus from 0x%02x: mode=%s dim=%d%% LED=%s timer=%d config=0x%02x",
			$addr, $mode, $dimvalue, $led, $delay, $config);
}; # }}}

push @parser, sub { # Dimmer Slider Status (0x0f) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0x0f;

	return undef unless $data[1] == 0x01;
	my $dim = $data[2];
	return undef unless $data[3] == 0x00;

	return sprintf("DimmerSliderStatus from 0x%02x: dim=%d%%",
			$addr, $dim);
}; # }}}

push @parser, sub { # Memory Block {,Write} (0xcc, 0xca) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 7;

	my ($cmd, $direction, $assign);
	if( $data[0] == 0xcc ) {
		$cmd = "MemoryBlock";
		$direction = "from";
		$assign = "=";
	} elsif( $data[0] == 0xca ) {
		$cmd = "MemoryBlockWrite";
		$direction = "to";
		$assign = "<=";
	} else {
		return undef;
	}

	my $mem_addr = ($data[1] << 8) + $data[2];
	my @mem_data = @data[3..6];

	return sprintf("$cmd $direction 0x%02x: \@0x%04x $assign %s",
			$addr, $mem_addr, join ' ', map { sprintf "%02x", $_; } @mem_data);
}; # }}}

push @parser, sub { # Memory Block Request (0xc9) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 3;
	return undef unless $data[0] == 0xc9;

	my $mem_addr = ($data[1] << 8) + $data[2];

	return sprintf("MemoryBlockRequest to 0x%02x: start address 0x%04x",
			$addr, $mem_addr);
}; # }}}

push @parser, sub { # Memory Data {,Write} (0xfe, 0xfc) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;

	my ($cmd, $direction, $assign);
	if( $data[0] == 0xfe ) {
		$cmd = "MemoryData";
		$direction = "from";
		$assign = "=";
	} elsif( $data[0] == 0xfc ) {
		$cmd = "MemoryDataWrite";
		$direction = "to";
		$assign = "<=";
	} else {
		return undef;
	}

	my $mem_addr = ($data[1] << 8) + $data[2];
	my $mem_data = $data[3];

	return sprintf("$cmd $direction 0x%02x: \@0x%04x $assign 0x%02x",
			$addr, $mem_addr, $mem_data);
}; # }}}

push @parser, sub { # Memory Data Request (0xfd) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 3;
	return undef unless $data[0] == 0xfd;

	my $mem_addr = ($data[1] << 8) + $data[2];

	return sprintf("MemoryDataRequest to 0x%02x: Address 0x%04x",
			$addr, $mem_addr);
}; # }}}

push @parser, sub { # Memory Dump Request (0xcb) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 1;
	return undef unless $data[0] == 0xcb;

	return sprintf("MemoryDumpRequest to 0x%02x", $addr);
}; # }}}

push @parser, sub { # Module Status (0xed, 5-byte version) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 5;
	return undef unless $data[0] == 0xed;

	my $input_status = $data[1];
	my $leds_on = $data[2];
	my $leds_slow_blink = $data[3];
	my $leds_fast_blink = $data[4];

	return sprintf("ModuleStatus from 0x%02x: inputs=0b%s LEDs: cont=0b%s slow=0b%s fast=0b%s",
			$addr, binary($input_status), binary($leds_on),
			binary($leds_slow_blink), binary($leds_fast_blink) );
}; # }}}

push @parser, sub { # Module Status (0xed, 7-byte version) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 7;
	return undef unless $data[0] == 0xed;

	my $input_status        = $data[1];
	my $enabled_status      = $data[2];
	my $not_inverted_status = $data[3];
	my $locked_status       = $data[4];
	my $program_disabled    = $data[5];

	my $program       = enum( $data[6] & 0x03,
			0=>"None", 1=>"Summer", 2=>"Winter", 3=>"Holiday" );
	my $alarm1        = $data[6] & 0x04 ? "on" : "off";
	my $alarm1_global = $data[6] & 0x80 ? "global" : "local";
	my $alarm2        = $data[6] & 0x10 ? "on" : "off";;
	my $alarm2_global = $data[6] & 0x20 ? "global" : "local";
	my $sunrise       = $data[6] & 0x40 ? "enabled" : "disabled";
	my $sunset        = $data[6] & 0x80 ? "enabled" : "disabled";

	return sprintf("ModuleStatus from 0x%02x: inputs=0b%s enabled=0b%s " .
			"noninverted=0b%s locked=0b%s progdisabled=0b%s " .
			"program=$program Alarms: 1($alarm1 $alarm1_global) " .
			"2($alarm2 $alarm2_global)  " .
			"Sunrise $sunrise Sunset $sunset",
			$addr, binary($input_status), binary($enabled_status),
			binary($not_inverted_status), binary($locked_status),
			binary($program_disabled));
}; # }}}

push @parser, sub { # Module Status Request (0xfa) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 2;
	return undef unless $data[0] == 0xfa;

	my $what = enum( $data[1],
			0x00 => "generic",
			0x03 => "Blind=1",
			0x0c => "Blind=2",
			0x01 => "Relay=1",
			0x02 => "Relay=2",
			0x04 => "Relay=3",
			0x08 => "Relay=4",
			0x10 => "Relay=5",
		);

	return sprintf("ModuleStatusRequest to 0x%02x: $what", $addr);
}; # }}}

push @parser, sub { # Module Type (0xff) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data >= 2;
	return undef unless $data[0] == 0xff;

	my $descr;
	if( $data[1] == 0x09 ) { # VMB2BL {{{
		return undef unless @data == 5;

		my %timeout = ( 0 => "15s", 1 => "30s", 2 => "1m", 3 => "2m" );
		my $timeout1 = enum( $data[2] & 0x03, %timeout );
		my $timeout2 = enum( ($data[2] >> 2) & 0x03, %timeout );
		my $build_year = $data[3];
		my $build_week = $data[4];

		$descr = "VMB2BL  Timeout blind 1=$timeout1 blind 2=$timeout2" .
			" Build $build_year-W$build_week";

	} # }}}
	elsif( $data[1] == 0x11 ) { # VMB4RYNO {{{
		return undef unless @data == 7;

		my $sn = ($data[2] << 8) + $data[3];
		my $mmap = $data[4];
		my $build_year = $data[5];
		my $build_week = $data[6];

		$descr = sprintf "VMB4RYNO  S/N: 0x%04x mmap v$mmap" .
			" Build $build_year-W$build_week",
			$sn;

	} # }}}
	else { # {{{
		$descr = sprintf "Unknown module [0x%02x] Data[%d]: %s",
			$data[1], scalar(@data)-2,
			join ' ', map { sprintf "%02x", $_ } @data[2..@data-1];
	} # }}}

	return sprintf("ModuleType from 0x%02x: $descr", $addr);
}; # }}}

push @parser, sub { # Name (0xf0, 0xf1, 0xf2) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;

	my $part;
	if( $data[0] == 0xf0 && @data == 8 ) {
		$part = 0;
	} elsif( $data[0] == 0xf1 && @data == 8 ) {
		$part = 1;
	} elsif( $data[0] == 0xf2 && @data == 6 ) {
		$part = 2;
	} else {
		return undef;
	}

	my $channel = $data[1];
	my $name = substr $data, 2;
	$name =~ s/\xff.*//;

	return sprintf("Name from 0x%02x: Item %s pt%d/3 \"$name\"",
			$addr, binary($channel), $part);
}; # }}}

push @parser, sub { # Name Request (0xef) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 2;
	return undef unless $data[0] == 0xef;

	my $what = enum($data[1],
			0x01 => "Relay/Pushbutton 1",
			0x02 => "Relay/Pushbutton 1",
			0x04 => "Relay/Pushbutton 1",
			0x08 => "Relay/Pushbutton 1",
			0x10 => "Relay/Pushbutton 1",
			0x20 => "Relay/Pushbutton 1",
			0x40 => "Relay/Pushbutton 1",
			0x80 => "Relay/Pushbutton 1",
			0x03 => "Blind 1",
			0x0c => "Blind 2",
		);

	return sprintf("NameRequest to 0x%02x: $what", $addr);
}; # }}}

push @parser, sub { # Push Button Status (0x00) {{{
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

push @parser, sub { # RTC status (0xd8) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0xd8;

	my $day = enum( $data[1],
			0 => "Monday",
			1 => "Tuesday",
			2 => "Wednesday",
			3 => "Thursday",
			4 => "Friday",
			5 => "Saturday",
			6 => "Sunday",
		);
	my $hour = $data[2];
	my $min = $data[3];

	return sprintf("RTCstatus to 0x%02x: %s %d:%02d",
			$addr, $day, $hour, $min);
}; # }}}

push @parser, sub { # Relay Status (0xfb) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 8;
	return undef unless $data[0] == 0xfb;

	my $relay_num = enum( $data[1],
			0x01 => 1,
			0x02 => 2,
			0x04 => 3,
			0x08 => 4,
			0x10 => 5,
		);;
	my $channel_mode = enum( $data[2],
			0 => "normal",
			1 => "inhibited",
			2 => "forced on",
			3 => "disabled",
		);
	my $relay_status = enum( $data[3],
			0 => "off",
			1 => "on",
			3 => "interval",
		);
	my $led_status = enum( $data[4],
			0x00 => "off",
			0x80 => "on",
			0x40 => "SlowBlink",
			0x20 => "FastBlink",
			0x10 => "VFastBlink",
		);
	my $timer = ($data[5] << 16) + ($data[6] << 8) + $data[7];

	return sprintf("RelayStatus of 0x%02x / %d : relay=%s status=%s LED=%s " .
			"timer=%d",
			$addr, $relay_num, $relay_status, $channel_mode, $led_status,
			$timer);
}; # }}}

push @parser, sub { # Sensor Temp (0xe6) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 7;
	return undef unless $data[0] == 0xe6;

	my $temp_cur = twos_complement( @data[1..2] ) / 512;
	my $temp_min = twos_complement( @data[3..4] ) / 512;
	my $temp_max = twos_complement( @data[5..6] ) / 512;

	return sprintf("SensorTemp from 0x%02x: Current=%3.2fC Min=%3.2fC Max=%3.2fC",
			$addr, $temp_cur, $temp_min, $temp_max);
}; # }}}

push @parser, sub { # Sensor Temp Request (0xe5) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 2;
	return undef unless $data[0] == 0xe5;

	my $autosend;
	if( $data[1] == 0 ) {
		$autosend = "disabled";
	} elsif( $data[1] <= 0 ) {
		$autosend = "onChange";
	} else {
		$autosend = $data[1] . "sec";
	}

	return sprintf("SensorTempRequest to 0x%02x: AutoSend %s", $addr, $autosend);
}; # }}}

push @parser, sub { # Set Dimvalue (at last used value) (0x07, 0x11) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 5;

	my $speed = ($data[3] << 8) | $data[4];
	if( $speed == 0 ) {
		$speed = "set by hex-switch";
	} elsif( $speed == 0xffff ) {
		$speed = "1.5 sec";
	} else {
		$speed = "$speed sec";
	}
	return undef unless $data[1] == 0x01;

	if( $data[0] == 0x07 ) {
		my $dim = $data[2];
		return sprintf("SetDimvalue to 0x%02x: dim=%d%% speed=$speed", $addr, $dim);

	} elsif( $data[0] == 0x11 ) {
		return sprintf("SetDimvalueToLast to 0x%02x: speed=$speed", $addr);

	} else {
		return undef;
	}
}; # }}}

push @parser, sub { # Set Temperature (0xe4) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 3;
	return undef unless $data[0] == 0xe4;

	my %temp_subcmd = (
		0 => "Current temperature set",
		1 => "Comfort temperature for heating set",
		2 => "Day temperature for heating set",
		3 => "Night temperature for heating set",
		4 => "Safe temperature for heating set",
		5 => "Temperature difference for turbo mode",
		5 => "Hysteresis",
		7 => "Comfort temperature for cooling set",
		8 => "Day temperature for cooling set",
		9 => "Night temperature for cooling set",
		10 => "Safe temperature for cooling set",
		11 => "Callibration",
		15 => "Low temperature alarm set",
		16 => "High temperature alarm set",
		17 => "Lower temperature range cool mode",
		18 => "Upper temperature range heat mode",
		20 => "Target temperature set for diff-sensor",
	);

	if( defined $temp_subcmd{ $data[1] } ) {
		return sprintf("SetTemperature to 0x%02x: %s <= %dºC",
				$addr, $temp_subcmd{$data[1]} , twos_complement($data[2]) / 2);
	} elsif( $data[1] == 12 ) {
		return sprintf("SetTemperature to 0x%02x: Reset %s temp",
				$addr, enum($data[2], 0x01 => "min", 0x02 => "max") );
	} elsif( $data[1] == 13 ) {
		return sprintf("SetTemperature to 0x%02x: Reset time statistics: %s %s",
				$addr,
				( $data[2] & 0x80 ? "heating" : "cooling" ),
				enum( $data[2] & 0x7f,
					0x01 => "safe",
					0x02 => "night",
					0x04 => "day",
					0x08 => "comfort",
					0x10 => "global",
				) );
	} elsif( $data[1] == 14 ) {
		return sprintf("SetTemperature to 0x%02x: Set unjamming: %svalve %spump",
				$addr,
				$data[2] & 0x02 ? "" : "no_",
				$data[2] & 0x01 ? "" : "no_",
			);
	} elsif( $data[1] == 19 ) {
		return sprintf("SetTemperature to 0x%02x: Set diff-sensor address <= 0x%02x",
				$addr, $data[2]);
	} elsif( $data[1] == 21 ) {
		my $min = $data[2];
		if( $min == 0xff ) { $min = 1; }
		return sprintf("SetTemperature to 0x%02x: Set minimum switching time <= %d mins",
				$addr, $min);

	} else {
		return sprintf("SetTemperature to 0x%02x: Unknown pointer 0x%02x (data=0x%02x)", $addr, $data[1], $data[2]);
	}
}; # }}}

push @parser, sub { # Start Relay {,Interval} Timer (0x03, 0x0d) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 5;

	my $cmd;
	if( $data[0] == 0x03 ) {
		$cmd = "StartRelayTimer";
	} elsif( $data[0] == 0x0d ) {
		$cmd = "StartRelayIntervalTimer";
	} else {
		return undef;
	}

	my $relay = enum( $data[1],
			0x01 => "1",
			0x02 => "2",
			0x04 => "3",
			0x08 => "4",
			0x10 => "5",
		);
	my $timeout = ($data[2] << 16) + ($data[3] << 8) + $data[4];
	if( $timeout == 0xffffff ) {
		$timeout = "permanent";
	} else {
		$timeout = sprintf "%ds", $timeout;
	}

	return sprintf("$cmd to 0x%02x: Relay=%s Timeout=$timeout",
			$addr, $relay);
}; # }}}

push @parser, sub { # Start Dimmer Timer (0x08) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 5;
	return undef unless $data[0] == 0x08;

	return undef unless $data[1] == 0x01;
	my $timeout = ($data[2] << 16) + ($data[3] << 8) + $data[4];
	if( $timeout == 0 ) {
		$timeout = "set by hex-switch";
	} elsif( $timeout >= 0xff0000 ) {
		$timeout = "permanent";
	} else {
		$timeout = sprintf "%ds", $timeout;
	}

	return sprintf("StartDimmerTimer to 0x%02x: Timeout=$timeout",
			$addr);
}; # }}}

push @parser, sub { # Switch Blind Off (0x04) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 2;
	return undef unless $data[0] == 0x04;

	my $blind = enum( $data[1],
			0x03 => "1",
			0x0c => "2",
		);

	return sprintf("SwitchBlindOff to 0x%02x: Blind=%s",
			$addr, $blind);
}; # }}}

push @parser, sub { # Switch Blind {Up,Down} (0x05, 0x06) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 5;

	my $cmd;
	if( $data[0] == 0x05 ) {
		$cmd = "SwitchBlindUp";
	} elsif( $data[0] == 0x06 ) {
		$cmd = "SwitchBlindDown";
	} else {
		return undef;
	}

	my $blind = enum( $data[1],
			0x03 => "1",
			0x0c => "2",
		);
	my $timeout = ($data[2] << 16) + ($data[3] << 8) + $data[4];
	if( $timeout == 0 ) {
		$timeout = "default";
	} elsif( $timeout == 0xffffff ) {
		$timeout = "permanent";
	} else {
		$timeout = sprintf "%ds", $timeout;
	}

	return sprintf("$cmd to 0x%02x: Blind=%s Timeout=$timeout",
			$addr, $blind);
}; # }}}

push @parser, sub { # Switch Relay {On,Off} (0x01, 0x02) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 2;

	my $cmd;
	if( $data[0] == 0x01 ) {
		$cmd = "SwitchRelayOff";
	} elsif( $data[0] == 0x02 ) {
		$cmd = "SwitchRelayOn";
	} else {
		return undef;
	}

	my $relay = enum( $data[1],
			0x01 => "1",
			0x02 => "2",
			0x04 => "3",
			0x08 => "4",
			0x10 => "5",
		);

	return sprintf("$cmd to 0x%02x: Relay=%s",
			$addr, $relay);
}; # }}}

push @parser, sub { # Switch To Mode (0xdb, 0xdc, 0xdd, 0xde) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 3;

	my $mode;
	if( $data[0] == 0xdb ) {
		$mode = "comfort";
	} elsif( $data[0] == 0xdc ) {
		$mode = "day";
	} elsif( $data[0] == 0xdd ) {
		$mode = "night";
	} elsif( $data[0] == 0xde ) {
		$mode = "safe";
	} else {
		return undef;
	}

	my $timer = ($data[1] << 8) + $data[2];
	if( $timer == 0 ) {
		$timer = "Cancel";
	} elsif( $timer == 0xff00 ) {
		$timer = "Program step";
	} elsif( $timer == 0xffff ) {
		$timer = "Manual";
	} else {
		$timer = sprintf "%dmin", $timer;
	}

	return sprintf("SwitchToMode to 0x%02x: switch to %s; Timer: $timer",
			$addr, $mode);
}; # }}}

push @parser, sub { # Temp Sensor Status (0xea) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 8;
	return undef unless $data[0] == 0xea;

	my $s = Struct("TSS",
		Magic("\xea"),
		BitStruct("operating mode",
			Bit("cooler mode"),
			BitField("mode2", 3),
			Bit("auto send sensor temp"),
			BitField("mode", 2),
			Bit("push button locked"),
		),
		BitStruct("program step mode",
			Bit("all program"),
			BitField("program step", 3),
			Bit("zone program"),
			Bit("sensor program"),
			Bit("unjamming valve"),
			Bit("unjamming pump"),
		),
		BitStruct("output status",
			Bit("reserved"),
			Bit("high alarm"),
			Bit("low alarm"),
			Bit("pump"),
			Bit("cooler"),
			Bit("day"),
			Bit("boost"),
			Bit("valve"),
		),
		SBInt8("temp halfC"),
		Value("temp C", sub { $_->ctx->{"temp halfC"} * 0.5; } ),
		SBInt8("target temp halfC"),
		Value("target temp C", sub { $_->ctx->{"target temp halfC"} * 0.5; } ),
		UBInt16("timer"),
	)->parse($data);

	$s->{"operating mode"}->{"mode"} = enum( $s->{"operating mode"}->{"mode"},
			0 => "run",
			1 => "manual",
			2 => "timer",
			3 => "disable",
		);
	my %mode = (
			0 => "safe",
			1 => "night",
			2 => "day",
			4 => "comfort",
		);
	$s->{"operating mode"}->{"mode2"} = enum( $s->{"operating mode"}->{"mode2"},
			%mode
		);
	$s->{"program step mode"}->{"program step"} = enum( $s->{"program step mode"}->{"program step"},
			%mode
		);

	return sprintf("TempSensorStatus from 0x%02x:" .
			" PB=" . ( $s->{"operating mode"}->{"push button locked"} ? "locked" : "unlocked" ) .
			" Mode=" . $s->{"operating mode"}->{"mode"} .
			" AutoSend=" . ( $s->{"operating mode"}->{"auto send sensor temp"} ? "on" : "off" ) .
			" Mode=" . $s->{"operating mode"}->{"mode2"} .
			" " . ( $s->{"operating mode"}->{"cooler"} ? "Cooler" : "Heater" ) .
			" Unjam={" .
				( $s->{"program step mode"}->{"unjamming pump"} ? "pump" : "no_pump" ) .
				" " .
				( $s->{"program step mode"}->{"unjamming valve"} ? "valve" : "no_valve" ) .
			"}" .
			" Program={" .
				( $s->{"program step mode"}->{"all program"} ? "all" : "no_all" ) .
				" " .
				( $s->{"program step mode"}->{"all zone"} ? "zone" : "no_zone" ) .
				" " .
				( $s->{"program step mode"}->{"all sensor"} ? "sensor" : "no_sensor" ) .
			"}" .
			" ProgramStep=" . $s->{"program step mode"}->{"program step"} .
			" Output={" .
				( $s->{"output status"}->{"valve"} ? "valve" : "no_valve" ) .
				" " .
				( $s->{"output status"}->{"boost"} ? "boost" : "no_boost" ) .
				" " .
				( $s->{"output status"}->{"day"} ? "day" : "no_day" ) .
				" " .
				( $s->{"output status"}->{"cooler"} ? "cooler" : "no_cooler" ) .
				" " .
				( $s->{"output status"}->{"pump"} ? "pump" : "no_pump" ) .
				" " .
				( $s->{"output status"}->{"low alarm"} ? "low_alarm" : "no_low_alarm" ) .
				" " .
				( $s->{"output status"}->{"high alarm"} ? "high_alarm" : "no_high_alarm" ) .
			"}" .
			" Temp=%2.1fC Target=%2.1fC Timer=%d",
			$addr, $s->{"temp C"}, $s->{"target temp C"}, $s->{"timer"});
}; # }}}

push @parser, sub { # Update LEDs (0xf4) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 4;
	return undef unless $data[0] == 0xf4;

	my $on = $data[1];
	my $sblink = $data[2];
	my $fblink = $data[3];

	# "The continuous on bit overrides the blinking modes"
	$sblink &= ~$on;
	$fblink &= ~$on;
	# "If the slow and fast blinking bits for a LED are both on, the LED
	#  blinks very fast."
	my $vfblink = $sblink & $fblink;
	$sblink &= ~$vfblink;
	$fblink &= ~$vfblink;

	return sprintf("UpdateLeds to 0x%02x: on=%s slowblink=%s fastblink=%s " .
			"veryfastblink=%s",
			$addr, binary($on), binary($sblink),
			binary($fblink), binary($vfblink) );
}; # }}}

push @parser, sub { # Write Address (0x6a) {{{
	my ($prio, $addr, $rtr, $data, @data) = @_;
	return undef unless $rtr  == 0;
	return undef unless @data == 7;
	return undef unless $data[0] == 0x6a;

	my $module_type = $data[1];
	my $cur_sn = ($data[2] << 8) + $data[3];
	my $new_addr = $data[4];
	my $new_sn = ($data[5] << 8) + $data[6];

	return sprintf("WriteAddr to 0x%02x: Current S/N: 0x%04x  " .
			"New addr: 0x%02x  New S/N: 0x%04x",
			$addr, $cur_sn, $new_addr, $new_sn);
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
