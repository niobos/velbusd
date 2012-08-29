var util = require('util');

exports.add_routes = function(webapp, velbus, config) {

function reply_to_get(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var input = parseInt( req.params[1] );
	var field = req.params[2];
	var addr_h = addr.toString(16);
	if( addr_h.length == 1 ) addr_h = '0' + addr_h;

	// Set up listener for the answer
	var timeout;
	var send_answer = function(msg) {
		clearTimeout(timeout);

		delete msg.data;
		var mybit = msg.input & (1 << (input-1));
		delete msg.input;
		msg.status = (mybit != 0);

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
	velbus.once('module status ' + addr_h, send_answer);
	timeout = setTimeout(function() {
			velbus.removeListener('module status ' + addr_h, send_answer);
			res.send("Timeout", 500);
		}, config.webapp.timeout);

	// Now send the request
	util.log("[" + req.connection.remoteAddress + "]:"
			+ req.connection.remotePort + " : "
			+ "Sending ModuleStatusRequest to 0x" + addr_h + " to get input status");
	velbus.send_message(3, addr, 0, "\xfa\x3f" );
}

webapp.get(/\/control\/input\/([0-9a-fA-F]{2})-([1-6])(?:\/([a-zA-Z ]*))?$/, reply_to_get);

}

exports.add_watchers = function(velbus, state, config) {
	velbus.on('module status', function(msg) {
		for( var i=0; i<8; i++) {
			state.set( msg.id + '-' + (i+1) + '.input_status', (msg.input & (1<<i)) != 0);
		}
	});
	velbus.on('push button status', function(msg) {
		for( var i=0; i<8; i++) {
			if( msg.just_pressed & (1<<i) ) {
				state.set( msg.id + '-' + (i+1) + '.input_status', true);
			}
			if( msg.just_released & (1<<i) ) {
				state.set( msg.id + '-' + (i+1) + '.input_status', false);
			}
		}
	});

	// And initialize the current status of all relays in config
	for(var control in config.controls) {
		if( config.controls[control].type != "input"
		 && config.controls[control].type != "gate" ) continue;

		velbus.once('connect', function(id, control) { return function() {
			// Closure with id and control

			var addr = id.split('-');

			// Spread queries in time in order not to overload the bus when starting up
			var starttime = Math.random() * Object.keys(config.controls).length * 100;
			setTimeout(function() { 
				util.log("startup : "
						+ "Sending ModuleStatusRequest to 0x" + addr + " to get input status");
				velbus.send_message(3, parseInt(addr[0],16), 0, "\xfa\x3f" );
				}, starttime);
		}}(control, config.controls[control]));
	}
}
