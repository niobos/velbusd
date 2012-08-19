$('<style type="text/css">' +
		'div.icon.light { background-image: url(images/light-on.svg) }' +
		'div.icon.light.off { background-image: url(images/light-off.svg) }' +

		'div.icon.fan.on { background-image: url(images/fan-ani.svg) }' +
		'div.icon.fan { background-image: url(images/fan.svg) }' +
	'</style>').appendTo("head");

var Relay = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.interval = {
		state: undefined,
		for_until: undefined
	};
	this.update();
}
$.extend(Relay.prototype, Unknown.prototype);

Relay.prototype.show = function() {
	var p = Unknown.prototype.show.call(this);

	$.ajax({ url: 'control/relay/' + this.addr, dataType: 'json' })
		.success(function(data) {} );

	if( p == undefined ) return;

	p.append('<div class="relay">' +
			'<div style="padding-bottom: 0.2em; white-space: nowrap;">Current state: ' +
				'<span class="state"><img src="images/loading.gif"/></span></div>' +
			'<div style="display: table;">' +
				'<div class="button" ' +
					'style="display: table-cell; vertical-align: middle;">' +
						'<img src="images/loading.gif"/></div>' +
				'<div class="duration" style="margin-left: 0.5em;">' +
					'<div style="white-space: nowrap;">for <input type="text" name="for" size="23" placeholder="permanent"/></div>' +
					'<div style="white-space: nowrap;">until <input type="text" name="until" size="23" placeholder="permanent"/></div>' +
				'</div>' +
				'<div style="clear: both;"></div>' +
			'</div>' +
		'</div>');

	var that = this;
	p.find('div.relay div.button').click(function() {
			var data = {};
			if( that.state.status ) {
				data['off'] = '';
			} else {
				data['on'] = '';
				var dur = that.div.find('div.relay input[name="for"]').val();
				dur = parse_duration(dur);
				if( dur != undefined ) data['on'] = dur/1000;
			}
			$.post('control/relay/' + that.addr + '/status', data );
		});

	p.find('div.relay input').click(function(e) {
		e.stopPropagation(); // because that would undo the selection
	});

	p.find('div.relay input').focus(function(e) {
		return $(this).trigger('keyup');
	});
	p.find('div.relay input[name="for"]').keyup(function(e) {
			if( e.keyCode == 13 ) { // Enter
				return p.find('div.relay div.button').trigger('click');
			}

			clearInterval( that.interval.for_until );
			var v = $(this).val();
			var until_input = p.find('div.relay input[name="until"]');
			var f = parse_duration( v );
			if( f != undefined ) {
				$(this).removeClass('invalid').addClass('valid');
				until_input.removeClass('invalid').removeClass('valid');
				var update_f = function() {
					var until = +new Date();
					until += f;
					until_input.val( pretty_print_datetime(until) );
				};
				that.interval.for_until = setInterval( update_f, 1000 );
				update_f();
			} else if( v == '' ) {
				until_input.val('');
				$(this).removeClass('invalid').removeClass('valid');
			} else {
				$(this).removeClass('valid').addClass('invalid');
			}
		});
	p.find('div.relay input[name="until"]').keyup(function(e) {
			if( e.keyCode == 13 ) { // Enter
				return p.find('div.relay div.button').trigger('click');
			}

			clearInterval( that.interval.for_until );
			var v = $(this).val();
			var for_input = p.find('div.relay input[name="for"]');
			var u = parse_datetime( v );
			if( u != undefined ) {
				$(this).removeClass('invalid').addClass('valid');
				for_input.removeClass('invalid').removeClass('valid');
				var update_f = function() {
					var f = +new Date();
					f = u - f;
					for_input.val( pretty_print_duration(f) );
				};
				that.interval.for_until = setInterval( update_f, 1000 );
				update_f();
			} else if( v == '' ) {
				for_input.val('');
				$(this).removeClass('invalid').removeClass('valid');
			} else {
				$(this).removeClass('valid').addClass('invalid');
			}
		});
}

Relay.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	switch( this.state.status ) {
	case true:	this.div.removeClass('off').addClass("on"); break;
	case false:	this.div.addClass('off').removeClass("on"); break;
	}

	if( d == "not displayed" ) return d;

	var that = this;
	var update_state = function() {
		var now = +new Date();
		var current_state = that.div.find('div.relay span.state');
		var s = ( that.state.status ? "on" : "off" );
		if( that.state.timer.value != 0 ) {
			var t = that.state.timer.value;
			t -= ( now-my_server_time_delta - that.state.timer.ref ) / 1000;
			s += ' for ' + sec_to_pretty(t);
		}
		current_state.text( s );
	};
	clearInterval( this.interval.state ); this.interval.state = undefined;
	if( this.state.timer.value != 0 ) {
		this.interval.state = setInterval(update_state, 1000);
	}
	update_state();

	this.div.find('div.relay div.button').text(
			(this.state.status ? "OFF" : "ON" )
		);
	this.div.find('div.relay div.duration').css('display',
			( this.state.status ? 'none' : 'block' ));

}

constructors["relay"] = Relay;
