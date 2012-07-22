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
	var current_state = p.find('span.state');

	$.ajax({ url: 'control/relay/' + this.addr, dataType: 'json' })
		.success(function(data) {
			var s = data['status'];
			if( data['timer'] != 0 ) {
				s += ' for ' + sec_to_hms(data['timer']);
			}
			current_state.text( s );
		});
}

Relay.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	switch( this.state.status ) {
	case "on":	this.div.removeClass('off'); break;
	case "off":	this.div.addClass('off'); break;
	}

	if( d == "not displayed" ) return d;
}

constructors["light"] = Relay;
constructors["relay"] = Relay;
