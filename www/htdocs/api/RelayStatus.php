<?php

require_once("config.php");
require_once("velbus.php");
require_once("output.php");

if( preg_match("%^/([0-9a-fA-F]{2})\.(\d)$%", $_SERVER["PATH_INFO"], $matches) == 0 ) {
	fail('Invalid syntax in URL');
}
$addr = hexdec($matches[1]);
$relay = (int)($matches[2]);

if( $relay == 0 ) fail('Invalid relay number');
if( $relay > 5 ) fail('Invalid relay number');
$relay_byte = chr(1 << ($relay-1));

$sock = socket_create(AF_INET6, SOCK_STREAM, 0);
if( $sock === FALSE ) fail("Could not create backend socket");

if( @socket_connect($sock, $config["host"], $config["port"]) === FALSE ) {
	fail("Could not connect");
}

switch( $_SERVER["REQUEST_METHOD"] ) {
case "POST":
	switch( $_POST["state"] ) {
	case "on":
		$cmd = 0x02;
		break;
	case "off":
		$cmd = 0x01;
		break;
	default: fail("invalid POST argument");
	}

	$msg = message($addr, 0, 0, chr($cmd) . $relay_byte );
	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) )
		fail("Send failed");
	// DON'T break, fall through to GET

case "GET":
	$ret = array(
		"id" => "$addr.$relay",
		"addr" => $addr,
		"relay" => $relay,
		);
	$msg = message($addr, 3, 0, chr(0xfa) . $relay_byte );
	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) )
		fail("Send failed");
	$msg = expect_answer($sock, $addr, "\xfb".$relay_byte, 1);

	if( $msg !== NULL ) {
		switch( ord($msg[7]) ) {
		case 0x00: $ret["state"] = "off"; break;
		case 0x01: $ret["state"] = "on"; break;
		case 0x03: $ret["state"] = "interval"; break;
		}
	}

	output($ret);
	break;

default:
	fail('Invalid method');
}

?>
