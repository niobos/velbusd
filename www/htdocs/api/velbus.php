<?php

function message($addr, $prio, $rtr, $data) {
	$msg = "\x0f"; $sum = 0x0f;

	$prio = $prio & 0x03 | 0xf8;
	$msg .= chr($prio); $sum += $prio;

	$msg .= chr($addr); $sum += $addr;

	$rtr = ($rtr & 0x01)<<6;
	$datalen = strlen($data);
	if( $datalen > 8 ) trigger_error("too much data to send", E_USER_ERROR);
	$rtr |= $datalen;
	$msg .= chr($rtr); $sum += $rtr;

	foreach( str_split($data) as $c ) {
		$msg .= $c; $sum += ord($c);
	}

	$sum = -( $sum & 0xff );
	$msg .= chr($sum);

	$msg .= "\x04";

	return $msg;
}

function read_message($sock, $timeout=1) {
	$read = array($sock);
	$write = NULL; $except = NULL;

	$msg = "";
	$start = microtime(true);

	$bytes_needed = 6;
	while( socket_select($read, $write, $except, floor($timeout), floor($timeout*1000000 % 1000000) ) ) {
		$timeout -= microtime(true) - $start;

		$buf = "";
		if( socket_recv($sock, $buf, $bytes_needed, 0) === FALSE ) trigger_error("Recv failed", E_USER_ERROR);

		$msg .= $buf;
		if( strlen($msg) < 6 ) {
			$bytes_needed -= strlen($buf);
			continue; # gather more data
		}

		$a = str_split($msg);
		if( $a[0] != "\x0f" ) trigger_error("Unsynchronized read, should not happen", E_USER_ERROR);

		$len = ord($a[3]) & 0x0f;
		if( strlen($msg) < 6+$len ) {
			$bytes_needed = $len;
			continue; # gather more data
		}

		return $msg;
	}
	return NULL;
}

function expect_answer($sock, $addr, $msg_filter=NULL, $timeout=1) {
	$start = microtime(true);

	while( $timeout > 0 ) {
		$msg = read_message($sock, $timeout);

		if( $msg == NULL ) return NULL; // Timeout

		$timeout -= microtime(true) - $start; // Correct timeout

		$a = str_split($msg);
		if( ord($a[2]) != $addr ) continue; # read next message
		if( $msg_filter != NULL ) {
			for( $i = 0; $i < strlen($msg); $i++ ) {
				if( $a[4+$i] != $msg_filter[$i] ) continue 2;
			}
		}

		return $msg;
	}
}

?>
