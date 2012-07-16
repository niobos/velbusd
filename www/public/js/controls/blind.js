$('<style type="text/css">' +
		'div.icon.blind { background-image: url(images/blind-down.svg) }' +
	'</style>').appendTo("head");

var Blind = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
}

$.extend(Blind.prototype, Unknown.prototype);

constructors["blind"] = Blind;
