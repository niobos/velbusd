$('<style type="text/css">' +
		'div.icon.blind.down      { background-image: url(images/blind-down.svg) }' +
		'div.icon.blind           { background-image: url(images/blind-half.svg) }' +
		'div.icon.blind.up        { background-image: url(images/blind-up.svg) }' +
		'div.icon.blind.goingup   { background-image: url(images/blind-up-ani.svg) }' +
		'div.icon.blind.goingdown { background-image: url(images/blind-down-ani.svg) }' +
	'</style>').appendTo("head");

var Blind = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.interval = {state: undefined};
	this.update();
}
$.extend(Blind.prototype, Unknown.prototype);

Blind.prototype.show = function() {
	var p = Unknown.prototype.show.call(this);

	$.ajax({ url: 'control/blind/' + this.addr, dataType: 'json' })
		.success(function(data) {} );

	if( p == undefined ) return;

	p.append('<div class="blind">' +
			'<div style="padding-bottom: 0.4em; white-space: nowrap;">Currently ' +
				'<span class="state"><img src="images/loading.gif"/></span> open</div>' +
			'<div style="">' +
				'<div class="button up" style="display: inline;">&#x2191;</div>' +
				'<div class="button stop" style="display: none;">&#x275A;&#x275A;</div>' +
				'<div class="button down" style="display: inline;">&#x2193;</div>' +
			'</div>' +
			'</div>');

	that = this;
	p.find('div.blind div.button').click(function() {
		var el = $(this);
		var data = {};
		if( el.hasClass('up') )   data['up'] = '';
		if( el.hasClass('down') ) data['down'] = '';
		if( el.hasClass('stop') ) data['stop'] = '';
		$.post('control/blind/' + that.addr + '/status', data );
		return false; // don't update
	});
}

Blind.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	this.div.removeClass('down')
	        .removeClass('up')
	        .removeClass('goingup')
	        .removeClass('goingdown');
	switch( this.state.status ) {
	case "going down":	this.div.addClass('goingdown'); break;
	case "going up":  	this.div.addClass('goingup'); break;
	case "stopped":
		if( this.state.position.value == 1 ) this.div.addClass('up');
		else if( this.state.position.value == 0 ) this.div.addClass('down');
		break;
	}

	if( d == "not displayed" ) return d;

	var that = this;
	var update_state = function() {
		var pos = that.state.position.value;

		var time_since_pos = +new Date();
		time_since_pos -= my_server_time_delta;
		time_since_pos -= that.state.position.ref;

		var moved_since = time_since_pos/1000 * 1/that.state.time;

		if( that.state.status == "going up" ) {
			pos += moved_since;
			pos = Math.min(1,pos);
		}
		if( that.state.status == "going down" ) {
			pos -= moved_since;
			pos = Math.max(0,pos);
		}

		var state = (pos * 100).toFixed(0) + '% ';
		switch( that.state.status ) {
		case "going up": state += "\u2191"; break;
		case "going down": state += "\u2193"; break;
		case "stopped": state += ""; break;
		}
		that.div.find('div.blind span.state').text(state);
	};
	clearInterval( this.interval.state ); this.interval.state = undefined;
	if( this.state.status != "stopped" ) {
		this.interval.state = setInterval(update_state, 500);
	}
	update_state();

	switch( this.state.status ) {
	case "going down":
	case "going up":
		this.div.find('div.blind div.button.up').css('display', 'none');
		this.div.find('div.blind div.button.stop').css('display', 'inline');
		this.div.find('div.blind div.button.down').css('display', 'none');
		break;

	case "stopped":
		this.div.find('div.blind div.button.up').css('display', 'inline');
		this.div.find('div.blind div.button.stop').css('display', 'none');
		this.div.find('div.blind div.button.down').css('display', 'inline');
		break;
	}

}


constructors["blind"] = Blind;
