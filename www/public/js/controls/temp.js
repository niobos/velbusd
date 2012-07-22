$('<style type="text/css">' +
		'div.icon.temp         { background-image: url(images/temp-blue.svg) }' +
		'div.icon.temp.heating { background-image: url(images/temp-red.svg) }' +
	'</style>').appendTo("head");

var Temp = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.update();
}
$.extend(Temp.prototype, Unknown.prototype);

Temp.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	switch( this.state.heater ) {
	case "on":	this.div.addClass('heating'); break;
	case "off":	this.div.removeClass('heating'); break;
	}

	if( d == "not displayed" ) return d;
}

constructors["temp"] = Temp;
