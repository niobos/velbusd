Velbusd
=======

Velbusd is a project to extend the Velbus home automation to HTTP-based
clients. It contains a deamon program `velbusd` that acts as a proxy between
the serial port and TCP sockets. The second part `velbusweb` is a web
application server that translates HTTP requests to/from the TCP socket.


Dependencies
------------

For the velbusd:

* libev, including the development files (libev-dev on debian-based systems)


For velbusweb:

* Node.js and NPM


Copyright
---------
Light-bulp image licensed under Creative Commons BY(Ignacio javier igjav)-SA

The rest of the project is licensed under AGPL3, see COPYING for the details.
