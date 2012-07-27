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

	p.append('<div class="relay">' +
			'<div style="padding-bottom: 0.2em;">Current state: ' +
				'<span class="state"><img src="images/loading.gif"/></span></div>' +
			'<div style="display: table;">' +
				'<div class="switchbutton" ' +
					'style="display: table-cell; vertical-align: middle;' +
						'padding: 0.4em; background-color: #a0a0a0;' +
						'border-radius: 0.3em; cursor: pointer;">ON</div>' +
				'<div style="margin-left: 0.5em;">' +
					//'<div>for NNN</div>' +
					//'<div>until XXX</div>' +
				'</div>' +
				'<div style="clear: both;"></div>' +
			'</div>' +
		'</div>');

	$.ajax({ url: 'control/relay/' + this.addr, dataType: 'json' })
		.success(function(data) {} );

	var that = this;
	p.find('div.relay div.switchbutton').click(function() {
			var data = {};
			if( that.state.status ) {
				data['off'] = '';
			} else {
				data['on'] = '';
			}
			$.post('control/relay/' + that.addr + '/status', data );
		});
}

Relay.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);
	var now = +new Date();

	switch( this.state.status ) {
	case true:	this.div.removeClass('off'); break;
	case false:	this.div.addClass('off'); break;
	}

	if( d == "not displayed" ) return d;

	var current_state = this.div.find('div.relay span.state');
	var s = ( this.state.status ? "on" : "off" );
	if( this.state.timer.value != 0 ) {
		var t = this.state.timer.value;
		t -= ( now - this.state.timer.ref + my_server_time_delta ) / 1000;
		s += ' for ' + sec_to_pretty(t);
	}
	current_state.text( s );

	this.div.find('div.relay div.switchbutton').text(
			(this.state.status ? "OFF" : "ON" )
		);

}

constructors["light"] = Relay;
constructors["relay"] = Relay;
