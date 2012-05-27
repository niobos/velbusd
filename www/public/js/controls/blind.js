$('<style type="text/css">' +
		'div.icon.blind { background-image: url(images/blind-down.svg) }' +
	'</style>').appendTo("head");

$('<div class="control template blind">' +
     'Current position: <span id="blindposition">?</span><br/>' +
     '<input type="button" name="up" value="Up" /><br/>' +
     '<input type="button" name="stop" value="Stop" /><br/>' +
     '<input type="button" name="down" value="Down" />' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, id, element, coords) {
		$('#blindposition').text('?');
		$.ajax({ url: 'control/blind/' + id, dataType: 'json' })
			.success(function(data) {
				var pct = data['position']*100;
				var dir;
				switch( data['status'] ) {
				case "going up": dir = "\u2191"; break;
				case "going down": dir = "\u2193"; break;
				case "stopped": dir = ""; break;
				}
				$('#blindposition').text( pct.toFixed(0) + '% ' + dir );
			});
	});

$('#control div.blind input').click( function() {
	var id = $(this).parent().parent().find(".id").text();
	$.ajax({
		type: 'POST',
		url: 'control/blind/' + id + '/status',
		data: this.name,
		error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
	});
	return false;
});
