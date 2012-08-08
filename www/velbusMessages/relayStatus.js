module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data.length == 8 && msg.data[0] == 0xfb )) {
		// not Relay Status, skip
		return;
	}

	msg.type = "relay status";
	switch(msg.data[1]) {
		case 0x01: msg.relay = 1; break;
		case 0x02: msg.relay = 2; break;
		case 0x04: msg.relay = 3; break;
		case 0x08: msg.relay = 4; break;
		case 0x10: msg.relay = 5; break;
		default: msg.relay = null; break;
	}
	msg.id = msg.address.toString(16) + '-' + msg.relay;
	if( msg.id.length == 3 ) { msg.id = '0' + msg.id; }

	switch(msg.data[2] & 0x03) {
		case 0: msg.mode = "normal"; break;
		case 1: msg.mode = "inhibited"; break;
		case 2: msg.mode = "forced on"; break;
		case 3: msg.mode = "disabled"; break;
		// no default, we listen everything
	}
	switch(msg.data[3] & 0x03) {
		case 0: msg.status = false; break;
		case 1: msg.status = true; break;
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
}
