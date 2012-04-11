$('<style type="text/css">' +
		'div.icon.blind { background-image: url(images/blind) }' +
	'</style>').appendTo("head");

$('<div class="control template blind">' +
     '<input type="button" name="up" value="Up" /><br/>' +
     '<input type="button" name="stop" value="Stop" /><br/>' +
     '<input type="button" name="down" value="Down" />' +
	'</div>'
 ).appendTo("#control");

$('#control div.blind input').click( function() {
	var id = $(this).parent().parent().find(".id").text();
	$.ajax({
		type: 'POST',
		url: 'api/BlindStatus.php/' + id,
		data: 'state=' + this.name,
		error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
	});
	return false;
});
