module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data[0] == 0x00 && msg.data.length == 4 )) {
		// not Push Button Status, skip
		return;
	}

	msg.type = "push button status";

	msg.id = msg.address.toString(16);
	if( msg.id.length == 1 ) { msg.id = '0' + msg.id; }

	msg.just_pressed  = msg.data[1];
	msg.just_released = msg.data[2];
	msg.long_pressed  = msg.data[3];
}
