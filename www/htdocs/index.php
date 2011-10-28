<html>
 <head>
  <title>Domotica control</title>
  <script src="js/jquery.js"></script>
  <style>
	div.icon {
		position: absolute;
	}
	div.popover {
		background-color: #000000;
		color: #dddddd;
		padding: 10px;
		z-index: 1000;
		position: absolute;
		min-width: 150px;
	}
  </style>
  </head>
 <body>
  <h1>Domotica control</h1>
  <div style="position: relative" id="floormap">
   <img src="data/floorplan.jpg" />
   <script type="text/javascript">
	var icon = {};
	icon.unknown = $('<div class="icon"><a href="#" onClick="return false;"><img src="images/unknown.png"/></a></div>');
	icon.light = $('<div class="icon"><a href="#" onClick="return false;"><img src="images/light.png"/></a></div>');
	icon.blind = $('<div class="icon"><a href="#" onClick="return false;"><img src="images/blind.png"/></a></div>');

    var control = {};
	control.unknown = $('<div class="popover"><div class="name">Name</div><div class="id">id</div></div>');

	control.light = $('<div class="popover"><div class="name">Name</div>' + 
			'<div class="id">id</div>' +
			'<input type="button" name="on" value="On" /><br/>' +
			'<input type="button" name="off" value="Off" />' +
			'</div>');
	control.light.find('input').click( function() {
		var id = $(this).parent().find(".id").text();
		$.ajax({
			type: 'POST',
			url: 'api/RelayStatus.php/' + id,
			data: 'state=' + this.name,
			error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
		});
	});

	control.blind = $('<div class="popover"><div class="name">Name</div>' + 
			'<div class="id">id</div>' +
			'<input type="button" name="up" value="Up" /><br/>' +
			'<input type="button" name="stop" value="Stop" /><br/>' +
			'<input type="button" name="down" value="Down" />' +
			'</div>');
	control.blind.find('input').click( function() {
		var id = $(this).parent().find(".id").text();
		$.ajax({
			type: 'POST',
			url: 'api/BlindStatus.php/' + id,
			data: 'state=' + this.name,
			error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
		});
	});

	function detachAll() {
		for( var i in control ) {
			control[i].detach();
		}
	}
	function attach(el, domel) {
		var c;
		switch( el.type ) {
		case "light":
		case "blind":
			c = control[ el.type ];
			break;
		default:
			c = control.unknown;
			break;
		}
		c.find(".name").text( el.name );
		c.find(".id").text( el.id );
		domel.append(c);
	}

	$.ajax({ url: 'data/coords.json', dataType: 'json'})
	  .error(function(jqXHR, text, error) { alert('error:'+text); })
	  .success(function(data) {
		$.each(data, function(index, el) {

			var domel;
			switch( el.type ) {
			case "light":
			case "blind":
				domel = icon[ el.type ].clone();
				break;
			default:
				domel = icon.unknown.clone();
				break;
			}

			$("#floormap").append( domel );
			domel.css('left', el.left - domel.width()/2 ).css('top', el.top - domel.height()/2);

			domel.click( function() {
				// Closure with all details
				detachAll();
				attach(el, domel);
				return false;
			});
		});
		$(document).click( detachAll );
	});
   </script>
  </div>
 </body>
</html>
