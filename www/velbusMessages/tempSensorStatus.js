function twosComplement(tc, bytes) {
	if( bytes == undefined ) { bytes = 1; }
	var thres = 1 << ((bytes*8)-1);
	if( tc < thres ) { return tc; }
	else { return thres - tc; }
}


module.exports.parse = function(msg, config, next) {

	if(!( msg.rtr == 0 && msg.data.length == 8 && msg.data[0] == 0xea )) {
		// not Temp Sensor Status, skip
		return;
	}

	msg.type = "temp sensor status";
	msg.id = msg.address.toString(16);
	if( msg.id.length == 1 ) { msg.id = '0' + msg.id; }

	msg['operating mode'] = {};
	msg['operating mode']['mode push button'] =
		( msg.data[1] & 0x01 ? 'locked' : 'unlocked' );
	switch( msg.data[1] & 0x06 ) {
		case 0x00: msg['operating mode']['schedule mode'] = 'run'; break;
		case 0x02: msg['operating mode']['schedule mode'] = 'manual'; break;
		case 0x04: msg['operating mode']['schedule mode'] = 'sleep timer'; break;
		case 0x06: msg['operating mode']['schedule mode'] = 'disable'; break;
		// no default, we listed everything
	}
	msg['operating mode']['auto send sensor temperature'] =
		( msg.data[1] & 0x08 ? 'enabled' : 'disabled' );
	switch( msg.data[1] & 0x70 ) {
		case 0x00: msg['operating mode']['temperature mode'] = 'safe'; break;
		case 0x10: msg['operating mode']['temperature mode'] = 'night'; break;
		case 0x20: msg['operating mode']['temperature mode'] = 'day'; break;
		case 0x40: msg['operating mode']['temperature mode'] = 'comfort'; break;
		default: msg['operating mode']['temperature mode'] = null; break;
	}
	msg['operating mode']['direction'] =
		( msg.data[1] & 0x80 ? 'cooler' : 'heater' );

	msg['program step'] = {};
	msg['program step']['pump unjamming'] =
		( msg.data[2] & 0x01 ? 'enabled' : 'disabled' );
	msg['program step']['heater valve unjamming'] =
		( msg.data[2] & 0x02 ? 'enabled' : 'disabled' );
	msg['program step']['sensor program'] =
		( msg.data[2] & 0x04 ? 'available' : 'not available' );
	msg['program step']['zone program'] =
		( msg.data[2] & 0x08 ? 'available' : 'not available' );
	switch( msg.data[2] & 0x70 ) {
		case 0x00: msg['program step']['received'] = 'safe'; break;
		case 0x10: msg['program step']['received'] = 'night'; break;
		case 0x20: msg['program step']['received'] = 'day'; break;
		case 0x40: msg['program step']['received'] = 'comfort'; break;
		default: msg['program step']['received'] = null; break;
	}
	msg['program step']['all program'] =
		( msg.data[2] & 0x80 ? 'available' : 'not available' );

	msg['output'] = {};
	msg['output']['heater']     = ( msg.data[3] & 0x01 ? 'on' : 'off' );
	msg['output']['boost']      = ( msg.data[3] & 0x02 ? 'on' : 'off' );
	msg['output']['day']        = ( msg.data[3] & 0x04 ? 'on' : 'off' );
	msg['output']['cooler']     = ( msg.data[3] & 0x08 ? 'on' : 'off' );
	msg['output']['valve']      = ( msg.data[3] & 0x10 ? 'on' : 'off' );
	msg['output']['low alarm']  = ( msg.data[3] & 0x20 ? 'on' : 'off' );
	msg['output']['high alarm'] = ( msg.data[3] & 0x40 ? 'on' : 'off' );

	msg['current temperature'] = twosComplement(msg.data[4]) / 2;
	msg['target temperature'] = twosComplement(msg.data[5]) / 2;

	msg['sleep timer'] = (msg.data[6] << 8) + msg.data[7];
	if( msg['sleep timer'] == 0xffff ) {
		msg['sleep timer'] = 'manual';
	}
}
