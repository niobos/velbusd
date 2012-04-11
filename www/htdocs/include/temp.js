$('<style type="text/css">' +
		'div.icon.temp { background-image: url(images/temp) }' +
	'</style>').appendTo("head");

$('<div class="control template temp">' +
		'<img id="tempgraph" src=""/>' +
	'</div>'
 ).appendTo("#control").bind('update', function(event, element) {
	$("#tempgraph").attr('src', 'graphs/TS.php?' +
		'id=' + element.id + '&' +
		'id_relay=' + element.relay + '&' +
		'title=' + element.name + '&' +
		'width=150&height=70&graph_only=1');
});
