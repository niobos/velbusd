var blindStatus = {};

module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data.length == 8 && msg.data[0] == 0xec )) {
		// not Blind Status, skip
		return;
	}

	msg.type = "blind status";
	switch(msg.data[1]) {
		case 0x03: msg.blind = 1; break;
		case 0x0c: msg.blind = 2; break;
		default: msg.blind = null; break;
	}
	switch(msg.data[2]) {
		case 0x00: msg.timeout = 15; break;
		case 0x01: msg.timeout = 30; break;
		case 0x02: msg.timeout = 60; break;
		case 0x03: msg.timeout = 120; break;
		default: msg.timeout = null; break;
	}
	switch( ( msg.data[3] >> (msg.blind-1)*2 ) & 0x03 ) {
		case 0x00: msg.status = "stopped"; break;
		case 0x01: msg.status = "going up"; break;
		case 0x02: msg.status = "going down"; break;
		default: msg.status = null; break;
	}
	switch(msg.data[4]) {
		case 0x00: msg.led = "off"; break;
		case 0x80: msg.led = "down on"; break;
		case 0x40: msg.led = "down slow blinking"; break;
		case 0x20: msg.led = "down fast blinking"; break;
		case 0x10: msg.led = "down very fast blinking"; break;
		case 0x08: msg.led = "up on"; break;
		case 0x04: msg.led = "up slow blinking"; break;
		case 0x02: msg.led = "up fast blinking"; break;
		case 0x01: msg.led = "up very fast blinking"; break;
		default: msg.led = null; break;
	}
	msg.timer = (msg.data[5] << 16) + (msg.data[6] << 8) + msg.data[7];

	// Calculate augmented info
	var id = msg.address.toString(16) + '.' + msg.blind;
	if( id.length == 3 ) { id = '0' + id; }

	if( blindStatus[id] == undefined ) {
		blindStatus[id] = { 'public': { 'position': 0.5 } };
	}

	{
		var travel = config.controls[id].time;
		if( travel == undefined ) { travel = msg.timeout - 5; } // Default
		var now = +new Date();

		// Have we been moving since the last time we checked?
		if( blindStatus[id].since != undefined ) {
			var traveled = (now - blindStatus[id].since) / 1000 / travel;
			blindStatus[id].public.position += traveled *
				( blindStatus[id].public['last move'] == "up" ? 1 : -1 );
			// And clamp to [0,1]
			blindStatus[id].public.position = Math.min(1, Math.max(0,
				blindStatus[id].public.position ));
			blindStatus[id].since = now;
		}

		if( msg.status == "stopped" ) {
			blindStatus[id].since = undefined;
		} else {
			blindStatus[id].since = now;
		}

	}

	// Augment with calculated info
	for( var attr in blindStatus[id].public ) {
		msg[attr] = blindStatus[id].public[attr];
	}

	// Update last move AFTER it has been copied into msg
	switch( msg.status ) {
		case "going up": blindStatus[id].public['last move'] = "up"; break;
		case "going down": blindStatus[id].public['last move'] = "down"; break;
	}

}
