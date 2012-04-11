<html>
 <head>
  <title>Domotica control</title>
  <script src="js/jquery.js"></script>
  <style>
	div.icon {
		position: absolute;
		width: 1.2em;
		height: 1.2em;
		cursor: pointer;
		background-image: url(images/unknown);
		background-size: contain;
	}
	div.icon.light {
		background-image: url(images/light-on);
	}
	div.icon.blind {
		background-image: url(images/blind);
	}
	div.icon.temp {
		background-image: url(images/temp);
	}

	div.popover {
		top: 1.3em; /* Place them below the icon */
		background-color: #000000;
		color: #dddddd;
		padding: 10px;
		z-index: 1000;
		position: absolute;
		min-width: 10em;
	}
	span.tech {
		font-size: 0.75em;
	}
	div.template {
		display: none;
	}
  </style>
  </head>
 <body>
  <h1>Domotica control</h1>
  <div style="position: relative" id="floormap">
   <div class="popover template">
    <!-- Header line, common for all -->
    <div style="white-space: nowrap; padding-bottom: .4em;">
     <span class="name">Name</span> <span class="tech">(<span class="type">type</span> @ <span class="id">FF</span>)</span>
    </div>

    <!-- Specific controls for light -->
    <div class="control template light">
     <input type="button" name="on" value="On" /><br/>
     <input type="button" name="off" value="Off" />
    </div>

    <!-- Specific controls for blinds -->
    <div class="control template blind">
     <input type="button" name="up" value="Up" /><br/>
     <input type="button" name="stop" value="Stop" /><br/>
     <input type="button" name="down" value="Down" />
    </div>

   </div>
   <script type="text/javascript">
    	$('.control.template.light input').click( function() {
    		var id = $(this).parent().parent().find(".id").text();
    		$.ajax({
    			type: 'POST',
    			url: 'api/RelayStatus.php/' + id,
    			data: 'state=' + this.name,
    			error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
    		});
    		return false;
    	});

    	$('.control.template.blind input').click( function() {
    		var id = $(this).parent().parent().find(".id").text();
    		$.ajax({
    			type: 'POST',
    			url: 'api/BlindStatus.php/' + id,
    			data: 'state=' + this.name,
    			error: function(jqXHR, textStatus, errorThrown) { alert(textStatus); }
    		});
    	});
   </script>

   <img src="data/floorplan" />

   <script type="text/javascript">
	$.ajax({ url: 'data/coords.json', dataType: 'json'})
	  .error(function(jqXHR, text, error) { alert('error:'+text); })
	  .success(function(data) {
		$.each(data, function(index, el) {

			if( el.type == undefined || el.left == undefined ||
				el.top == undefined || el.name == undefined ) {
				console.log('Elements must have at least `type`, `left`, `top`'
					+ 'and `name` attributes, ignoring this one');
				return true; // continue to next iteration
			}

			var icon = $('<div class="icon"/>').addClass( el.type );
			$("#floormap").append( icon );
			icon.css('left', el.left - icon.width()/2 ).css('top', el.top - icon.height()/2);

			icon.click( function() {
				// Closure with all details
				var popover = $('.popover');
				popover.find('.control').addClass('template');
				popover.removeClass('template').appendTo(icon);

				popover.find('.name').text(el.name);
				popover.find('.id').text(el.id);
				popover.find('.type').text(el.type);
				$('.control.' + el.type).removeClass('template');

				// Undo selection which has happened while clicking and appending at the same time
				if (window.getSelection) {
					if (window.getSelection().empty) {  // Chrome
						window.getSelection().empty();
					} else if (window.getSelection().removeAllRanges) {  // Firefox
						window.getSelection().removeAllRanges();
					}
				} else if (document.selection) {  // IE?
					document.selection.empty();
				}

				return false;
			});
		});
		$(document).click( function() { $('.popover').addClass('template'); } );
	});
   </script>
  </div>
 </body>
</html>
