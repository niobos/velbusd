<?php

require_once('config.php');

if( preg_match("%^([0-9a-fA-F]+)$%", $_GET["id"], $matches ) == 0 ) {
	fail('Invalid `id` in GET parameter');
}
$id = $matches[1];

$filename = $config["rrddir"] . "/" . $id . "-TS.rrd";
if( ! is_readable($filename) ) {
	fail("Could not read file `$filename`");
}

if( preg_match("%^([a-zA-Z0-9 _-]*)$%", $_GET["name"], $matches) == 0 ) {
	fail("Invalid `name` in GET parameters");
}
$name = $matches[1];
if( $name == "" ) { $name = $id; }

$start = "-1 day";
$end = "now";

header('Content-type: image/png');
passthru("rrdtool graph - --imgformat PNG" .
	" --start '$start' --end '$end'" .
	" --title 'Temperature sensor $name'" .
	" --vertical-label '°C'" .
	" --alt-autoscale" . 
	" 'DEF:temp=$filename:temperature:AVERAGE'" .
	" 'DEF:tt=$filename:set_temperature:AVERAGE'" .
	" 'DEF:heater=$filename:heater:AVERAGE'" .
	" TICK:heater#ff000040:1" .
	" LINE1:temp#ff0000:'Actual temperature'" .
	" LINE1:tt#0000ff:'Target temperature'" .
	" CDEF:empty=UNKN,temp,POP" .
	" LINE1:empty#ff000040:'Heat requested'" .
	"");

?>
