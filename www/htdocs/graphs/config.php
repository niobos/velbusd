<?php

$config = array();
$config["rrddir"] = "/mnt/data/rrdtool/";

function fail($explanation) {
	header('X-PHP-Bug: Requires to add a header to change status', true, 500);
	print "$explanation";
	exit();
}

?>
