$('<style type="text/css">' +
		'div.icon.light { background-image: url(images/light-on.svg) }' +
		'div.icon.light.off { background-image: url(images/light-off.svg) }' +
	'</style>').appendTo("head");

var Relay = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.update();
}
$.extend(Relay.prototype, Unknown.prototype);

Relay.prototype.show = function() {
	var p = Unknown.prototype.show.call(this);

	p.append('<div class="relay">Current state: <span class="state"><img src="images/loading.gif"/></span></div>');

	$.ajax({ url: 'control/relay/' + this.addr, dataType: 'json' })
		.success(function(data) {} );
}

Relay.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);
	var now = +new Date();

	switch( this.state.status ) {
	case "on":	this.div.removeClass('off'); break;
	case "off":	this.div.addClass('off'); break;
	}

	if( d == "not displayed" ) return d;

	var current_state = this.div.find('div.relay span.state');
	var s = this.state.status;
	if( this.state.timer.value != 0 ) {
		var t = this.state.timer.value;
		t -= ( now - this.state.timer.ref + my_server_time_delta ) / 1000;
		s += ' for ' + sec_to_pretty(t);
	}
	current_state.text( s );

}

constructors["light"] = Relay;
constructors["relay"] = Relay;

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
