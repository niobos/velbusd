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

Blind.prototype.show = function() {
	var p = Unknown.prototype.show.call(this);
	if( p == undefined ) return;

	p.append('<div class="blind">' +
			'<div style="padding-bottom: 0.2em;">Current state: ' +
				'<span class="state"><img src="images/loading.gif"/></span></div>' +
			'<div style="display: table;">' +
				'<div class="button up">UP</div>' +
				'<div class="button stop" style="display: none;">STOP</div>' +
				'<div class="button down">DOWN</div>' +
			'</div>' +
			'</div>');

	$.ajax({ url: 'control/blind/' + this.addr, dataType: 'json' })
		.success(function(data) {} );
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
		if( this.state.position == 1 ) this.div.addClass('up');
		else if( this.state.position == 0 ) this.div.addClass('down');
		break;
	}

	if( d == "not displayed" ) return d;

	this.div.find('div.blind span.state').text( this.state.status );

	switch( this.state.status ) {
	case "going down":
	case "going up":
		this.div.find('div.blind div.button.up').css('display', 'none');
		this.div.find('div.blind div.button.stop').css('display', 'block');
		this.div.find('div.blind div.button.down').css('display', 'none');
		break;

	case "stopped":
		this.div.find('div.blind div.button.up').css('display', 'block');
		this.div.find('div.blind div.button.stop').css('display', 'none');
		this.div.find('div.blind div.button.down').css('display', 'block');
		break;
	}

}


constructors["blind"] = Blind;
