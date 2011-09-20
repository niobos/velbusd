function pad(value, length, prefix) {
	value = '' + value;
	if( prefix == undefined ) prefix = '0';
	while( value.length < length ) { value = prefix + value; }
	return value;
}

function ISO_datetimestamp() {
	var date = new Date();
	var y = date.getFullYear();
	var mo = pad( date.getMonth()+1, 2 );
	var d = pad( date.getDate(), 2 );
	var h = pad( date.getHours(), 2);
	var mi = pad( date.getMinutes(), 2);
	var s = pad( date.getSeconds(), 2);
	var zo = -date.getTimezoneOffset();
	var z = ( zo < 0 ? "-" : "+" )
	      + pad( Math.floor( Math.abs(zo) / 60 ), 2 )
	      + pad( Math.floor( Math.abs(zo) % 60 ), 2 );
	return y+"-"+mo+"-"+d+"T"+h+":"+mi+":"+s+z;
}

var vb_host = process.argv[2] || "::1"
var vb_port = process.argv[3] || 8445;

var vb = require('net').createConnection(vb_port, vb_host);
vb.on('error', function() {
	console.log(ISO_datetimestamp() + " Connect to backend ([" + vb_host + "]:" + vb_port + ") failed, exitting...");
	node.exit(1);
});
vb.on('connect', function() {
	console.log(ISO_datetimestamp() + " Connected to backend");
});

VbState = function() {};
VbState.prototype.update = function(id, state) {
	this[id] = state;
	console.log(ISO_datetimestamp() + " VB: " + id + " : " + state);
	io.sockets.emit('message', { "id": id, "state": state } );
};
var vb_state = new VbState();

var io = require('socket.io').listen(8080, null, "::").set('log level', 0);
io.sockets.on('connection', function (socket) {
	console.log(ISO_datetimestamp() + " New connection from [" + socket.handshake.address.address +"]:"+ socket.handshake.address.port);

	socket.on('disconnect', function () {
		console.log(ISO_datetimestamp() + " Disconnect from [" + socket.handshake.address.address + "]:" + socket.handshake.address.port);
	});

	for(var id in vb_state) {
		if( typeof(vb_state[id]) == 'function' ) continue;
		socket.emit('message', {"id": id, "state": vb_state[id]} );
	}
});


var vb_buffer = new Buffer( 2*(4+8+2) ); // We will never need to store more than 2 messages before we can process
var vb_buffer_len = 0;

vb.on('data', function(data) {
	// Add data to buffer
	data.copy(vb_buffer, vb_buffer_len, 0);
	vb_buffer_len += data.length;

next_message:
	while( vb_buffer_len > 6 ) { // Try to parse a message
		if( vb_buffer[0] != 0x0f ) { // Unaligned read
			console.log(ISO_datetimestamp() + " backend: unaligned read, skipping byte");
			vb_buffer.copy(vb_buffer, 0, 1, vb_buffer.length-1);
			vb_buffer_len--;
			continue;
		}

		if( (vb_buffer[1] & 0xfc) != 0xf8
		 || (vb_buffer[3] & 0xb0) != 0x00 ) { // Invalid syntax
			console.log(ISO_datetimestamp() + " backend: invalid message");
			vb_buffer.copy(vb_buffer, 0, 1, vb_buffer.length-1);
			vb_buffer_len--;
			continue;
		}

		var len = vb_buffer[3] & 0x0f;
		if( vb_buffer_len < 6+len ) break; // and wait for more data

		var msg = {};
		msg.addr = vb_buffer[2];
		msg.prio = vb_buffer[1] & 0x03;
		msg.rtr = vb_buffer[3] & 0x40;
		msg.data = new Buffer(len);
		vb_buffer.copy(msg.data, 0, 4, 4+len);

		// Remove message from buffer
		vb_buffer.copy(vb_buffer, 0, 6+len, vb_buffer.length-6-len);
		vb_buffer_len -= 6+len;

		// Parse the message
		if( msg.data[0] == 0xfb ) {
			var relay;
			switch(msg.data[1]) {
			case 0x01: relay = 0; break;
			case 0x02: relay = 1; break;
			case 0x04: relay = 2; break;
			case 0x08: relay = 3; break;
			case 0x10: relay = 4; break;
			default: continue next_message;
			}
			var id = pad(msg.addr.toString(16), 2) + "." + relay

			var state;
			switch(msg.data[3]) {
			case 0x00: state="off"; break;
			case 0x01: state="on"; break;
			case 0x03: state="interval"; break;
			default: continue next_message;
			}

			vb_state.update(id, state);
		} else if( msg.data[0] == 0xec ) {
			var blind;
			switch(msg.data[1]) {
			case 0x03: blind = 0; break;
			case 0x0c: blind = 1; break;
			default: continue next_message;
			}
			var id = pad(msg.addr.toString(16), 2) + "." + blind;

			var state;
			switch(msg.data[3]) {
			case 0x00:
				if( vb_state[id] == 'going up' ) state="up";
				else if( vb_state[id] == 'going down' ) state="down";
				else state="stopped";
				break;
			case 0x01:
			case 0x04: state="going up"; break;
			case 0x02:
			case 0x08: state="going down"; break;
			default: continue next_message;
			}

			vb_state.update(id, state);
		}
	}
});
