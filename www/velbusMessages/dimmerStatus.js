var util = require('util');

module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data.length == 8 && msg.data[0] == 0xee )) {
		// not Dimmer Status, skip
		return;
	}

	msg.type = "dimmer status";

	msg.id = msg.address.toString(16);
	if( msg.id.length == 1 ) { msg.id = '0' + msg.id; }

	switch(msg.data[1]) {
		case 0: msg.mode = "start/stop timer"; break;
		case 1: msg.mode = "staircase timer"; break;
		case 2: msg.mode = "dimmer"; break;
		case 3: msg.mode = "dimmer with memory"; break;
		case 4: msg.mode = "multi step dimmer"; break;
		case 5: msg.mode = "slow on dimmer"; break;
		case 6: msg.mode = "slow off dimmer"; break;
		case 7: msg.mode = "slow on/off dimmer"; break;
	}

	msg.dimvalue = msg.data[2];

	switch( msg.data[3] ) {
		case 0x00: msg.led = "off"; break;
		case 0x80: msg.led = "on"; break;
		case 0x40: msg.led = "slow blinking"; break;
		case 0x20: msg.led = "fast blinking"; break;
		case 0x10: msg.led = "very fast blinking"; break;
	}
	
	msg.delay = (msg.data[4] << 16) + (msg.data[5] << 8) + (msg.data[6]);
	msg.config = msg.data[7];
}
