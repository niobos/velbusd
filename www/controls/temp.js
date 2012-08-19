var child_process = require('child_process');

exports.add_routes = function(webapp, velbus, config) {

function reply_to_get(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var addr_h = addr.toString(16);
	if( addr_h.length == 1 ) addr_h = '0' + addr_h;
	var field = req.params[1];
	if( field != undefined ) {
		field = field.split('/');
		field.shift(); // remove empty first element
	} else {
		field = [];
	}

	var need_cmd = {
		'temperature sensor status': 1,
		'sensor temperature': 1
	};
	if( field.length > 0 ) {
		// We might be able to optimize what we need
		if( field[0] == 'current temperature' ||
		    field[0] == 'minimum temperature' ||
		    field[0] == 'maximum temperature' ) {
			// we only need sensorTemp
			need_cmd['temperature sensor status'] = 0;
		} else {
			// we only need temperature sensor status
			need_cmd['sensor temperature'] = 0;
		}
	}

	// Set up listener for the answer
	var timeout;

	var total_msg = {};
	var send_answer = function() {
		// Are we still waiting for someone?
		var waiting = 0;
		for( var i in need_cmd ) { waiting += need_cmd[i]; }
		if( waiting > 0 ) {
			return; // And wait for the next one to call us
		}

		clearTimeout(timeout);

		delete total_msg.data;

		// Descend into requested property
		for( var i = 0; i < field.length; i++ ) {
			if( ! total_msg.hasOwnProperty( field[i] ) ) {
				res.send("Unknown property", 404);
				return;
			}
			total_msg = total_msg[ field[i] ];
		}

		if( typeof total_msg == "object" ) {
			res.send(total_msg);
		} else {
			res.send('' + total_msg, {'Content-Type': 'text/plain'} );
		}
	};

	var save_tss = function(msg) {
		need_cmd['temperature sensor status'] = 0;
		delete msg['current temperature']; // We want the precise one from sensorTemp
		for (var attr in msg) {
			total_msg[attr] = msg[attr];
		}
		send_answer();
	};

	var save_st = function(msg) {
		need_cmd['sensor temperature'] = 0;
		for (var attr in msg) {
			total_msg[attr] = msg[attr];
		}
		send_answer();
	};

	if( need_cmd['temperature sensor status'] ) {
		velbus.once('temp sensor status ' + addr_h, save_tss);
	}
	if( need_cmd['sensor temperature'] ) {
		velbus.once('sensor temperature ' + addr_h, save_st);
	}

	timeout = setTimeout(function() {
			velbus.removeListener('temp sensor status ' + addr_h, save_tss);
			velbus.removeListener('sensor temperature ' + addr_h, save_st);
			res.send("Timeout", 500);
		}, config.webapp.timeout);

	// Now send the request
	if( need_cmd['temperature sensor status'] ) {
		velbus.send_message(3, addr, 0, "\xfa\x00" );
	}
	if( need_cmd['sensor temperature'] ) {
		velbus.send_message(3, addr, 0, "\xe5\x00" ); // high precision temp
	}
}

webapp.get(/\/control\/temp\/([0-9a-fA-F]{2})((?:\/(?:[a-zA-Z]|%20)+)*)\/?$/, reply_to_get);

function dts(dt) {
	var days = [ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" ];
	var wday = days[ dt.getDay() ];

	var year = dt.getFullYear();
	var month = dt.getMonth();
	if( month < 10 ) { month = '0' + month; }
	var day = dt.getDate();
	if( day < 10 ) { day = '0' + day; }

	var hour = dt.getHours();
	if( hour < 10 ) { hour = '0' + hour; }
	var min = dt.getMinutes();
	if( min < 10 ) { min = '0' + min; }
	var sec = dt.getSeconds();
	if( sec < 10 ) { sec = '0' + sec; }

	var zonem = dt.getTimezoneOffset();
	var zones = (zonem < 0 ? '+' : '-' ); // Offsets are noted the other way around!
	zonem = Math.abs(zonem);
	var zoneh = Math.floor( zonem / 60 );
	zonem -= zoneh * 60;
	if( zoneh < 10 ) { zoneh = '0' + zoneh; }
	if( zonem < 10 ) { zonem = '0' + zonem; }

	return wday + ' ' + year + '-' + month + '-' + day + ' ' +
		hour + ':' + min + ':' + sec + ' ' + zones+zoneh+zonem;
}


webapp.get(/\/graph\/temp\/([0-9a-fA-F]{2})$/, function(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var addr_s = addr.toString(16);
	if( addr_s.length == 1 ) addr_s = '0' + addr_s;

	var name = config.controls[ addr_s ].name;
	var relay = config.controls[ addr_s ].relay;

	var width = req.query.width != undefined ? parseInt(req.query.width) : 400;
	var height = req.query.height != undefined ? parseInt(req.query.height) : 150;
	var graph_only = req.query.graph_only != undefined ? parseInt(req.query.graph_only) : 0;
	var start = req.query.start != undefined ? req.query.start : "-1 day";
	var end = req.query.end != undefined ? req.query.end : "now";
	var title = req.query.title ? req.query.title : name;
	var filename = config.graphs.directory + '/' + addr_s + '-TS.rrd';
	var filename_relay = config.graphs.directory + '/' + relay + '-RELAY.rrd';

	var rrdcmd =
		"rrdtool graph - --imgformat SVG" +
		" --width " + width + " --height " + height +
		( graph_only ? " -g" : "" ) +
		" --start " + start + " --end " + end +
		" --title '" + title + "'" +
		" --vertical-label '°C'" +
		" --alt-autoscale" +
		" 'DEF:temp=" + filename + ":temperature:AVERAGE'" +
		" 'DEF:tt=" + filename + ":set_temperature:AVERAGE'" +
		" 'DEF:heater=" + filename + ":heater:AVERAGE'" +
		( relay != undefined ? " 'DEF:relay=" + filename_relay + ":state:AVERAGE'" : "" ) +
		( relay != undefined ? " 'TICK:relay#a0a00030:1'" : "" ) +
		" TICK:heater#ff000030:1" +
		" LINE1:temp#ff0000:'Actual temperature'" +
		" VDEF:temp_avg=temp,AVERAGE" +
		" VDEF:temp_min=temp,MINIMUM" +
		" VDEF:temp_max=temp,MAXIMUM" +
		" 'GPRINT:temp_avg:avg=%4.1lf°C'" +
		" 'GPRINT:temp_min:min=%4.1lf°C'" +
		" 'GPRINT:temp_max:max=%4.1lf°C'" +
		" COMMENT:\\\\n" +
		" LINE1:tt#0000ff:'Target temperature'" +
		" VDEF:tt_avg=tt,AVERAGE" +
		" 'GPRINT:tt_avg:avg=%4.1lf°C'" +
		" COMMENT:\\\\n" +
		" CDEF:empty=UNKN,temp,POP" +
		" LINE1:empty#ffa00060:'Heat requested here '" +
		" CDEF:heater_100=heater,100,*" +
		" VDEF:duty=heater_100,AVERAGE" +
		" 'GPRINT:duty:duty=%4.2lf%%'" +
		" COMMENT:\\\\n" +
		( relay != undefined ? " LINE1:empty#a0a00030:'Heat requested total'" +
		  " CDEF:relay_100=relay,100,*" +
		  " VDEF:duty_total=relay_100,AVERAGE" +
		  " 'GPRINT:duty_total:duty=%4.2lf%%'" +
		  " COMMENT:\\\\n" : "" ) +
		" 'COMMENT: \\\\n'" +
		" 'COMMENT:Graph showing from " + dts(new Date(start*1000)).replace(/:/g, '\\:') + "\\\\n'" +
		" 'COMMENT:                to " + dts(new Date(end*1000)).replace(/:/g , '\\:') + "\\\\n'" +
		" 'COMMENT:       rendered on " + dts(new Date()).replace(/:/g, '\\:') + "\\\\n'" +
		"";


	child_process.exec(rrdcmd, {
		timeout: 1000,
		killSignal: 'SIGKILL',
		}, function(err, stdout, stderr) {
			if( err != null ) {
				res.send("Error: cmd=\n" + rrdcmd + "\nRV=" + err.code + " stderr=\n" + stderr, 500);
				return;
			}
			res.send(stdout, {'Content-type': 'image/svg+xml'});
	});

});

}

exports.add_watchers = function(velbus, state, config) {
	velbus.on('temp sensor status', function(msg) {
		state.set( msg.id + '.output.valve', msg.output.valve );
		state.set( msg.id + '.schedule_mode', msg['operating mode']['schedule mode'] );
		state.set( msg.id + '.temp_mode', msg['operating mode']['temperature mode'] );
		state.set( msg.id + '.prog_temp_mode', msg['program step']['received'] );
		state.set( msg.id + '.sleep_timer', { 'value': msg['sleep timer'], 'ref': +new Date() });

	});
	velbus.on('sensor temperature', function(msg) {
		state.set( msg.id + '.temperature', msg['current temperature']);
	});

	// And initialize the current status of all temp sensors in the config
	for(var control in config.controls) {
		if( config.controls[control].type != "temp" ) continue;

		velbus.once('connect', function(id, control) { return function() {
			// Closure with id and control

			var addr = id;

			// Spread queries in time in order not to overload the bus when starting up
			var starttime = Math.random() * Object.keys(config.controls).length * 100;
			setTimeout(function() {
				velbus.send_message(3, parseInt(addr, 16), 0, "\xfa\x00" );
				}, starttime);

		}}(control, config.controls[control]));
	}
}
