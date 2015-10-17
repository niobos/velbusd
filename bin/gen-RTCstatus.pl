#!/usr/bin/perl

use strict;
use warnings;

sub checksum ($) {
	my $sum = unpack("%8C*", $_[0]);
	return chr( (256-$sum)%256 );
}

my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);

$wday--; $wday = 6 if $wday == -1; # convert 0=Sunday to 0=Monday

my $rtc_message = "\x0f\xfb\x00\x04\xd8" . chr($wday) . chr($hour) . chr($min);
$rtc_message .= checksum($rtc_message);
$rtc_message .= "\x04";

print $rtc_message;
