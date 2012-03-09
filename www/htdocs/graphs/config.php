<?php

$config = array();
$config["rrddir"] = "/mnt/data/rrdtool/";

function fail($explanation) {
	header('X-PHP-Bug: Requires to add a header to change status', true, 500);
	print "$explanation";
	exit();
}

function filter_get($param_name, $regex, $default="", $default_if_no_match=FALSE) {
	if( ! array_key_exists( $param_name, $_GET ) ) {
		return $default;
	}
	if( preg_match($regex, $_GET[$param_name], $matches) == 0 ) {
		if( $default_if_no_match ) {
			return $default;
		} else {
			return NULL;
		}
	}
	return $matches[1];
}

?>
