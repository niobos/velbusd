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

switch( $_SERVER["REQUEST_METHOD"] ) {
case "GET":
	$ret = array(
		"id" => "$addr",
		"addr" => $addr,
		);
	$msg = message($addr, 3, 0, chr(0xe5) . chr(0) );
	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) )
		fail("Send failed");
	$msg = expect_answer($sock, $addr, "\xe6");

	if( $msg !== NULL ) {
		$ret["temp_raw"] = ord($msg[5])*256 + ord($msg[6]);
		if( $ret["temp_raw"] < 0x7fff ) {
			$ret["temp"] = (float)$ret["temp_raw"] / 512;
		} else {
			$ret["temp"] = (float)(0x8000 - $ret["temp_raw"]) / 512;
		}
	}

	print json_encode($ret);
	break;

default:
	fail('Invalid method');
}

?>
