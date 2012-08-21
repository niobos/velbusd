var events = require('events'),
    util = require('util');

var state = function() {
	var that = this;
	events.EventEmitter.call(this); // Inherit EventEmitter

	this.properities = {};

	return this;
}
util.inherits(state, events.EventEmitter);

exports.ctor = function() {
	return new state();
}

state.prototype.get = function(key) {
	var p = this.properities;

	var k = key.split(".");
	for(var e in k ) {
		if( p[ k[e] ] === undefined ) {
			return undefined;
		}
		p = p[ k[e] ];
	}

	return p;
}

state.prototype.set = function(key, value) {
	//console.log("state: " + key + " <= " + util.inspect(value));
	var p = this.properities;

	var k = key.split(".");
	for(var i=0; i<k.length-1; i++ ) {
		if( p[ k[i] ] === undefined ) {
			p[ k[i] ] = {};
		}
		p = p[ k[i] ];
	}
	var oldvalue = p[ k[ k.length-1 ] ];
	p[ k[ k.length-1 ] ] = value;

	this.emit('update', JSON.stringify({
		'timestamp': +new Date(),
		'changetype': 'update',
		'key': key,
		'value': value
		}) );

	return oldvalue;
}

state.prototype.dump = function() {
	return JSON.stringify( {
			'timestamp': +new Date(),
			'changetype': 'new',
			'value': this.properities
		});
};
