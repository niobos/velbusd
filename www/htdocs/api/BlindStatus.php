<?php

require_once("config.php");
require_once("velbus.php");

if( preg_match("%^/([0-9a-fA-F]{2})\.(\d)$%", $_SERVER["PATH_INFO"], $matches) == 0 ) {
	fail('Invalid syntax in URL');
}
$addr = hexdec($matches[1]);
$blind = (int)($matches[2]);

if( $blind == 0 ) fail('Invalid blind number');
if( $blind > 2 ) fail('Invalid blind number');
$blind_byte = chr(3 << (($blind-1)*2) );

$sock = socket_create(AF_INET6, SOCK_STREAM, 0);
if( $sock === FALSE ) trigger_error("Could not create backend socket", E_ERROR_USER);

if( socket_connect($sock, $config["host"], $config["port"]) === FALSE ) {
	trigger_error("Could not connect", E_USER_ERROR);
}

switch( $_SERVER["REQUEST_METHOD"] ) {
case "POST":
	switch( $_POST["state"] ) {
	case "stop":
		$cmd = "\x04{$blind_byte}";
		break;
	case "up":
		$cmd = "\x05{$blind_byte}\x00\x00\x00"; // Default duration
		break;
	case "down":
		$cmd = "\x06{$blind_byte}\x00\x00\x00"; // Default duration
		break;
	default: fail("invalid POST argument");
	}

	$msg = message($addr, 0, 0, $cmd );
	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) )
		trigger_error("Send failed", E_ERROR_USER);
	// DON'T break, fall through to GET

case "GET":
	$ret = array(
		"id" => "$addr.$blind",
		"addr" => $addr,
		"blind" => $blind,
		);
	$msg = message($addr, 3, 0, chr(0xfa) . $blind_byte );
	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) )
		trigger_error("Send failed", E_ERROR_USER);
	$msg = expect_answer($sock, $addr, "\xec".$blind_byte, 1);

	if( $msg !== NULL ) {
		switch( ord($msg[7]) ) {
		case 0x00: $ret["state"] = "stopped"; break;
		case 0x01:
		case 0x04: $ret["state"] = "going up"; break;
		case 0x02:
		case 0x08: $ret["state"] = "going down"; break;
		}
	}

	print json_encode($ret);
	break;

default:
	fail('Invalid method');
}

?>
