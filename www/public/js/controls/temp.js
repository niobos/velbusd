$('<style type="text/css">' +
		'div.icon.temp { background-image: url(images/temp.svg) }' +
	'</style>').appendTo("head");

$('<div class="control template temp">' +
		'Current: <span id="tempcur">?</span>&deg;C<br/>' +
		'Mode: <span id="tempmode">?</span> (<span id="temptarget">?</span>&deg;C), ' +
		'<span id="tempreason">why am I in this mode?</span><br/>' +
		'<img id="tempgraph" src=""/>' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, id, element, coords) {
	$("#tempgraph").attr('src', 'graph/temp/' + id + '?' +
		'width=150&height=70&graph_only=1' +
		'&refresh=' + Math.floor((+new Date())/1000/60) // Refresh max once every minute
		);

	$("#tempcur").text('?');
	$("#tempmode").text('?');
	$("#temptarget").text('?');
	$("#tempreason").text('?');
	$.ajax({ url: 'control/temp/' + id, dataType: 'json' })
		.success(function(data) {
				$("#tempcur").text(data['current temperature']);
				$("#tempmode").text(data['operating mode']['temperature mode']);
				$("#temptarget").text(data['target temperature']);
				var reason = '';
				if( data['operating mode']['schedule mode'] == "run" ) {
					if( data['program step']['received'] == data['operating mode']['temperature mode'] ) {
						reason = "schedule";
					} else {
						reason = "override"; // Until next program step
					}
				} else if( data['operating mode']['schedule mode'] == "sleep timer" ) {
					reason = "override for " + min_to_hm( data['sleep timer'] );
				} else if( data['operating mode']['schedule mode'] == "manual" ) {
					reason = "manual";
				} else {
					reason = "?";
				}
				$("#tempreason").text(reason);
			});
});

function pad(number, digits, pad) {
	if( pad == undefined ) { pad = '0'; }
	var s = '' + number;
	while( s.length < digits ) {
		s = pad + s;
	}
	return s;
}

function min_to_hm(min) {
	var end = new Date();
	end.setMinutes( end.getMinutes() + min );

	var r = '';
	if( min >= 24*60 ) {
		r = end.getFullYear() + '-' + pad(end.getMonth(),2) + '-' + pad(end.getDate(),2) + ' ';
	}
	r = r + end.getHours() + ':' + pad(end.getMinutes(),2);

	var h = Math.floor(min / 60);
	var m = min - h*60;

	var r = m + 'm (until ' + r + ')';
	if( h > 0 ) {
		r = h + 'h' + r;
	}

	return r;
}
