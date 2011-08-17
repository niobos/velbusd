#!/usr/bin/perl

use strict;
use warnings;

$| = 1; # autoflush

sub parse_message($$$$) {
	my ($prio, $addr, $rtr, $data) = @_;

	my @cdata = split //, $data;
	my @ndata = map { ord($_) } @cdata;
	my $len = length($data);

	if( $len == 0 ) {
		if( $rtr == 1 ) {
			printf "  Module 0x%02x type request\n", $addr;
		} else {
			return;
		}

	} elsif( $ndata[0] == 0x00 && $len == 4 ) {
		printf "  Status of 0x%02x: on=%08b off=%08b long=%08b\n",
		       $addr, $ndata[1], $ndata[2], $ndata[3];

	} elsif( $ndata[0] == 0x01 && $len == 2 ) {
		printf "  Switch off relay 0x%02x / %d\n",
		       $addr,
		       ( 1+ log( $ndata[1] ) / log(2) );

	} elsif( $ndata[0] == 0x02 && $len == 2 ) {
		printf "  Switch on relay 0x%02x / %d\n",
		       $addr,
		       ( 1+ log( $ndata[1] ) / log(2) );

	} elsif( $ndata[0] == 0x03 && $len == 5 ) {
		my $time = ($ndata[2] << 16) + ($ndata[3] << 8) + $ndata[4];
		if( $time == 0 ) { $time = "[default]";
		} elsif( $time == 0xffffff ) { $time = "permanently on";
		} else { $time = "$time secs"; }
		printf "  Start relay timer of relay 0x%02x / %d time %s\n",
		       $addr,
		       ( 1+ log( $ndata[1] ) / log(2) ),
		       $time;

	} elsif( $ndata[0] == 0x04 && $len == 2 ) {
		my $blind = ($ndata[1] == 0x03 ? "1" : ($ndata[1] == 0x0C ? "2" : "?" ) );
		printf "  Switch 0x%02x blind %d off\n",
		       $addr, $blind;

	} elsif( $ndata[0] == 0x05 && $len == 5 ) {
		my $blind = ($ndata[1] == 0x03 ? "1" : ($ndata[1] == 0x0C ? "2" : "?" ) );
		my $time = ($ndata[2]<<16)+($ndata[3]<<8)+$ndata[4];
		if( $time == 0 ) { $time = "[default timer]";
		} elsif( $time == 0xffffff ) { $time = "permanently";
		} else { $time = "for $time secs"; }
		printf "  Switch 0x%02x blind %d up %s\n",
		       $addr, $blind, $time;

	} elsif( $ndata[0] == 0x06 && $len == 5 ) {
		my $blind = ($ndata[1] == 0x03 ? "1" : ($ndata[1] == 0x0C ? "2" : "?" ) );
		my $time = ($ndata[2]<<16)+($ndata[3]<<8)+$ndata[4];
		if( $time == 0 ) { $time = "[default timer]";
		} elsif( $time == 0xffffff ) { $time = "permanently";
		} else { $time = "for $time secs"; }
		printf "  Switch 0x%02x blind %d down %s\n",
		       $addr, $blind, $time;

	} elsif( $ndata[0] == 0x09 && $len == 1 ) {
		printf "  From 0x%02x: Bus off\n", $addr;

	} elsif( $ndata[0] == 0x0A && $len == 1 ) {
		printf "  From 0x%02x: Bus active\n", $addr;

	} elsif( $ndata[0] == 0x0B && $len == 1 ) {
		printf "  From 0x%02x: Rx buffer full\n", $addr;

	} elsif( $ndata[0] == 0x0C && $len == 1 ) {
		printf "  From 0x%02x: Rx ready\n", $addr;

	} elsif( $ndata[0] == 0x0D && $len == 5 ) {
		my $time = ($ndata[2]<<16) + ($ndata[3]<<8) + $ndata[4];
		if( $time == 0 ) { $time = "[default]";
		} elsif( $time == 0xffffff ) { $time = "permanently on";
		} else { $time = "$time secs"; }
		printf "  Start relay blinking timer of 0x%02x / %d time %s\n",
		       $addr,
		       ( 1+ log( $ndata[1] ) / log(2) ),
		       $time;

	} elsif( $ndata[0] == 0x0E && $len == 1 ) {
		printf "  From 0x%02x: Interface status request\n", $addr;

	} elsif( $ndata[0] == 0x6A && $len == 7 ) {
		printf "  Write module & s/n: addr:%02x->%02x S/N:%04x->%04x\n",
		       $addr, $ndata[4],
		       ($ndata[2]<<8)+$ndata[3], ($ndata[5]<<8)+$ndata[6];

	} elsif( $ndata[0] == 0xC9 && $len == 3 ) {
		printf "  Read data block from 0x%02x @ memory 0x%04x+4\n",
		       $addr, ($ndata[1]<<8)+$ndata[2];

	} elsif( $ndata[0] == 0xCA && $len == 7 ) {
		my $memaddr = ($ndata[1]<<8) + $ndata[2];
		printf "  Write memory block to 0x%02x @ 0x%04x+4: 0x %02x %02x %02x %02x\n",
		       $addr, $memaddr,
		       $ndata[3], $ndata[4], $ndata[5], $ndata[6];

	} elsif( $ndata[0] == 0xCB && $len == 1 )  {
		printf "  Request memory dump of %02x\n", $addr;

	} elsif( $ndata[0] == 0xCC && $len == 7 ) {
		my $memaddr = ($ndata[1]<<8) + $ndata[2];
		printf "  Memory block from 0x%02x at 0x%04x+4: 0x %02x %02x %02x %02x\n",
		       $addr, $memaddr,
		       $ndata[3], $ndata[4], $ndata[5], $ndata[6];

	} elsif( $ndata[0] == 0xD9 && $len == 1 ) {
		printf "  Bus error counter status request for 0x%02x\n",
		       $addr;

	} elsif( $ndata[0] == 0xDA && $len == 4 ) {
		printf "  Bus error counter of 0x%02x: Tx=%d Rx=%d BusOff=%d\n",
		       $addr, $ndata[1], $ndata[2], $ndata[3];

	} elsif( $ndata[0] == 0xEC && $len == 8) {
		my $blind = ($ndata[1] == 0x03 ? "1" : ($ndata[1] == 0x0C ? "2" : "?" ) );
		my %timeout = ( 0x00 => "15s", 0x01 => "30s", 0x02 => "1min", 0x03 => "2min" );
		printf "  Blind status 0x%02x / %d: timeout=%s status=%08b LEDS=%08b cur_delay=%d\n",
		       $addr, $blind, $timeout{ $ndata[2] }, $ndata[3], $ndata[4],
		       ($ndata[5]<<16)+($ndata[6]<<8)+$ndata[7];

	} elsif( $ndata[0] == 0xED && $len == 5 ) {
		printf "  Module 0x%02x status: in=%08b LEDs_on=%08b LEDs_blink=%08b LEDs_fblink=%08b\n",
		       $addr, $ndata[1], $ndata[2], $ndata[3], $ndata[4];

	} elsif( $ndata[0] == 0xEF && $len == 2 ) {
		printf "  Name request 0x%02x / %08b\n",
		       $addr,
			   $ndata[1];

	} elsif( $ndata[0] == 0xF0 && $len == 8 ) {
		printf "  Name of 0x%02x / %08b, pt1/3: \"%s\"\n",
		       $addr,
		       $ndata[1],
		       substr($data, 2);

	} elsif( $ndata[0] == 0xF1 && $len == 8 ) {
		printf "  Name of 0x%02x / %08b, pt2/3: \"%s\"\n",
		       $addr,
		       $ndata[1],
		       substr($data, 2);

	} elsif( $ndata[0] == 0xF2 && $len == 6 ) {
		printf "  Name of 0x%02x / %08b, pt3/3: \"%s\"\n",
		       $addr,
		       $ndata[1],
		       substr($data, 2);

	} elsif( $ndata[0] == 0xF4 && $len == 4 ) {
		printf "  Set LEDs of 0x%02x: on=%08b blink=%08b fblink=%08b\n",
		       $addr, $ndata[1], $ndata[2], $ndata[3];

	} elsif( $ndata[0] == 0xF5 && $len == 2 ) {
		printf "  Clear PB 0x%02x LEDs %08b\n",
		       $addr, $ndata[1];

	} elsif( $ndata[0] == 0xF6 && $len == 2 ) {
		printf "  Set PB 0x%02x LEDs %08b\n",
		       $addr, $ndata[1];

	} elsif( $ndata[0] == 0xF7 && $len == 2 ) {
		printf "  Blink PB 0x%02x LEDs %08b slowly\n",
		       $addr, $ndata[1];

	} elsif( $ndata[0] == 0xF8 && $len == 2 ) {
		printf "  Blink PB 0x%02x LEDs %08b fast\n",
		       $addr, $ndata[1];

	} elsif( $ndata[0] == 0xF9 && $len == 2 ) {
		printf "  Blink PB 0x%02x LEDs %08b very fast\n",
		       $addr, $ndata[1];

	} elsif( $ndata[0] == 0xFA && $len == 2 ) {
		printf "  Status request 0x%02x / %08b\n",
		       $addr,
		       $ndata[1];

	} elsif( $ndata[0] == 0xFB && $len == 8 ) {
		my %dif = ( 0 => "normal", 1 => "inhibited", 2 => "forced on", 3 => "disabled" );
		my %oot = ( 0 => "off", 1 => "on", 2 => "unknown", 3 => "interval-timer" );
		my %led = ( 0x00 => "off", 0x80 => "on", 0x40 => "slow blinking",
		            0x20 => "fast blinking", 0x10 => "very fast blinking");
		my $time = ($ndata[5] << 16) + ($ndata[6] << 8) + $ndata[7];
		printf "  Relay status of 0x%02x: relay=%d %s %s LED=%s Timer=%d secs\n",
		       $addr,
		       ( 1+ log( $ndata[1] ) / log(2) ),
		       $dif{ $ndata[2] & 0x03 },
		       $oot{ $ndata[3] & 0x03 },
		       $led{ $ndata[4] },
		       $time;

	} elsif( $ndata[0] == 0xFC && $len == 4 ) {
		printf "  Write data 0x%02x to 0x%02x @ memory 0x%04x\n",
		       $ndata[3], $addr, ($ndata[1]<<8)+$ndata[2];

	} elsif( $ndata[0] == 0xFD && $len == 3 ) {
		printf "  Read data from 0x%02x @ memory 0x%04x\n",
		       $addr, ($ndata[1]<<8)+$ndata[2];

	} elsif( $ndata[0] == 0xFE && $len == 4 ) {
		printf "  Memory data from 0x%02x at 0x%04x = 0x%02x\n",
		       $addr, ($ndata[1]<<8) + $ndata[2], $ndata[3];

	} elsif( $ndata[0] == 0xFF ) {
		if( $ndata[1] == 0x11 && $len == 7 ) {
			printf "  Module 0x%02x type VMB4RYNO S/N=0x%04x MMAP=%02x Build=%d/%d\n",
			       $addr,
			       ($ndata[2]<<16) + $ndata[3],
			       $ndata[4],
			       $ndata[5],
			       $ndata[6];
		} elsif( $ndata[1] == 0x01 && $len == 7 ) {
			printf "  Module 0x%02x type VMB8PB LEDs_on=%08b LEDs_blink=%08b LEDs_fblink=%08b build=%d/%d\n",
		       $addr, $ndata[2], $ndata[3], $ndata[4], $ndata[5], $ndata[6];
		} elsif( $ndata[1] == 0x09 && $len == 5 ) {
			my %timer = ( 0x00 => "15s", 0x01 => "30s", 0x02 => "1min", 0x03 => "2min" );
			printf "  Module 0x%02x type VMB2BL: timers 1:%s 2:%s\n",
				   $addr, $timer{ $ndata[2] & 0x03 }, $timer{ ($ndata[2] & 0x0C) >> 2 };
		} else {
			printf "  Module 0x%02x type unknown\n", $addr;
		}

	} else {
		printf "  Unknown command or incorrect length 0x%02x\n", $ndata[0];
	}
}

