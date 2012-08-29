$('<style type="text/css">' +
		'div.icon.temp         { background-image: url(images/temp-blue.svg) }' +
		'div.icon.temp.heating { background-image: url(images/temp-red.svg) }' +
		'div.temp div.button   { display: table-cell; vertical-align: middle; }' +
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
			'<div style="display: table;">' +
				'<div class="button comfort">' +
						'<img src="images/temp-comfort.svg"/></div>' +
				'<div class="button day">' +
						'<img src="images/temp-day.svg"/></div>' +
				'<div class="button night">' +
						'<img src="images/temp-night.svg"/></div>' +
				'<div class="button safe">' +
						'<img src="images/temp-safe.svg"/></div>' +
				'<div class="duration" style="margin-left: 0.5em;"></div>' +
				'<div style="clear: both;"></div>' +
			'</div>' +
		'</div>');

	var that = this;
	p.find("div.temp div.button").click(function() {
			var data = {};
			var dur = that.div.find('div.temp input[name="for"]').val();
			dur = parse_duration(dur);
			if( dur != undefined ) {
				dur = dur/1000/60; // temp works in minutes
			} else {
				dur = ''; // until next step
			}
			if( $(this).hasClass("comfort") ) {
				data["comfort"] = dur;
			} else if( $(this).hasClass("day") ) {
				data["day"] = dur;
			} else if( $(this).hasClass("night") ) {
				data["night"] = dur;
			} else if( $(this).hasClass("safe") ) {
				data["safe"] = dur;
			} else {
				console.log("Got click on button, but couldn't figure out which one");
				console.log(this);
				return false;
			}
			$.post('control/temp/' + that.addr + '/operating%20mode/temperature%20mode', data );
			return false; // don't update
		});

	make_duration( p.find('div.temp div.duration'), {
			placeholder_for: 'this step duration',
			placeholder_until: 'next step received',
		});
};

Temp.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	switch( this.state.output.valve ) {
	case "on":	this.div.addClass('heating'); break;
	case "off":	this.div.removeClass('heating'); break;
	}

	if( d == "not displayed" ) return d;

	this.div.find('div.temp span.temp').text( this.state.temperature + 'ºC' );

	var that = this;
	var update_state = function() {
		var now = +new Date();
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
