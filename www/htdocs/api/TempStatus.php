<?php

require_once("config.php");
require_once("velbus.php");

if( preg_match("%^/([0-9a-fA-F]{2})$%", $_SERVER["PATH_INFO"], $matches) == 0 ) {
	fail('Invalid syntax in URL');
}
$addr = hexdec($matches[1]);

$sock = socket_create(AF_INET6, SOCK_STREAM, 0);
if( $sock === FALSE ) fail("Could not create backend socket");

if( @socket_connect($sock, $config["host"], $config["port"]) === FALSE ) {
	fail("Could not connect");
}

function twoscomplement ($tc, $bytes = 1) {
	$threshold = 1 << (($bytes*8)-1);
	if( $tc < $threshold ) {
		 return $tc;
	} else {
		return $threshold - $tc;
	}
}

switch( $_SERVER["REQUEST_METHOD"] ) {
case "GET":
	$ret = array(
		"id" => "$addr",
		"addr" => $addr,
		);
	$msg = message($addr, 3, 0, chr(0xfa) . chr(0) );
	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) )
		fail("Send failed");
	$msg = expect_answer($sock, $addr, chr(0xea));

	if( $msg !== NULL ) {
		$ret["push button"] = ord($msg[5]) &  0x01 ? "locked" : "unlocked";
		switch(ord($msg[5]) & 0x06) {
		case 0x00: $ret["mode"] = "run"; break;
		case 0x02: $ret["mode"] = "manual"; break;
		case 0x04: $ret["mode"] = "sleep timer"; break;
		case 0x06: $ret["mode"] = "disabled"; break;
		default: $ret["mode"] = "unknown"; break;
		}
		$ret["auto send temp"] = (bool)(ord($msg[5]) & 0x08);
		switch(ord($msg[5]) & 0x70) {
		case 0x00: $ret["temperature mode"] = "anti frost"; break;
		case 0x10: $ret["temperature mode"] = "night"; break;
		case 0x20: $ret["temperature mode"] = "day"; break;
		case 0x40: $ret["temperature mode"] = "comfort"; break;
		default: $ret["temperature mode"] = "unknown"; break;
		}
		$ret["heating cooling"] = ord($msg[5]) & 0x80 ? "cooling" : "heating";

		$ret["heater"] = (bool)(ord($msg[7]) & 0x01);
		$ret["boost"] = (bool)(ord($msg[7]) & 0x02);
		$ret["day"] = (bool)(ord($msg[7]) & 0x04);
		$ret["cooler"] = (bool)(ord($msg[7]) & 0x08);
		$ret["pump"] = (bool)(ord($msg[7]) & 0x10);
		$ret["low alarm"] = (bool)(ord($msg[7]) & 0x20);
		$ret["high alarm"] = (bool)(ord($msg[7]) & 0x40);

		$ret["temp"] = twoscomplement(ord($msg[8])) / 2;
		$ret["target temp"] = twoscomplement(ord($msg[9])) / 2;

		$ret["sleep timer"] = ord($msg[10]) * 256 + ord($msg[11]);
	}

	print json_encode($ret);
	break;

default:
	fail('Invalid method');
}

?>
