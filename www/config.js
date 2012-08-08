exports.velbusd = {
	'host': 'localhost',
	'port': 8445,
	'reconnect_after_ms': 5000,
}

exports.webapp = {
	'port': 8080,
	'bind': '::',
	'timeout': 2000,
}

exports.graphs = {
	'directory': '/mnt/data/rrdtool',
}

exports.controls = {
	"0a-1": {
		"name": "Master bedroom light",
		"coords": [ {"left": 590, "top": 270} ],
		"type": "light"
	},
	"01-1": {
		"name": "Master bedroom blind",
		"coords": [ {"left": 730, "top": 270} ],
		"type": "blind",
		"time": 18
	},
	"04": {
		"name": "Master bedroom temp",
		"coords": [ {"left": 450, "top": 365} ],
		"type": "temp",
		"relay": "06-4"
	},
	"0b-1": {
		"name": "Garage gate",
		"coords": [ {"left": 500, "top": 100} ],
		"type": "gate",
	}
};
