#!/usr/bin/env node

var fs = require('fs'),
    util = require('util');

var input = fs.readFileSync('/dev/stdin').toString();

var j = JSON.parse(input);

function dump_object (obj, path) {
	var prefix = path.join(' / ');
	if( prefix != '' ) { prefix += ' / '; }
	for( var attr in obj ) {
		if( typeof obj[attr] == "object" ) {
			var path2 = path.slice(0);
			path2.push(attr);
			dump_object( obj[attr], path2 );
		} else {
			console.log( prefix + attr + " = " + obj[attr] );
		}
	}
}
dump_object(j, []);
