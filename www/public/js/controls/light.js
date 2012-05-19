$('<style type="text/css">' +
		'div.icon.light { background-image: url(images/light-on.svg) }' +
	'</style>').appendTo("head");
$('<style type="text/css">' +
		'div.icon.relay { background-image: url(images/light-off.svg) }' +
	'</style>').appendTo("head");

$('<div class="control template light relay">' +
	'Current state: <span id="lightstate">?</span><br/>' +
	'<input type="button" name="on" value="On" /><br/>' +
	'<input type="button" name="onfor" value="On for" />' +
		'<input type="text" id="onfor" size="5" value="30" /><br/>' +
	'<input type="button" name="off" value="Off" />' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, id, element, coords) {
		$('#lightstate').text('?');
		$.ajax({ url: 'control/relay/' + id, dataType: 'json' })
			.success(function(data) {
				var s = data['status'];
				if( data['timer'] != 0 ) {
					s += ' for ' + sec_to_hms(data['timer']);
				}
				$('#lightstate').text( s );
			});
	});
$('#onfor').click( function() { return false; } ); // just enter text field, do nothing more

$('#control div.light input[type="button"]').click( function() {
	var id = $(this).parent().parent().find(".id").text();
	var data;
	switch( this.name ) {
	case "onfor": data = "on=" + $("#onfor").val(); break;
	default: data = this.name; break;
	}
	$.ajax({
		type: 'POST',
		url: 'control/relay/' + id + "/status",
		data: data,
		error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
	});
	return false;
});

function sec_to_hms(sec) {
	var end = new Date();
	end.setSeconds( end.getSeconds() + sec );

	var r = '';
	if( sec >= 24*60*60 ) {
		r = end.getFullYear() + '-' + pad(end.getMonth(),2) + '-' + pad(end.getDate(),2) + ' ';
	}
	r = r + end.getHours() + ':' + pad(end.getMinutes(),2) + ':' + pad(end.getSeconds(),2);

	var h = Math.floor(sec / (60*60));
	sec -= h*60*60;
	var m = Math.floor(sec / 60 );
	sec -= m*60;

	var r = sec + 's (until ' + r + ')';
	if( m > 0 || h > 0 ) {
		r = m + 'm' + r;
	}
	if( h > 0 ) {
		r = h + 'h' + r;
	}

	return r;
}
