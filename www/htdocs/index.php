<html>
 <head>
  <title>Domotica control</title>
  <script src="js/jquery.js"></script>
  </head>
 <body>
  <h1>Domotica control</h1>
  <div style="position: relative" id="floormap">
   <img src="data/floorplan.jpg" />
   <script type="text/javascript">
	$.ajax({ url: 'data/coords.json', dataType: 'json'})
	  .error(function(jqXHR, text, error) { alert('error:'+text); })
	  .success(function(data) {
		$.each(data, function(index, el) {
			var domel = "<div style=\"position: absolute\">" +
			            "<a href=\"#\" onClick=\"return false;\" >" +
			            "<img src=\"images/unknown.png\" />" +
			            "</a></div>";
			domel = $( domel ).css('left', el.left-10).css('top', el.top-10);
			$("#floormap").append( domel );
		});
	});;
   </script>
  </div>
 </body>
</html>
