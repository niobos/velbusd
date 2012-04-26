var events = require('events'),
    util = require('util'),
    net = require('net');

var velbus = function(host, port) {
	var that = this;
	events.EventEmitter.call(this); // Inherit EventEmitter
	that.host = host;
	that.port = port;
	that.retry_after_ms = 1000;

	that.socket = net.connect(that.port, that.host, function() {
		that.emit('connect');
	});

	that.socket.on('error', function (e) {
		that.emit('error', e);
	});

	that.socket.on('close', function() {
		that.emit('close');
		setTimeout(function(){
				that.socket.connect(that.host, that.port);
			}, that.retry_after_ms);
	});

	that.unprocessed_data = new Buffer(4096);
	that.n_unprocessed_data = 0;

	that.socket.on('data', function(data) {
		data.copy(that.unprocessed_data, that.n_unprocessed_data);
		that.n_unprocessed_data += data.length;
		that.process_data();
	});

	return this;
}
util.inherits(velbus, events.EventEmitter);

velbus.prototype.process_data = function() {
	while( this.n_unprocessed_data > 6 ) {
		this.process_message();
	}
}

velbus.prototype.process_message = function() {
	if( this.unprocessed_data[0] != 0x0f ) { this.resync("STX"); return; }
	if( (this.unprocessed_data[1] & 0xfc) != 0xf8 ) { this.resync("STX1"); return; }
	var priority = this.unprocessed_data[1] & 0x03;
	var address = this.unprocessed_data[2];
	var rtr = (this.unprocessed_data[3] & 0x40) >> 6;
	var length = this.unprocessed_data[3] & 0x0f;
	if( this.n_unprocessed_data < 6+length ) { return; }
	var data = new Buffer(length);
	this.unprocessed_data.copy(data, 0, 4, 4+length);
	var checksum = this.unprocessed_data[4+length];
	if( this.unprocessed_data[4+length+1] != 0x04 ) { this.resync("ETX"); return; }

	var sum = 0;
	for( var i = 0; i < 4+length; i++ ) {
		sum += this.unprocessed_data[i];
	}
	sum = (-sum)&0xff;
	if( checksum != sum ) { this.resync("Checksum calc("+sum+")!=data("+checksum+")"); }

	this.unprocessed_data.copy( this.unprocessed_data, 0, 6+length );
	this.n_unprocessed_data -= 6+length;

	this.emit('message', {
			"prio": priority,
			"address": address,
			"rtr": rtr,
			"data": data
			});
}

velbus.prototype.resync = function(where) {
	this.emit('resync', where);
	this.unprocessed_data.copy(this.unprocessed_data, 0, 1); this.n_unprocessed_data--;
	while( this.n_unprocessed_data > 0 && this.unprocessed_data[0] != 0x0f ) {
		this.unprocessed_data.copy(this.unprocessed_data, 0, 1); this.n_unprocessed_data--;
	}
}

exports.ctor = function(host, port) {
	return new velbus(host, port);
}

exports.create_message = function (prio, addr, rtr, data) {
	var length = data.length;
	var msg = new Buffer( 4 + length + 2 );
	msg[0] = 0x0f;
	msg[1] = 0xf8 | (prio & 0x03);
	msg[2] = addr & 0xff;
	msg[3] = ( rtr ? 0x40 : 0x00 ) | (length & 0x0f);
	if( typeof data == "string" ) { msg.write(data, 4, 'binary'); }
	else if( data instanceof Buffer ) { data.copy(msg, 4); }
	else { return null; }

	var sum = 0;
	for( var i = 0; i < 4+length; i++ ) {
		sum += msg[i];
	}
	sum = (-sum)&0xff;

	msg[ 4+length ] = sum;
	msg[ 4+length+1 ] = 0x04;

	return msg;
}

velbus.prototype.send_message = function (prio, addr, rtr, data) {
	var msg = exports.create_message( prio, addr, rtr, data );
	if( msg == null ) { return null; }

	this.socket.write( msg );
}
