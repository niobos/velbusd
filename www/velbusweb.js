#!/usr/bin/env node

var util = require('util');
var config = require('./config');
var fs = require('fs');

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

var messageParsers = fs.readdirSync('./velbusMessages');
for( var i = 0; i < messageParsers.length; i++ ) {
	messageParsers[i] = require('./velbusMessages/' + messageParsers[i] );
}
velbus.on('message', function(msg) {
	//util.log('velbus connection: message: ' + util.inspect(msg) );
	for( var i = 0; i < messageParsers.length; i++ ) {
		messageParsers[i].parse( msg, config, null );
	}
	//util.log('velbus connection: message (after processing): ' + util.inspect(msg) );

	if( msg.type != null ) {
		velbus.emit( msg.type + ' ' + msg.id, msg );
	}
});


var express = require('express');
var webapp = express.createServer();

webapp.configure(function() {
	webapp.use( express.logger() );
	webapp.use( express.bodyParser() );
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

webapp.listen(config.webapp.port, config.webapp.bind);
util.log("webserver listening on [" +
	config.webapp.bind + ']:' + config.webapp.port +
	" in " + webapp.settings.env + " mode");

webapp.get('/js/controls.js', function(req, res, next) {
	// Generate the concatenated JS file of the content
	fs.readdir('public/js/controls', function(err, files) {
		if( err !== null ) { next(err); }
		var body = '';
		for( var f in files ) {
			var fn = files[f];
			// Read all files
			fs.readFile('public/js/controls/' + fn, function(err, data) {
				if( err !== null ) { next(err); }
				// Append content to the body
				body += data + "\n";
				// Remove us from the todo list
				files.splice( files.indexOf(fn), 1 );

				if( files.length == 0 ) {
					// We are the last one to finish, send output
					res.send(body, {'Content-Type': 'application/javascript'} );
				}
			});
		}
	});
});


webapp.get('/data/coords.json', function(req, res, next) {
	res.json(config.controls);
});

var controls = fs.readdirSync('./controls');
for( var i = 0; i < controls.length; i++ ) {
	require('./controls/' + controls[i]).add_routes(webapp, velbus, config);
}