$('<style type="text/css">' +
		'div.button {' +
			'padding: 0.4em;' +
			'margin: 0.2em;' +
			'background-color: #a0a0a0;' +
			'border-radius: 0.3em;' +
			'border: 1px solid rgb(153, 153, 153);' +
			'text-shadow: 0px 0px 5px rgb(255, 255, 255);' +
			'cursor: pointer;' +
			'/* Drop shadow */' +
			'box-shadow: 1px 1px 2px rgba(0,0,0,0.7);' +
			'-moz-box-shadow: 1px 1px 2px rgba(0,0,0,0.7);' +
			'-webkit-box-shadow: 1px 1px 2px rgba(0,0,0,0.7);' +
			'/* For IE 8 */' +
			'-ms-filter: "progid:DXImageTransform.Microsoft.Shadow(Strength=2, Direction=135, Color=\'#000000\')";' +
			'/* For IE 5.5 - 7 */' +
			'filter: progid:DXImageTransform.Microsoft.Shadow(Strength=2, Direction=135, Color=\'#000000\');' +
			'/* Gradient */' +
			'background-image:linear-gradient(-90deg, rgb(238, 238, 238), rgb(204, 204, 204));' +
			'background-image:-webkit-gradient(linear, 50% 0%, 50% 100%, from(rgb(238, 238, 238)), to(rgb(204, 204, 204)));' +
			'background-image:-moz-linear-gradient(-90deg, rgb(238, 238, 238), rgb(204, 204, 204));' +
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
