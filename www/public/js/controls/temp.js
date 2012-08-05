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
				'Current state: <span class="state">' +
					'<img src="images/loading.gif"/></span>' +
				' (<a href="graph/temp.html?id=' + this.addr + '">' +
					'history</a>)</div>' +
		'</div>');
};

Temp.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	switch( this.state.output.heater ) {
	case "on":	this.div.addClass('heating'); break;
	case "off":	this.div.removeClass('heating'); break;
	}

	if( d == "not displayed" ) return d;

	this.div.find('div.temp span.state').text( this.state.temperature + 'ºC' );
}

constructors["temp"] = Temp;
