<?php

require_once("../api/config.php");

$h = opendir(".");
if( $h === FALSE ) {
	fail("Couldn't opendir('.')");
}

while (false !== ($entry = readdir($h))) {
	if( preg_match("%\.js$%", $entry) > 0 ) {
		if( readfile($entry) === FALSE ) {
			fail("Couldn't readfile('$entry')");
		}
		echo "\n";
	}
}

closedir($h);

?>
