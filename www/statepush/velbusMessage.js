var events = require('events'),
    util = require('util');

module.exports = new events.EventEmitter;

module.exports.parse = function(msg) {

	// Relay Status
	if( msg.rtr == 0 && msg.data.length == 8 && msg.data[0] == 0xfb ) {
		msg.type = "relay status";
		switch(msg.data[1]) {
			case 0x01: msg.relay = 1; break;
			case 0x02: msg.relay = 2; break;
			case 0x04: msg.relay = 3; break;
			case 0x08: msg.relay = 4; break;
			case 0x10: msg.relay = 5; break;
			default: msg.relay = null; break;
		}
		switch(msg.data[2] & 0x03) {
			case 0: msg.mode = "normal"; break;
			case 1: msg.mode = "inhibited"; break;
			case 2: msg.mode = "forced on"; break;
			case 3: msg.mode = "disabled"; break;
			// no default, we listen everything
		}
		switch(msg.data[3] & 0x03) {
			case 0: msg.status = "off"; break;
			case 1: msg.status = "on"; break;
			case 3: msg.status = "interval"; break;
			default: msg.status = null; break;
		}
		switch(msg.data[4]) {
			case 0x00: msg.led = "off"; break;
			case 0x80: msg.led = "on"; break;
			case 0x40: msg.led = "slow blinking"; break;
			case 0x20: msg.led = "fast blinking"; break;
			case 0x10: msg.led = "very fast blinking"; break;
			default: msg.led = null; break;
		}
		msg.timer = (msg.data[5] << 16) + (msg.data[6] << 8) + msg.data[7];

		delete msg.data;
		delete msg.rtr;

		// Emit the different levels
		this.emit('relay status', msg);
		this.emit('relay status ' + msg.address, msg);
		this.emit('relay status ' + msg.address + '.' + msg.relay, msg);

	// Blind Status
	} else if( msg.rtr == 0 && msg.data.length == 8 && msg.data[0] == 0xec ) {
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

		delete msg.data;
		delete msg.rtr;

		this.emit('blind status', msg);
		this.emit('blind status ' + msg.address, msg);
		this.emit('blind status ' + msg.address + '.' + msg.blind, msg);
	}



}
