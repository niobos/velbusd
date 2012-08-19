$('<style type="text/css">' +
		'div.icon.temp         { background-image: url(images/temp-blue.svg) }' +
		'div.icon.temp.heating { background-image: url(images/temp-red.svg) }' +
	'</style>').appendTo("head");

var Temp = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.interval = {
	};
	this.update();
}
$.extend(Temp.prototype, Unknown.prototype);

Temp.prototype.show = function() {
	var p = Unknown.prototype.show.call(this);

	$.ajax({ url: 'control/temp/' + this.addr, dataType: 'json' });
	// Result is automatically processed via WebSocket

	if( p == undefined ) return;

	p.append('<div class="temp">' +
			'<div style="padding-bottom: 0.2em; white-space: nowrap;">' +
				'Current temperature: <span class="temp">' +
					'<img src="images/loading.gif"/></span>' +
				' (<a href="graph/temp.html?id=' + this.addr + '">' +
					'history</a>)</div>' +
			'<div style="padding-bottom: 0.2em; white-space: nowrap;">' +
				'Current state: <span class="state">' +
					'<img src="images/loading.gif"/></span></div>' +
		'</div>');
};

Temp.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);
	var now = +new Date();

	switch( this.state.output.valve ) {
	case "on":	this.div.addClass('heating'); break;
	case "off":	this.div.removeClass('heating'); break;
	}

	if( d == "not displayed" ) return d;

	this.div.find('div.temp span.temp').text( this.state.temperature + 'ºC' );

	var that = this;
	var update_state = function() {
		var reason = '?';
		if( that.state['schedule_mode'] == "run" ) {
			if( that.state['prog_temp_mode'] == that.state['temp_mode'] ) {
				reason = "schedule";
			} else {
				reason = "override"; // Until next program step
			}
		} else if( that.state['schedule_mode'] == "sleep timer" ) {
			var t = that.state['sleep_timer'].value * 60;
			t -= ( now-my_server_time_delta - that.state['sleep_timer'].ref ) / 1000;
			reason = "override for " + sec_to_pretty(t);
		} else if( that.state['schedule_mode'] == "manual" ) {
			reason = "manual";
		}
		that.div.find('div.temp span.state').text( that.state['temp_mode'] +
				' (' + that.state['target_temp'] + 'ºC), ' +
				reason);
	};
	clearInterval( this.interval.state ); this.interval.state = undefined;
	if( this.state['schedule_mode'] == "sleep timer" ) {
		this.interval.state = setInterval(update_state, 1000);
	}
	update_state();
}

constructors["temp"] = Temp;
