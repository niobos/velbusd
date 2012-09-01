var util = require('util');

exports.add_routes = function(webapp, velbus, config) {

function reply_to_get(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var field = req.params[1];
	var addr_h = addr.toString(16);
	if( addr_h.length == 1 ) addr_h = '0' + addr_h;

	// Set up listener for the answer
	var timeout;
	var send_answer = function(msg) {
		clearTimeout(timeout);

		delete msg.data;

		if( field != undefined && field != '' ) {
			if( msg.hasOwnProperty(field) ) {
				res.send( msg[field].toString(), {'Content-Type': 'text/plain'} );
			} else {
				res.send("Unknown property", 404);
			}
		} else {
			res.send(msg);
		}
	};
	velbus.once('dimmer status ' + addr_h, send_answer);
	timeout = setTimeout(function() {
			velbus.removeListener('dimmer status ' + addr_h, send_answer);
			res.send("Timeout", 500);
		}, config.webapp.timeout);

	// Now send the request
	util.log("[" + req.connection.remoteAddress + "]:"
			+ req.connection.remotePort + " : "
			+ "Sending ModuleStatusRequest to 0x" + addr_h + " to get dimmer status");
	velbus.send_message(3, addr, 0, "\xfa\x01" );
}

webapp.get(/\/control\/dimmer\/([0-9a-fA-F]{2})(?:\/([a-zA-Z ]*))?$/, reply_to_get);

}

exports.add_watchers = function(velbus, state, config) {
	velbus.on('dimmer status', function(msg) {
		state.set( msg.id + '.dimvalue', msg.dimvalue );
	});

	// And initialize the current status of all relays in config
	for(var control in config.controls) {
		if( config.controls[control].type != "dimmer" ) continue;

		velbus.once('connect', function(id, control) { return function() {
			// Closure with id and control

			// Spread queries in time in order not to overload the bus when starting up
			var starttime = Math.random() * Object.keys(config.controls).length * 100;
			setTimeout(function() {
				util.log("startup : "
						+ "Sending ModuleStatusRequest to 0x" + id + " to get dimmer status");
				velbus.send_message(3, parseInt(id,16), 0, "\xfa\x01" );
				}, starttime);
		}}(control, config.controls[control]));
	}
}
