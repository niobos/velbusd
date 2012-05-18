
exports.add_routes = function(webapp, velbus, config) {

function reply_to_get(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var relay = parseInt( req.params[1] );
	var field = req.params[2];

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
	velbus.once('relay status ' + addr + '.' + relay, send_answer);
	timeout = setTimeout(function() {
			velbus.removeListener('relay status ' + addr + '.' + relay, send_answer);
			res.send("Timeout", 500);
		}, config.webapp.timeout);

	// Now send the request
	var relaybit = String.fromCharCode( 1 << (relay-1) );
	velbus.send_message(3, addr, 0, "\xfa" + relaybit );
}

webapp.get(/\/control\/relay\/([0-9a-fA-F]{2}).([1-4])(?:\/([a-zA-Z ]*))?$/, reply_to_get);

webapp.post(/\/control\/relay\/([0-9a-fA-F]{2}).([1-4])\/([a-zA-Z ]*)$/, function(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var relay = parseInt( req.params[1] );
	var field = req.params[2];

	var relaybit = 1 << (relay-1);

	// We expect a single value in the body
	var value = Object.keys( req.body )
	if( value.length != 1 ) {
		res.send("Expecting single value in POST body", 400);
		return;
	}
	value = value[0];

	switch( field ) {
	case "status":
		var command;
		switch( value ) {
		case "on":
			command = new Buffer([ 0x02, relaybit ] );
			break;

		case "off":
			command = new Buffer([ 0x01, relaybit ]);
			break;

		default:
			res.send("Unknown status", 400);
			return;
		}
		velbus.send_message(0, addr, 0, command);
		break;

	default:
		res.send("Not implemented", 501);
		return;
	}

	// And fall through to the GET response
	reply_to_get(req, res, next);
});

}
