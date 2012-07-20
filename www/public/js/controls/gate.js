$('<style type="text/css">' +
		'div.icon.gate        { background-image: url(images/gate.svg) }' +
		'div.icon.gate.open   { background-image: url(images/gate-open.svg) }' +
		'div.icon.gate.closed { background-image: url(images/gate-closed.svg) }' +
	'</style>').appendTo("head");

var Gate = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.update();
}
$.extend(Gate.prototype, Unknown.prototype);

Gate.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	this.div.removeClass('open')
	        .removeClass('closed')
	switch( this.state.status ) {
	case true:  this.div.addClass('closed'); break;
	case false: this.div.addClass('open'); break;
	}

	if( d == "not displayed" ) return d;
}


constructors["gate"] = Gate;
