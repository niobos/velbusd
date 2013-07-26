Velbusd
=======

Velbusd is a project to extend the Velbus home automation to HTTP-based
clients. It contains a deamon program `velbusd` that acts as a proxy between
the serial port and TCP sockets. The second part `velbusweb` is a web
application server that translates HTTP requests to/from the TCP socket.
Both modules are supplied with this software package.


Dependencies
------------

For the velbusd:

* libev, including the development files (libev-dev on debian-based systems)
* libboost (build dependency only)
* autotools (included in ubuntu's `build-essential` package)


For velbusweb:

* Node.js and NPM


Sketchy setup instructions
--------------------------
These instructions assume 2 machines: your own development machine, and the
server you want to run the daemons on. You can give both roles to a single
machine without a problem; just run all commands on the same host.

First, get the source, either by running
`git clone https://github.com/niobos/velbusd.git` or by downloading and
extracting the ZIP. Once in the project directory, do:

```
your-dev-machine$ ln -s README.md README
your-dev-machine$ autoreconf -i
your-dev-machine$ ./configure
your-dev-machine$ make dist   # Not needed if you don't have 2 machines
```

Then copy over the `tar.gz` or `tar.bz2` file to your velbus-machine

```
your-velbus-machine$ tar jxvf velbusd.tar.bz2   # not needed for 1 machine
your-velbus-machine$ cd velbusd
your-velbus-machine$ ./configure && make
your-velbus-machine$ sudo make install
your-velbus-machine$ sudo cp contrib/velbusd.conf contrib/velbusweb.conf /etc/init/.
```

optionally change the configuration in /etc/default/velbusd (see the upstart
script for the supported variables and their default)

```
your-velbus-machine$ sudo start velbusd
```

To use velbusweb, you should probably change
/usr/local/share/velbusd/velbusweb/public/data/floorplan.svg to match your
floorplan, and add your modules to /usr/local/share/velbusd/velbusweb/config.js.
Next, start the daemon
```
your-velbus-machine$ sudo start velbusweb
```

More detailed instructions contributed
======================================

Installation instructions when starting from scratch
----------------------------------------------------
1. Download the zip file from Github
2. Install the following programs from the repository
	libtool
	libboost-all-dev
	python-software-properties
	nodejs
	npm (in Ubuntu repositories since v. 12.04)
	automake
	build-essential
3. Unzip the file in your home directory (or elsewhere you wish)
4. touch ~/README
	There is a small error in the way Github works. This file must be made manually
	Else you will get an error.
5. Fix the the following link (or change for a higher version number)
	ln -s /usr/share/automake-1.11/INSTALL INSTALL
6. autoreconf -i
7. ./configure
8. sudo make
9. sudo make install
10. make the following directory /var/log/domotica directory
11. make (touch) the following file: /var/log/domotica/domotica.log
12. sudo cp ~/velbusd-master/contrib/*.conf /etc/init
13. sudo cp ~/velbusd-master/contrib/velbusd.conf /etc/default/velbusd
14. Check what the USB address is when you plugin the USD interface to the server (logfile!)
15. Use the appropriate USB address in the following testing from the command line
	sudo velbusd -f -s /dev/ttyACM0 -b [127.0.0.1]:[8445]
If you get messages from the velbus interface you are in business
17. Change /etc/default/velbusd from the default
	it should contain your new USB connection path
	it should contain you IP connection (default does not always work)
	ot should contain your log file path	
18. Modify /usr/local/share/velbusd/velbusweb/public/data/floorplan.svg using Inkscape
	(or other vector drawing program)
19. Modify /usr/local/share/velbusd/velbusweb/config.js containing your module definitions
20. Start the interface daemon (sudo start velbusd)
21. Copy contrib/velbusweb.conf to /etc/init/
22. Start the web daemon (sudo start velbusweb)


Is the daemon active?
---------------------
The velbusd daemon can be found in the process list when typing 'ps -ef'. It
will show the interface, the logfile reference and the port binding. The
velbusweb daemon should also show in this process list.


Velbus module information
-------------------------
In config.js you have to list the modules of your installation. See the default
file supplied at installation for an example. Only the temperature module is
without any channels and can be called directly by the module number. All other
modules must stage module.channel, even the single channel relays modules.


Adding modules or icons (config.js)
-----------------------------------
In config.js the configuration file refers to "type" and "icon". The type is
the type of Velbus module, the icon naturally refers to the icon that must be
displayed on the webpage. Currently defined icons are: light (on/off), gate,
fan, temperature (red-blue). For a new "type", add a file to www/controls/ (for
the backend code coding) and in www/public/js/controls/ (for the frontend
coding). A new icon is a .svg type file in www/public/js/controls/. To make the
link between the code and the icon, you must change the first lines e.g.
www/public/js/controls/relay.js.

Logging of velbusd communication
--------------------------------
Communication between the daemon and Velbus is logged by default to stderr.


Bugs
====

The newer VMB4RYNO allows to directly access all channels from the webpage to
start and stop the relais. The older VMB4RY only allows this for the first
channel. The other channels currently can only be started by entering a start
time and a duration. This is a known bug for which there is not yet a solution.


Copyright / License
==================
Light-bulb image licensed under Creative Commons BY(Ignacio javier igjav)-SA

The rest of the project is licensed under AGPL3, see COPYING for the details.
