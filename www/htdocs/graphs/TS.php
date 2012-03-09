<?php

require_once('config.php');

$id    = filter_get("id", "%^([0-9a-fA-F]+)$%", "");

$filename = $config["rrddir"] . "/" . $id . "-TS.rrd";
if( ! is_readable($filename) ) {
	fail("Could not read file `$filename`");
}
$name   = filter_get("name",   "%^([a-zA-Z0-9 _-]*)$%", $id);
$start  = filter_get("start",  "%^([0-9a-zA-Z /:-]+)$%", "-1 day");
$end    = filter_get("end",    "%^([0-9a-zA-Z /:-]+)$%", "now");
$width  = filter_get("width",  "%^([0-9]+)$%", "400");
$height = filter_get("height", "%^([0-9]+)$%", "100");


header('Content-type: image/png');
passthru("rrdtool graph - --imgformat PNG" .
	" --width $width --height $height" .
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
