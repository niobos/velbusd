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
	//util.log('velbus connection: message: ' + util.inspect(data) );
});


var express = require('express');
var webapp = express.createServer();

webapp.configure(function() {
	webapp.use( express.logger() );
	//webapp.use( express.bodyParser() );
	//webapp.use( express.cookieParser(...) );
	//webapp.use( express.session(...) );
	webapp.use( webapp.router );
	webapp.use( express.static(__dirname + '/public') );
});
webapp.configure('development', function() {
	webapp.use( express.errorHandler({ dumpExceptions: true, showStack: true }) );
});
webapp.configure('production', function() {
	webapp.use(express.errorHandler() );
});

webapp.listen(config.webapp.port);
util.log("webserver listening on " +
		webapp.address().address + ':' + webapp.address().port +
		" in " + webapp.settings.env + " mode");
