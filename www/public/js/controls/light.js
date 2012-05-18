$('<style type="text/css">' +
		'div.icon.light { background-image: url(images/light-on.svg) }' +
	'</style>').appendTo("head");

$('<div class="control template light">' +
	'Current state: <span id="lightstate">?</span><br/>' +
	'<input type="button" name="on" value="On" /><br/>' +
	'<input type="button" name="off" value="Off" />' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, id, element, coords) {
		$('#lightstate').text('?');
		$.ajax({ url: 'control/relay/' + id, dataType: 'json' })
			.success(function(data) {
				var s = data['status'];
				if( data['timer'] != 0 ) {
					s += ' for ' + data['timer'] + ' secs';
				}
				$('#lightstate').text( s );
			});
	});

$('#control div.light input').click( function() {
	var id = $(this).parent().parent().find(".id").text();
	$.ajax({
		type: 'POST',
		url: 'control/relay/' + id + "/status",
		data: this.name,
		error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
	});
	return false;
});
