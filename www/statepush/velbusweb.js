#!/usr/bin/env node

var util = require('util');
var config = require('./config');

util.log("velbusweb starting");

process.on('SIGINT', function () {
	util.log('Caught SIGINT, quitting');
	process.exit(0);
});

var velbus = require('./velbus').ctor(config.velbusd.host, config.velbusd.port);
velbus.on('error', function(e) {
	util.log("velbus connection error: " + e.syscall + " -> " + e.code);
});
velbus.on('connect', function() {
	util.log('velbus connection opened');
});
velbus.on('close', function() {
	util.log('velbus connection closed');
});

velbus.on('resync', function(why) {
	util.log('velbus connection: lost sync: ' + why + ', resyncing');
});
velbus.on('message', function(data) {
	util.log('velbus connection: message: ' + data);
});
