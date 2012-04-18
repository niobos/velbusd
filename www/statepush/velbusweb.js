#!/usr/bin/env node

var util = require('util');
var net = require('net');
var config = require('./config');

util.log("velbusweb starting");

process.on('SIGINT', function () {
	util.log('Caught SIGINT, quitting');
	process.exit(0);
});

var bus = net.connect(config.velbusd.port, config.velbusd.host, function() {
	util.log("velbus connection open");
});

bus.on('error', function (e) {
	util.log("velbus connection error: " + e.syscall + " -> " + e.code);
});

bus.on('close', function() {
	util.log("velbus connection closed");
	setTimeout(function(){
			bus.connect(config.velbusd.port, config.velbusd.host);
		}, config.velbusd.reconnect_after_ms);
});

var unprocessed_data = new Buffer(4096);
var n_unprocessed_data = 0;
bus.on('data', function(data) {
	data.copy(unprocessed_data, n_unprocessed_data);
	n_unprocessed_data += data.length;
	process_data();
});

function process_data() {
	while( n_unprocessed_data > 6 ) {
		process_message();
	}
}
function resync(where) {
	util.log("Lost sync (" + where + "), resyncing");
	unprocessed_data.copy(unprocessed_data, 0, 1); n_unprocessed_data--;
	while( n_unprocessed_data > 0 && unprocessed_data[0] != 0x0f ) {
		unprocessed_data.copy(unprocessed_data, 0, 1); n_unprocessed_data--;
	}
}
function process_message() {
	if( unprocessed_data[0] != 0x0f ) { resync("STX " + unprocessed_data[0]); return; }
	if( (unprocessed_data[1] & 0xfc) != 0xf8 ) { resync("STX1 " + unprocessed_data[1]); return; }
	var priority = unprocessed_data[1] & 0x03;
	var address = unprocessed_data[2];
	var rtr = (unprocessed_data[3] & 0x40) >> 6;
	var length = unprocessed_data[3] & 0x0f;
	if( n_unprocessed_data < 6+length ) { return; }
	var data = new Buffer(length);
	unprocessed_data.copy(data, 0, 4, 4+length);
	var checksum = unprocessed_data[4+length];
	if( unprocessed_data[4+length+1] != 0x04 ) { resync("ETX"); return; }

	var sum = 0;
	for( var i = 0; i < 4+length; i++ ) {
		sum += unprocessed_data[i];
	}
	sum = (-sum)&0xff;
	if( checksum != sum ) { resync("Checksum calc("+sum+")!=data("+checksum+")"); }

	unprocessed_data.copy( unprocessed_data, 0, 6+length );
	n_unprocessed_data -= 6+length;

	util.log("Rx data: Prio=" + priority +
			" Addr=0x" + address +
			" RTR=" + rtr +
			" data[" + length + "]=" + hexdump(data)
			);
}

function hexdump(data) {
	var h = '';
	for( var i = 0; i < data.length; i++ ) {
		h += data[i].toString(16) + ' ';
	}
	return h;
}
