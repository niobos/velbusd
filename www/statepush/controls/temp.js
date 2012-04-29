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

	// TODO: optimize based on field: we don't always need both requests

	// Set up listener for the answer
	var timeout;

	var total_msg = {};
	var send_answer = function() {
		clearTimeout(timeout);

		delete total_msg.data;

		for( var i = 0; i < field.length; i++ ) {
			if( ! total_msg.hasOwnProperty( field[i] ) ) {
				res.send("Unknown property", 404);
				return;
			}
			total_msg = total_msg[ field[i] ];
		}

		if( typeof total_msg == "string" ) {
			res.send(total_msg, {'Content-Type': 'text/plain'} );
		} else {
			res.send(total_msg);
		}
	};

	var waiting_for = { 'tss': 1, 'st': 1 };

	var save_tss = function(msg) {
		waiting_for['tss'] = 0;
		delete msg['current temperature']; // We want the precise one from sensorTemp
		for (var attr in msg) {
			total_msg[attr] = msg[attr];
		}
		if( waiting_for['st'] == 0 ) {
			// We're the last one
			send_answer();
		}
	};

	var save_st = function(msg) {
		waiting_for['st'] = 0;
		for (var attr in msg) {
			total_msg[attr] = msg[attr];
		}
		if( waiting_for['tss'] == 0 ) {
			// We're the last one
			send_answer();
		}
	};

	velbus.once('temp sensor status ' + addr, save_tss);
	velbus.once('sensor temperature ' + addr, save_st);

	timeout = setTimeout(function() {
			velbus.removeListener('temp sensor status ' + addr, save_tss);
			velbus.removeListener('sensor temperature ' + addr, save_st);
			res.send("Timeout", 500);
		}, config.webapp.timeout);

	// Now send the request
	velbus.send_message(3, addr, 0, "\xfa\x00" ); // status
	velbus.send_message(3, addr, 0, "\xe5\x00" ); // high precision temp
}

webapp.get(/\/control\/temp\/([0-9a-fA-F]{2})((?:\/(?:[a-zA-Z]|%20)+)*)\/?$/, reply_to_get);

}
