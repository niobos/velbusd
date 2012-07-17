$('<style type="text/css">' +
		'div.icon.blind.down      { background-image: url(images/blind-down.svg) }' +
		'div.icon.blind           { background-image: url(images/blind-half.svg) }' +
		'div.icon.blind.up        { background-image: url(images/blind-up.svg) }' +
		'div.icon.blind.goingup   { background-image: url(images/blind-up-ani.svg) }' +
		'div.icon.blind.goingdown { background-image: url(images/blind-down-ani.svg) }' +
	'</style>').appendTo("head");

var Blind = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.update();
}
$.extend(Blind.prototype, Unknown.prototype);

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
		if( this.state.position == 1 ) this.div.addClass('up');
		else if( this.state.position == 0 ) this.div.addClass('down');
		break;
	}

	if( d == "not displayed" ) return d;
}


constructors["blind"] = Blind;
