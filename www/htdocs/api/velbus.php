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

function expect_answer($sock, $addr, $cmd_filter=NULL) {

}

function send_command($addr, $prio, $rtr, $data, $answer_cmd=NULL) {
	$msg = message($addr, $prio, $rtr, $data);

	$sock = socket_create(AF_INET6, SOCK_STREAM, 0);
	if( $sock === FALSE ) trigger_error("Could not create backend socket", E_USER_ERROR);

	if( socket_set_option($sock, SOL_SOCKET, SO_RCVTIMEO, array("sec"=>1,"usec"=>0) ) === FALSE )
		trigger_error("Could not set_socket_opt()", E_USER_ERROR);

	if( socket_connect($sock, "::1", 8445) === FALSE ) trigger_error("Could not connect", E_USER_ERROR);

	if( socket_send($sock, $msg, strlen($msg), 0) != strlen($msg) ) trigger_error("Send failed", E_USER_ERROR);

	$answer = NULL;
	$timeout = microtime(true)+1;
	$msg = "";
	while( $answer_cmd != NULL && microtime(true) < $timeout ) { 
		$buf = "";
		if( socket_recv($sock, $buf, 128, 0) === FALSE ) trigger_error("Recv failed", E_USER_ERROR);

		$msg .= $buf;

		while(1) {
			if( strlen($msg) < 6 ) continue 2; # gather more data

			$a = str_split($msg);
			if( $a[0] != "\x0f" ) trigger_error("Unsynchronized read, should not happen", E_USER_ERROR);

			$len = ord($a[3]) & 0x0f;
			if( strlen($msg) < 6+$len ) continue 2; # gather more data

			# consume message; copy is in $a anyway
			$thismsg = substr($msg, 0, 6+$len);
			$msg = substr($msg, 6+$len);
			
			if( ord($a[2]) != $addr ) continue 1; # read next message
			if( ord($a[4]) != $answer_cmd ) continue 1;

			$answer = $thismsg;
			break 2;
		}
	}

	socket_close($sock);

	return $answer;
}


?>
