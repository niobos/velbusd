$('<style type="text/css">' +
		'div.icon.temp { background-image: url(images/temp.svg) }' +
	'</style>').appendTo("head");

$('<div class="control template temp">' +
		'Current: <span id="tempcur">?</span>&deg;C, target: <span id="temptarget">?</span>&deg;C<br/>' +
		'<img id="tempgraph" src=""/>' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, id, element, coords) {
	$("#tempgraph").attr('src', 'graph/temp/' + id + '?' +
		'width=150&height=70&graph_only=1' +
		'&refresh=' + Math.floor((+new Date())/1000/60) // Refresh max once every minute
		);

	$("#tempcur").text('?');
	$("#temptarget").text('?');
	$.ajax({ url: 'control/temp/' + id, dataType: 'json' })
		.success(function(data) {
				$("#tempcur").text(data['current temperature']);
				$("#temptarget").text(data['target temperature']);
			});
});
