exports.velbusd = {
	'host': 'localhost',
	'port': 8445,
	'reconnect_after_ms': 5000,
}

exports.webapp = {
	'port': 8080,
	'timeout': 2000,
}

exports.controls = [
	{
		"id": "02.1",
		"name": "Master bedroom light",
		"left": 590, "top": 270,
		"type": "light"
	},
	{
		"id": "01.1",
		"name": "Master bedroom blind",
		"left": 730, "top": 270,
		"type": "blind"
	},
	{
		"id": "04",
		"name": "Master bedroom temp",
		"left": 450, "top": 365,
		"type": "temp",
		"relay": "06.4"
	}
];
