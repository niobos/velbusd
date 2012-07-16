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
	state.set('00.state', 'online');
});
velbus.on('close', function() {
	util.log('velbus connection closed');
	state.set('00.state', 'offline');
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

var state = require('./state.js').ctor();

// Copy in config.controls
function traverse(obj, func, stack) {
	if( stack == null ) { stack = []; }
	for(var prop in obj) {
		var newstack = stack.slice();
		newstack.push(prop);
		if( Array.isArray( obj[prop] ) ) {
			func.apply(this, [ newstack, obj[prop] ]);
		} else if( typeof( obj[prop] ) == "object" ) {
			traverse( obj[prop], func, newstack );
		} else {
			func.apply(this, [ newstack, obj[prop] ]);
		}
	}
}
traverse( config.controls, function(prop, value) {
	var propname = prop.join('.');
	state.set(propname, value);
});

var sockjs = require('sockjs');
var sockjs_stream = sockjs.createServer( {
	sockjs_url: "/js/sockjs.js"
});
sockjs_stream.installHandlers(webapp, { prefix:'/events' });

sockjs_stream.on('connection', function(conn) {
	// Start by sending the full object
	conn.write( state.dump() );

	var fonupdate = function(data) { // Assign function to a variable, so we can remove it later
		// Closure over conn object
		conn.write(data);
	};
	state.on('update', fonupdate);
	conn.on('close', function() {
		state.removeListener('update', fonupdate);
	});
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


webapp.get('/state.json', function(req, res, next) {
	res.json(state.properities);
});

var controls = fs.readdirSync('./controls');
for( var i = 0; i < controls.length; i++ ) {
	require('./controls/' + controls[i]).add_routes(webapp, velbus, config);
}

