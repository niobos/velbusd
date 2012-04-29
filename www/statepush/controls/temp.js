var util = require('util');

exports.add_routes = function(webapp, velbus, config) {

function reply_to_get(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
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
		velbus.once('temp sensor status ' + addr, save_tss);
	}
	if( need_cmd['sensor temperature'] ) {
		velbus.once('sensor temperature ' + addr, save_st);
	}

	timeout = setTimeout(function() {
			velbus.removeListener('temp sensor status ' + addr, save_tss);
			velbus.removeListener('sensor temperature ' + addr, save_st);
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

}
