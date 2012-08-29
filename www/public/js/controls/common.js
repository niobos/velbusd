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
		'input.valid {' +
			'background-color: #a4ffb8;' +
		'}' +
		'input.invalid {' +
			'background-color: #ffaeb0;' +
		'}' +
	'</style>'
 ).appendTo("head");

function sec_to_pretty(sec) {
	var end = new Date();
	end.setSeconds( end.getSeconds() + sec );

	var r = '';
	if( sec >= 24*60*60 ) {
		r = end.getFullYear() + '-' + pad(end.getMonth()+1,2) + '-' + pad(end.getDate(),2) + ' ';
	}
	r = r + end.getHours() + ':' + pad(end.getMinutes(),2) + ':' + pad(end.getSeconds(),2);

	var h = Math.floor(sec / (60*60));
	sec -= h*60*60;
	var m = Math.floor(sec / 60 );
	sec -= m*60;

	var r = Math.round(sec) + 's (until ' + r + ')';
	if( m > 0 || h > 0 ) {
		r = m + 'm ' + r;
	}
	if( h > 0 ) {
		r = h + 'h ' + r;
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

function parse_duration(d) {
	/* Parse the string d into a duration in milliseconds
	 * recognized tokens:
	 *   " s sec[s] second[s]
	 *   ' m mi min[s] minute[s]
	 *   h hour[s]
	 *   d day[s]
	 *   w week[s]
	 *   mo month[s]    (30 days)
	 *   y year[s]      (365 days)
	 */
	
	var m = d.match(/\d+ ?(?:"|seconds?|secs?|s|'|minutes?|mins?|mi|m|hours?|h|days?|d|weeks?|w|months?|mo|years?|y)(?!['"a-zA-Z])/g);
	if( m == undefined ) return undefined;

	var d = 0;
	for( var e in m ) {
		var t = m[e].match(/(\d+) ?(.*)/);
		var multiply = 0;
		switch(t[2] /* units */) {
		case '"': case 's':
		case 'seconds': case 'second':
		case 'secs': case 'sec':
			multiply = 1000; break;

		case "'": case 'm': case 'mi':
		case 'mins': case 'min':
		case 'minutes': case 'minute':
			multiply = 1000 * 60; break;

		case 'h': case 'hours': case 'hour':
			multiply = 1000 * 60 * 60; break;
		case 'd': case 'days': case 'day':
			multiply = 1000 * 60 * 60 * 24; break;
		case 'w': case 'weeks': case 'week':
			multiply = 1000 * 60 * 60 * 24 * 7; break;
		case 'mo': case 'months': case 'month':
			multiply = 1000 * 60 * 60 * 24 * 30; break;
		case 'y': case 'years': case 'year':
			multiply = 1000 * 60 * 60 * 24 * 365; break;
		}
		var res = parseInt(t[1] * multiply);
		d += res;
	}

	return d;
}

function pretty_print_duration(ms) {
	/* print the given duration (in milliseconds) */
	if( ms < 0 ) return "<0";

	var sec = Math.round(ms/1000);

	var h = Math.floor(sec / (60*60));
	sec -= h*60*60;
	var mi = Math.floor(sec / 60 );
	sec -= mi*60;

	var s = Math.round(sec) + '"';
	if( mi > 0 || h > 0 ) {
		s = mi + "' " + s;
	}
	if( h > 0 ) {
		 s= h + 'h ' + s;
	}

	return s;
}

function parse_datetime(dt) {
	/* Parse the string dt into a Date() object
	 * recognized formats:
	 *  YYYY-MM-DD HH:MM:SS
	 *  HH:MM:SS    (assumed to be within the next 24 hours)
	 */

	var now = new Date();
	var m;
	if( m = dt.match(/^(\d\d\d\d)-(\d\d)-(\d\d)[ T-](\d\d)(?:[:-](\d\d))?(?:[:-](\d\d))?$/) ) {
		var y = parseInt(m[1]);
		var mo = parseInt(m[2],10);
		var d = parseInt(m[3],10);
		var h = parseInt(m[4],10);
		var mi = parseInt(m[5],10); if( isNaN(mi) ) { mi = 0; }
		var s = parseInt(m[6],10); if( isNaN(s) ) { s = 0; }
		return new Date(y, mo-1, d, h, mi, s);
	} else if( m = dt.match(/^(\d\d):(\d\d)(?::(\d\d))?$/) ) {
		var h = parseInt(m[1],10);
		var mi = parseInt(m[2],10);
		var s = parseInt(m[3],10); if( isNaN(s) ) { s = 0; }
		var time_today = new Date(now.getFullYear(), now.getMonth(), now.getDate(), h, mi, s );
		if( time_today > now ) return time_today;

		// This time today is already gone, user means tomorrow
		var tomorrow = now; tomorrow.setDate( tomorrow.getDate() + 1 );
		return new Date(tomorrow.getFullYear(), tomorrow.getMonth(), tomorrow.getDate(), h, mi, s );
	}

	return undefined;
}

function pretty_print_datetime(dt) {
	/* Print the given datetime (either milliseconds or Date object) */
	if( dt.constructor == Number ) {
		dt = new Date( dt );
	}

	var s = dt.getFullYear() + '-' + pad(dt.getMonth()+1,2) + '-' + pad(dt.getDate(),2) + ' ' +
		dt.getHours() + ':' + pad(dt.getMinutes(),2) + ':' + pad(dt.getSeconds(),2);

	return s;
}


// TODO: jQueryUI-ify this
function make_duration(jqe, options) {
/** Options is an object with properities:
  *  - enter_trigger_element: the element to trigger when enter is pressed (default: none)
  *  - enter_trigger_eventType: the eventType to trigger (default: click)
  */
	default_options = {
		enter_trigger_eventType: 'click',
	};
	if( options == undefined ) { options = {}; }
	for(var p in default_options) {
		if( ! options.hasOwnProperty(p) ) options[p] = default_options[p];
	}

	jqe.html('<div style="white-space: nowrap;">' +
				'<span class="for">for</span> ' +
				'<input type="text" name="for" size="23" placeholder="permanent"/>' +
			'</div>' +
			'<div style="white-space: nowrap;">' +
				'<span class="until">until</span> ' +
				'<input type="text" name="until" size="23" placeholder="permanent"/>' +
			'</div>');

	jqe.find('input').click(function(e) {
		e.stopPropagation(); // because that would undo the selection
	});

	jqe.find('input').focus(function(e) {
		return $(this).trigger('keyup');
	});

	var jqe_i_for = jqe.find('input[name="for"]');
	var jqe_i_until = jqe.find('input[name="until"]');
	var interval;

	jqe_i_for.keyup(function(e) {
		if( e.keyCode == 13 ) { // Enter
			if( options.enter_trigger_element != undefined ) {
				return options.enter_trigger_element.trigger(
					options.enter_trigger_eventType);
			}
			return; // anyway
		}

		clearInterval( interval );
		var v = $(this).val();
		var f = parse_duration( v );
		if( f != undefined ) {
			jqe_i_for.removeClass('invalid').addClass('valid');
			jqe_i_until.removeClass('invalid').removeClass('valid');
			var update_f = function() {
				var until = +new Date();
				until += f;
				jqe_i_until.val( pretty_print_datetime(until) );
			};
			interval = setInterval( update_f, 1000 );
			update_f();
		} else if( v == '' ) {
			jqe_i_until.val('');
			jqe_i_for.removeClass('invalid').removeClass('valid');
		} else {
			jqe_i_for.removeClass('valid').addClass('invalid');
		}
	});
	jqe_i_until.keyup(function(e) {
		if( e.keyCode == 13 ) { // Enter
			if( options.enter_trigger_element != undefined ) {
				return options.enter_trigger_element.trigger(
					options.enter_trigger_eventType);
			}
			return; // anyway
		}

		clearInterval( interval );
		var v = $(this).val();
		var u = parse_datetime( v );
		if( u != undefined ) {
			jqe_i_until.removeClass('invalid').addClass('valid');
			jqe_i_for.removeClass('invalid').removeClass('valid');
			var update_f = function() {
				var f = +new Date();
				f = u - f;
				jqe_i_for.val( pretty_print_duration(f) );
			};
			interval = setInterval( update_f, 1000 );
			update_f();
		} else if( v == '' ) {
			jqe_i_for.val('');
			jqe_i_until.removeClass('invalid').removeClass('valid');
		} else {
			jqe_i_until.removeClass('valid').addClass('invalid');
		}
	});
}
