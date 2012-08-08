function twosComplement(tc, bytes) {
	if( bytes == undefined ) { bytes = 1; }
	var thres = 1 << ((bytes*8)-1);
	if( tc < thres ) { return tc; }
	else { return thres - tc; }
}


module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data.length == 7 && msg.data[0] == 0xe6 )) {
		// not Temp Sensor Status, skip
		return;
	}

	msg.type = "sensor temperature";
	msg.id = msg.address.toString(16);
	if( msg.id.length == 1 ) msg.id = '0' + msg.id;

	msg['current temperature'] = twosComplement( (msg.data[1]<<8) + msg.data[2], 2 ) / 512;
	msg['minimum temperature'] = twosComplement( (msg.data[3]<<8) + msg.data[4], 2 ) / 512;
	msg['maximum temperature'] = twosComplement( (msg.data[5]<<8) + msg.data[6], 2 ) / 512;
}
