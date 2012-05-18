var child_process = require('child_process');

exports.add_routes = function(webapp, velbus, config) {

function reply_to_get(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var field = req.params[1];
	if( field != undefined ) {
		field = field.split('/');
		field.shift(); // remove empty first element
	} else {
		field = [];
	}

	var need_cmd = {
		'temperature sensor status': 1,
		'sensor temperature': 1
	};
	if( field.length > 0 ) {
		// We might be able to optimize what we need
		if( field[0] == 'current temperature' ||
		    field[0] == 'minimum temperature' ||
		    field[0] == 'maximum temperature' ) {
			// we only need sensorTemp
			need_cmd['temperature sensor status'] = 0;
		} else {
			// we only need temperature sensor status
			need_cmd['sensor temperature'] = 0;
		}
	}

	// Set up listener for the answer
	var timeout;

	var total_msg = {};
	var send_answer = function() {
		// Are we still waiting for someone?
		var waiting = 0;
		for( var i in need_cmd ) { waiting += need_cmd[i]; }
		if( waiting > 0 ) {
			return; // And wait for the next one to call us
		}

		clearTimeout(timeout);

		delete total_msg.data;

		// Descend into requested property
		for( var i = 0; i < field.length; i++ ) {
			if( ! total_msg.hasOwnProperty( field[i] ) ) {
				res.send("Unknown property", 404);
				return;
			}
			total_msg = total_msg[ field[i] ];
		}

		if( typeof total_msg == "object" ) {
			res.send(total_msg);
		} else {
			res.send('' + total_msg, {'Content-Type': 'text/plain'} );
		}
	};

	var save_tss = function(msg) {
		need_cmd['temperature sensor status'] = 0;
		delete msg['current temperature']; // We want the precise one from sensorTemp
		for (var attr in msg) {
			total_msg[attr] = msg[attr];
		}
		send_answer();
	};

	var save_st = function(msg) {
		need_cmd['sensor temperature'] = 0;
		for (var attr in msg) {
			total_msg[attr] = msg[attr];
		}
		send_answer();
	};

	if( need_cmd['temperature sensor status'] ) {
		velbus.once('temp sensor status ' + addr, save_tss);
	}
	if( need_cmd['sensor temperature'] ) {
		velbus.once('sensor temperature ' + addr, save_st);
	}

	timeout = setTimeout(function() {
			velbus.removeListener('temp sensor status ' + addr, save_tss);
			velbus.removeListener('sensor temperature ' + addr, save_st);
			res.send("Timeout", 500);
		}, config.webapp.timeout);

	// Now send the request
	if( need_cmd['temperature sensor status'] ) {
		velbus.send_message(3, addr, 0, "\xfa\x00" );
	}
	if( need_cmd['sensor temperature'] ) {
		velbus.send_message(3, addr, 0, "\xe5\x00" ); // high precision temp
	}
}

webapp.get(/\/control\/temp\/([0-9a-fA-F]{2})((?:\/(?:[a-zA-Z]|%20)+)*)\/?$/, reply_to_get);

function strtotime (str, now) {
    // http://kevin.vanzonneveld.net
    // +   original by: Caio Ariede (http://caioariede.com)
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: David
    // +   improved by: Caio Ariede (http://caioariede.com)
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Wagner B. Soares
    // +   bugfixed by: Artur Tchernychev
    // %        note 1: Examples all have a fixed timestamp to prevent tests to fail because of variable time(zones)
    // *     example 1: strtotime('+1 day', 1129633200);
    // *     returns 1: 1129719600
    // *     example 2: strtotime('+1 week 2 days 4 hours 2 seconds', 1129633200);
    // *     returns 2: 1130425202
    // *     example 3: strtotime('last month', 1129633200);
    // *     returns 3: 1127041200
    // *     example 4: strtotime('2009-05-04 08:30:00');
    // *     returns 4: 1241418600
    var i, l, match, s, parse = '';

    str = str.replace(/\s{2,}|^\s|\s$/g, ' '); // unecessary spaces
    str = str.replace(/[\t\r\n]/g, ''); // unecessary chars
    if (str === 'now') {
        return now === null || isNaN(now) ? new Date().getTime() / 1000 | 0 : now | 0;
    } else if (!isNaN(parse = Date.parse(str))) {
        return parse / 1000 | 0;
    } else if (now) {
        now = new Date(now * 1000); // Accept PHP-style seconds
    } else {
        now = new Date();
    }

    str = str.toLowerCase();

    var __is = {
        day: {
            'sun': 0,
            'mon': 1,
            'tue': 2,
            'wed': 3,
            'thu': 4,
            'fri': 5,
            'sat': 6
        },
        mon: [
            'jan',
            'feb',
            'mar',
            'apr',
            'may',
            'jun',
            'jul',
            'aug',
            'sep',
            'oct',
            'nov',
            'dec'
        ]
    };

    var process = function (m) {
        var ago = (m[2] && m[2] === 'ago');
        var num = (num = m[0] === 'last' ? -1 : 1) * (ago ? -1 : 1);

        switch (m[0]) {
        case 'last':
        case 'next':
            switch (m[1].substring(0, 3)) {
            case 'yea':
                now.setFullYear(now.getFullYear() + num);
                break;
            case 'wee':
                now.setDate(now.getDate() + (num * 7));
                break;
            case 'day':
                now.setDate(now.getDate() + num);
                break;
            case 'hou':
                now.setHours(now.getHours() + num);
                break;
            case 'min':
                now.setMinutes(now.getMinutes() + num);
                break;
            case 'sec':
                now.setSeconds(now.getSeconds() + num);
                break;
            case 'mon':
                if (m[1] === "month") {
                    now.setMonth(now.getMonth() + num);
                    break;
                }
                // fall through
            default:
                var day = __is.day[m[1].substring(0, 3)];
                if (typeof day !== 'undefined') {
                    var diff = day - now.getDay();
                    if (diff === 0) {
                        diff = 7 * num;
                    } else if (diff > 0) {
                        if (m[0] === 'last') {
                            diff -= 7;
                        }
                    } else {
                        if (m[0] === 'next') {
                            diff += 7;
                        }
                    }
                    now.setDate(now.getDate() + diff);
                    now.setHours(0, 0, 0, 0); // when jumping to a specific last/previous day of week, PHP sets the time to 00:00:00
                }
            }
            break;

        default:
            if (/\d+/.test(m[0])) {
                num *= parseInt(m[0], 10);

                switch (m[1].substring(0, 3)) {
                case 'yea':
                    now.setFullYear(now.getFullYear() + num);
                    break;
                case 'mon':
                    now.setMonth(now.getMonth() + num);
                    break;
                case 'wee':
                    now.setDate(now.getDate() + (num * 7));
                    break;
                case 'day':
                    now.setDate(now.getDate() + num);
                    break;
                case 'hou':
                    now.setHours(now.getHours() + num);
                    break;
                case 'min':
                    now.setMinutes(now.getMinutes() + num);
                    break;
                case 'sec':
                    now.setSeconds(now.getSeconds() + num);
                    break;
                }
            } else {
                return false;
            }
            break;
        }
        return true;
    };

    match = str.match(/^(\d{2,4}-\d{2}-\d{2})(?:\s(\d{1,2}:\d{2}(:\d{2})?)?(?:\.(\d+))?)?$/);
    if (match !== null) {
        if (!match[2]) {
            match[2] = '00:00:00';
        } else if (!match[3]) {
            match[2] += ':00';
        }

        s = match[1].split(/-/g);

        s[1] = __is.mon[s[1] - 1] || s[1];
        s[0] = +s[0];

        s[0] = (s[0] >= 0 && s[0] <= 69) ? '20' + (s[0] < 10 ? '0' + s[0] : s[0] + '') : (s[0] >= 70 && s[0] <= 99) ? '19' + s[0] : s[0] + '';
        return parseInt(this.strtotime(s[2] + ' ' + s[1] + ' ' + s[0] + ' ' + match[2]) + (match[4] ? match[4] / 1000 : ''), 10);
    }

    var regex = '([+-]?\\d+\\s' + '(years?|months?|weeks?|days?|hours?|min|minutes?|sec|seconds?' + '|sun\\.?|sunday|mon\\.?|monday|tue\\.?|tuesday|wed\\.?|wednesday' + '|thu\\.?|thursday|fri\\.?|friday|sat\\.?|saturday)' + '|(last|next)\\s' + '(years?|months?|weeks?|days?|hours?|min|minutes?|sec|seconds?' + '|sun\\.?|sunday|mon\\.?|monday|tue\\.?|tuesday|wed\\.?|wednesday' + '|thu\\.?|thursday|fri\\.?|friday|sat\\.?|saturday))' + '(\\sago)?';

    match = str.match(new RegExp(regex, 'gi')); // Brett: seems should be case insensitive per docs, so added 'i'
    if (match === null) {
        return false;
    }

    for (i = 0, l = match.length; i < l; i++) {
        if (!process(match[i].split(' '))) {
            return false;
        }
    }

    return now.getTime() / 1000 | 0;
}

function dts(dt) {
	var days = [ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" ];
	var wday = days[ dt.getDay() ];

	var year = dt.getFullYear();
	var month = dt.getMonth();
	if( month < 10 ) { month = '0' + month; }
	var day = dt.getDate();
	if( day < 10 ) { day = '0' + day; }

	var hour = dt.getHours();
	if( hour < 10 ) { hour = '0' + hour; }
	var min = dt.getMinutes();
	if( min < 10 ) { min = '0' + min; }
	var sec = dt.getSeconds();
	if( sec < 10 ) { sec = '0' + sec; }

	var zonem = dt.getTimezoneOffset();
	var zones = (zonem < 0 ? '+' : '-' ); // Offsets are noted the other way around!
	zonem = Math.abs(zonem);
	var zoneh = Math.floor( zonem / 60 );
	zonem -= zoneh * 60;
	if( zoneh < 10 ) { zoneh = '0' + zoneh; }
	if( zonem < 10 ) { zonem = '0' + zonem; }

	return wday + ' ' + year + '-' + month + '-' + day + ' ' +
		hour + ':' + min + ':' + sec + ' ' + zones+zoneh+zonem;
}


webapp.get(/\/graph\/temp\/([0-9a-fA-F]{2})$/, function(req, res, next) {
	var addr = parseInt( req.params[0], 16 );
	var addr_s = addr.toString(16);
	if( addr_s.length == 1 ) addr_s = '0' + addr_s;

	var name = config.controls[ addr_s ].name;
	var relay = config.controls[ addr_s ].relay;

	var width = req.query.width != undefined ? parseInt(req.query.width) : 200;
	var height = req.query.height != undefined ? parseInt(req.query.height) : 100;
	var graph_only = req.query.graph_only != undefined ? parseInt(req.query.graph_only) : 1;
	var start = strtotime( req.query.start != undefined ? req.query.start : "-1 day" );
	var end = strtotime( req.query.end != undefined ? req.query.end : "now" );
	var title = req.query.title ? req.query.title : name;
	var filename = config.graphs.directory + '/' + addr_s + '-TS.rrd';
	var filename_relay = config.graphs.directory + '/' + relay + '-RELAY.rrd';

	var rrdcmd =
		"rrdtool graph - --imgformat SVG" +
		" --width " + width + " --height " + height +
		( graph_only ? " -g" : "" ) +
		" --start " + start + " --end " + end +
		" --title '" + title + "'" +
		" --vertical-label '°C'" +
		" --alt-autoscale" +
		" 'DEF:temp=" + filename + ":temperature:AVERAGE'" +
		" 'DEF:tt=" + filename + ":set_temperature:AVERAGE'" +
		" 'DEF:heater=" + filename + ":heater:AVERAGE'" +
		( relay != undefined ? " 'DEF:relay=" + filename_relay + ":state:AVERAGE'" : "" ) +
		( relay != undefined ? " 'TICK:relay#a0a00030:1'" : "" ) +
		" TICK:heater#ff000030:1" +
		" LINE1:temp#ff0000:'Actual temperature'" +
		" VDEF:temp_avg=temp,AVERAGE" +
		" VDEF:temp_min=temp,MINIMUM" +
		" VDEF:temp_max=temp,MAXIMUM" +
		" 'GPRINT:temp_avg:avg=%4.1lf°C'" +
		" 'GPRINT:temp_min:min=%4.1lf°C'" +
		" 'GPRINT:temp_max:max=%4.1lf°C'" +
		" COMMENT:\\\\n" +
		" LINE1:tt#0000ff:'Target temperature'" +
		" VDEF:tt_avg=tt,AVERAGE" +
		" 'GPRINT:tt_avg:avg=%4.1lf°C'" +
		" COMMENT:\\\\n" +
		" CDEF:empty=UNKN,temp,POP" +
		" LINE1:empty#ffa00060:'Heat requested here '" +
		" CDEF:heater_100=heater,100,*" +
		" VDEF:duty=heater_100,AVERAGE" +
		" 'GPRINT:duty:duty=%4.2lf%%'" +
		" COMMENT:\\\\n" +
		( relay != undefined ? " LINE1:empty#a0a00030:'Heat requested total'" +
		  " CDEF:relay_100=relay,100,*" +
		  " VDEF:duty_total=relay_100,AVERAGE" +
		  " 'GPRINT:duty_total:duty=%4.2lf%%'" +
		  " COMMENT:\\\\n" : "" ) +
		" 'COMMENT: \\\\n'" +
		" 'COMMENT:Graph showing from " + dts(new Date(start*1000)).replace(/:/g, '\\:') + "\\\\n'" +
		" 'COMMENT:                to " + dts(new Date(end*1000)).replace(/:/g , '\\:') + "\\\\n'" +
		" 'COMMENT:       rendered on " + dts(new Date()).replace(/:/g, '\\:') + "\\\\n'" +
		"";


	child_process.exec(rrdcmd, {
		timeout: 1000,
		killSignal: 'SIGKILL',
		}, function(err, stdout, stderr) {
			if( err != null ) {
				res.send("Error: cmd=\n" + rrdcmd + "\nRV=" + err.code + " stderr=\n" + stderr, 500);
				return;
			}
			res.send(stdout, {'Content-type': 'image/svg+xml'});
	});

});

}
