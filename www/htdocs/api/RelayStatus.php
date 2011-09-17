<?php

require_once("config.php");
require_once("velbus.php");

if( preg_match("%^/([0-9a-fA-F]{2})\.(\d)$%", $_SERVER["PATH_INFO"], $matches) == 0 ) {
	fail('Invalid syntax in URL');
}
$addr = hexdec($matches[1]);
$relay = (int)($matches[2]);


$sock = socket_create(AF_INET6, SOCK_STREAM, 0);
if( $sock === FALSE ) trigger_error("Could not create backend socket", E_ERROR_USER);

//if( socket_connect($sock, $config["host"], $config["port"]) === FALSE ) {
//	trigger_error("Could not connect", E_USER_ERROR);
//}

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

	$msg = message($addr, 0, 0, chr($cmd) . chr(1 << $relay) );
	// DON'T break, fall through to GET

case "GET":
	$ret = array(
		"id" => "$addr.$relay",
		"addr" => $addr,
		"relay" => $relay,
		"state" => "on",
		);
	print json_encode($ret);
	break;

default:
	fail('Invalid method');
}

?>
