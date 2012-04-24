$('<style type="text/css">' +
		'div.icon.temp { background-image: url(images/temp.svg) }' +
	'</style>').appendTo("head");

$('<div class="control template temp">' +
		'Current: <span id="tempcur">?</span>&deg;C, target: <span id="temptarget">?</span>&deg;C<br/>' +
		'<img id="tempgraph" src=""/>' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, element) {
	$("#tempgraph").attr('src', 'graphs/TS.php?' +
		'id=' + element.id + '&' +
		'id_relay=' + element.relay + '&' +
		'title=' + element.name + '&' +
		'width=150&height=70&graph_only=1');

	$("#tempcur").text('?');
	$("#temptarget").text('?');
	$.ajax({ url: 'api/TempStatus/' + element.id, dataType: 'json' })
		.success(function(data) {
				$("#tempcur").text(data['temp']);
				$("#temptarget").text(data['target temp']);
			});
});
