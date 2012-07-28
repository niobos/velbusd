$('<style type="text/css">' +
		'div.button {' +
			'padding: 0.4em;' +
			'background-color: #a0a0a0;' +
			'border-radius: 0.3em;' +
			'cursor: pointer;' +
		'}' +
	'</style>'
 ).appendTo("head");

function sec_to_pretty(sec) {
	var end = new Date();
	end.setSeconds( end.getSeconds() + sec );

	var r = '';
	if( sec >= 24*60*60 ) {
		r = end.getFullYear() + '-' + pad(end.getMonth(),2) + '-' + pad(end.getDate(),2) + ' ';
	}
	r = r + end.getHours() + ':' + pad(end.getMinutes(),2) + ':' + pad(end.getSeconds(),2);

	var h = Math.floor(sec / (60*60));
	sec -= h*60*60;
	var m = Math.floor(sec / 60 );
	sec -= m*60;

	var r = Math.round(sec) + 's (until ' + r + ')';
	if( m > 0 || h > 0 ) {
		r = m + 'm' + r;
	}
	if( h > 0 ) {
		r = h + 'h' + r;
	}

	return r;
}

function pad(number, digits, pad) {
	if( pad == undefined ) { pad = '0'; }
	var s = '' + number;
	while( s.length < digits ) {
		s = pad + s;
	}
	return s;
}
