$('<style type="text/css">' +
		'div.icon.dimmer { background-image: url(images/light-on.svg) }' +
		'div.icon.dimmer.off { background-image: url(images/light-off.svg) }' +
	'</style>').appendTo("head");

var Dimmer = function(addr, state, coord) {
	Unknown.call(this, addr, state, coord);
	this.update();
}
$.extend(Dimmer.prototype, Unknown.prototype);

Dimmer.prototype.show = function() {
	var p = Unknown.prototype.show.call(this);

	$.ajax({ url: 'control/dimmer/' + this.addr, dataType: 'json' })
		.success(function(data) {} );

	if( p == undefined ) return;

	p.append('<div class="dimmer">' +
			'<div style="padding-bottom: 0.2em; white-space: nowrap;">Current state: ' +
				'<span class="state"><img src="images/loading.gif"/></span></div>' +
			'</div>');
}

Dimmer.prototype.update = function(attr) {
	var d = Unknown.prototype.update.call(this, attr);

	if( this.state.dimvalue == 0 ) {
		this.div.addClass('off').removeClass("on");
	} else {
		this.div.removeClass('off').addClass("on");
	}

	if( d == "not displayed" ) return d;

	this.div.find('div.dimmer span.state').text( this.state.dimvalue + '%' );
}

constructors["dimmer"] = Dimmer;