sub hexdump ($) {
	return join ' ', map { sprintf "%02x", ord($_) } split //, $_[0];
}

sub get_more_data() {
	my $rv = sysread(STDIN, my $in, 255);
	if( $rv == 0 ) { exit 0; }
	return $in;
}

my $buffer = get_more_data();
while( 1 ) { # Max length of a message
	$buffer .= get_more_data() until length($buffer) >= 6;

	if( substr($buffer, 1, 1) eq "\x0F" ) {
		print STDERR "Start of Tx is not 0x0F, assuming lost sync\n";
		$buffer = substr($buffer, 1); # skip 1 char
		next;
	}

	my @header = unpack("CB8CB8", $buffer);
	my $checksum = 0; $checksum += unpack("%8C*", substr($buffer, 0, 4));
	$buffer = substr($buffer, 4);

	if( $header[1] !~ m/^111110([01])([01])$/ ) {
		print STDERR "Second byte does not match 0xF8/0xFC\n";
		next
	}
	my $priority = 0; # lower values are more important
	$priority += 2 if $1 == '1';
	$priority += 1 if $2 == '1';

	my $address = $header[2];

	if( $header[3] !~ m/^0([01])00([01]{4})$/ ) {
		print STDERR "Form Error in byte 4\n";
		next;
	}
	my $RTR = $1;
	my $datalen = unpack("C", pack("B8", "0000$2"));

	$buffer .= get_more_data() until length($buffer) >= $datalen;
	my $data = substr($buffer, 0, $datalen);
	$checksum += unpack("%8C*", substr($buffer, 0, $datalen));
	$buffer = substr($buffer, $datalen);

	$checksum = 0x100 - ($checksum & 0xff);
	$buffer .= get_more_data() until length($buffer) >= 2;
	my ($their_checksum, $etx) = unpack("CC", $buffer);
	$buffer = substr($buffer, 2);

	if( $checksum != $their_checksum ) {
		print STDERR "Checksum mismatch ".$checksum."!=".$their_checksum."\n";
		next;
	}

	if( $etx != 0x04 ) {
		print STDERR "End of Tx is not 0x04\n";
		next;
	}

	my $ts;
	{
		my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
		$year += 1900; $mon += 1;
		$ts = sprintf "%04d-%02d-%02d %02d:%02d:%02d",
		              $year, $mon, $mday, $hour, $min, $sec;
	}
	printf "%s Prio:%d Addr:%02x RTR:%d data[%d]: %s\n",
	       $ts, $priority, $address, $RTR, $datalen, hexdump($data);

	parse_message($priority, $address, $RTR, $data);
}
