<?php

$config = array();
$config["host"] = "::1";
$config["port"] = 8445;

function fail($explanation) {
	header('X-PHP-Bug: Requires to add a header to change status', true, 500);
	print "$explanation";
	exit();
}

?>
