module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data[0] == 0xed && 
	       (msg.data.length == 5 || msg.data.length == 7) )) {
		// not Input Status, skip
		return;
	}

	msg.type = "module status";

	msg.id = msg.address.toString(16);
	if( msg.id.length == 1 ) { msg.id = '0' + msg.id; }

	msg.status = msg.data[1];
}
