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


var io = require('socket.io').listen(8080, null, "::").set('log level', 0);
io.sockets.on('connection', function (socket) {
	console.log(ISO_datetimestamp() + " New connection from [" + socket.handshake.address.address +"]:"+ socket.handshake.address.port);

	socket.on('disconnect', function () {
		console.log(ISO_datetimestamp() + " Disconnect from [" + socket.handshake.address.address + "]:" + socket.handshake.address.port);
	});
});


var vb_buffer = new Buffer( 2*(4+8+2) ); // We will never need to store more than 2 messages before we can process
var vb_buffer_len = 0;

vb.on('data', function(data) {
	// Add data to buffer
	data.copy(vb_buffer, vb_buffer_len, 0);
	vb_buffer_len += data.length;

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

		io.sockets.emit('message', msg);
	}
});
