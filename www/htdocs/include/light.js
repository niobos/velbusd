$('<style type="text/css">' +
		'div.icon.light { background-image: url(images/light-on) }' +
	'</style>').appendTo("head");

$('<div class="control template light">' +
	'<input type="button" name="on" value="On" /><br/>' +
	'<input type="button" name="off" value="Off" />' +
	'</div>'
 ).appendTo("#control");

$('#control div.light input').click( function() {
	var id = $(this).parent().parent().find(".id").text();
	$.ajax({
		type: 'POST',
		url: 'api/RelayStatus.php/' + id,
		data: 'state=' + this.name,
		error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
	});
	return false;
});
