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
* libboost


For velbusweb:

* Node.js and NPM


Sketchy setup instructions
--------------------------
```
your-dev-machine$ git clone https://github.com/niobos/velbusd.git
your-dev-machine$ ln -s README.md README
your-dev-machine$ autoreconf
your-dev-machine$ ./configure
your-dev-machine$ make dist
```

Then copy over the `tar.gz` or `tar.bz2` file to your velbus-machine

```
your-velbus-machine$ tar jxvf velbusd.tar.bz2
your-velbus-machine$ cd velbusd
your-velbus-machine$ ./configure && make
your-velbus-machine$ sudo make install
your-velbus-machine$ cp contrib/velbusd.conf contrib/velbusweb.conf /etc/init/.
```

optionally change the configuration:

* velbusd has its (very limited) config in /etc/default/velbusd.conf (see the
  upstart script for the supported variables and their default)
* velbusweb has its config in /usr/local/share/velbusd/velbusweb/config.js.

```
your-velbus-machine$ sudo start velbusd
your-velbus-machine$ sudo start velbusweb
```


Copyright / License
------------------
Light-bulp image licensed under Creative Commons BY(Ignacio javier igjav)-SA

The rest of the project is licensed under AGPL3, see COPYING for the details.
