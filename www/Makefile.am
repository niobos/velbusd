velbuswebdir = $(prefix)/share/@PACKAGE@/velbusweb
EXTRA_DIST = velbusweb.js
EXTRA_DIST += config.js package.json velbus.js velbusweb.js
EXTRA_DIST += controls/ velbusMessages/ public/

install-data-hook:
	install -d "$(velbuswebdir)"
	cp -av . "$(velbuswebdir)/."
	cd "$(velbuswebdir)"; npm install
