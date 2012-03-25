<?php

require_once('config.php');

$id    = filter_get("id", "%^([0-9a-fA-F]+)$%", "");
$filename = $config["rrddir"] . "/" . $id . "-TS.rrd";
if( ! is_readable($filename) ) {
	fail("Could not read file `$filename`");
}

$id_relay = filter_get("id_relay", "%^([0-9a-fA-F]+.[0-9])$%", "");
if( $id_relay != "" ) {
	$filename_relay = $config["rrddir"] . "/" . $id_relay . "-RELAY.rrd";
	if( ! is_readable($filename_relay) ) {
		fail("Could not read file `$filename_relay`");
	}
} else {
	$filename_relay = FALSE;
}

$name   = filter_get("name",   "%^([a-zA-Z0-9 _-]*)$%", $id);
$start  = strtotime(filter_get("start",  "%^([0-9a-zA-Z /:-]+)$%", "-1 day"));
if( $start === FALSE ) { fail("Couldn't parse start date"); }
$end    = strtotime(filter_get("end",    "%^([0-9a-zA-Z /:-]+)$%", "now"));
if( $end === FALSE ) { fail("Couldn't parse end date"); }
$width  = filter_get("width",  "%^([0-9]+)$%", "400");
$height = filter_get("height", "%^([0-9]+)$%", "100");


header('Content-type: image/png');
passthru("rrdtool graph - --imgformat PNG" .
	" --width $width --height $height" .
	" --start $start --end $end" .
	" --title 'Temperature sensor $name'" .
	" --vertical-label '°C'" .
	" --alt-autoscale" . 
	" 'DEF:temp=$filename:temperature:AVERAGE'" .
	" 'DEF:tt=$filename:set_temperature:AVERAGE'" .
	" 'DEF:heater=$filename:heater:AVERAGE'" .
	( $filename_relay ? " 'DEF:relay=$filename_relay:state:AVERAGE'" : "" ) .
	( $filename_relay ? " 'TICK:relay#a0a00030:1'" : "" ) .
	" TICK:heater#ff000030:1" .
	" LINE1:temp#ff0000:'Actual temperature'" .
	" VDEF:temp_avg=temp,AVERAGE" .
	" VDEF:temp_min=temp,MINIMUM" .
	" VDEF:temp_max=temp,MAXIMUM" .
	" 'GPRINT:temp_avg:avg=%4.1lf°C'" .
	" 'GPRINT:temp_min:min=%4.1lf°C'" .
	" 'GPRINT:temp_max:max=%4.1lf°C'" .
	" COMMENT:\\\\n" .
	" LINE1:tt#0000ff:'Target temperature'" .
	" VDEF:tt_avg=tt,AVERAGE" .
	" 'GPRINT:tt_avg:avg=%4.1lf°C'" .
	" COMMENT:\\\\n" .
	" CDEF:empty=UNKN,temp,POP" .
	" LINE1:empty#ffa00060:'Heat requested here '" .
	" CDEF:heater_100=heater,100,*" .
	" VDEF:duty=heater_100,AVERAGE" .
	" 'GPRINT:duty:duty=%4.2lf%%'" .
	" COMMENT:\\\\n" .
	( $filename_relay ? " LINE1:empty#a0a00030:'Heat requested total'" .
	  " CDEF:relay_100=relay,100,*" .
	  " VDEF:duty_total=relay_100,AVERAGE" .
	  " 'GPRINT:duty_total:duty=%4.2lf%%'" .
	  " COMMENT:\\\\n" : "" ).
	" 'COMMENT: \\\\n'" .
	" 'COMMENT:Graph showing from " . strftime("%a %Y-%m-%d %H\\:%M\\:%S %z", $start) . "\\\\n'" .
	" 'COMMENT:                to " . strftime("%a %Y-%m-%d %H\\:%M\\:%S %z", $end) . "\\\\n'" .
	" 'COMMENT:       rendered on " . strftime("%a %Y-%m-%d %H\\:%M\\:%S %z" /*, now*/) . "\\\\n'" .
	"");

?>
