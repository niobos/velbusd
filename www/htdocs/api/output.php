<?php

function accept_header_options($type_options) {
    $precedence = 1;
    $tokens = array();
    if (is_string($type_options)) {
        $type_options = explode(';', $type_options);
    }
    $type_options = array_map('trim', $type_options);
    foreach ($type_options as $option) {
        $option = explode('=', $option);
        $option = array_map('trim', $option);
        if ($option[0] == 'q') {
            $precedence = $option[1];
        } else {
            $tokens[$option[0]] = $option[1];
        }
    }
    $tokens = count ($tokens) ? $tokens : false;
    return array($precedence, $tokens);
}

function accept_header ($header) {
	$accept = array_map('trim', explode(',', $header));
	foreach( $accept as $type ) {
		$type = explode(';', $type);
		$mime = array_shift($type);
		if( $mime ) {
			list($precedence, $tokens) = accept_header_options($type);
			$toret[] = array('mimetype' => $mime, 'precedence' => (float)$precedence, 'tokens' => $tokens);
		}
	}
	usort($toret, 'sort_precedence_htl');
	return $toret;
}

function sort_precedence_htl($a, $b) {
	if( $a["precedence"] < $b["precedence"] ) return 1;
	if( $a["precedence"] == $b["precedence"] ) return 0;
	return -1;
}


function text ($o) {
	$ret = '';
	foreach( $o as $k => $v ) {
		$k = preg_replace('/ /', '_', $k);
		$ret .= "$k:$v\n";
	}
	return $ret;
}

function output ($o) {
	$accept = accept_header($_SERVER['HTTP_ACCEPT']);

	# find best matching output type
	while( count($accept) ) {
		$type = array_shift($accept);

		switch($type["mimetype"]) {
		case "application/json":
			print json_encode($o);
			return;
		case "text/plain":
			print text($o);
			return;
		}
	}

	# default
	print text($o);
}

?>
