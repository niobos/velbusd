$('<style type="text/css">' +
		'div.icon.temp { background-image: url(images/temp.svg) }' +
	'</style>').appendTo("head");

var Temp = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
}

$.extend(Temp.prototype, Unknown.prototype);

constructors["temp"] = Temp;
