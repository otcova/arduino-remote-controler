const net = require('net')
const port = process.env.PORT || 80
console.log("PORT: ", port)

import("node-fetch").then(({ default: fetch }) => {
	const sendState = socket => {
		console.log("Sending state");
		fetch("https://raw.githubusercontent.com/otcova/internet-remote-controler/main/state.txt")
			.then(response => response.json())
			.then(data => {
				socket.write("" + data);
				socket.end()
			})
	}

	const sendHello = socket => {
		console.log("Sending hello");
		socket.write('HTTP/1.0 200 OK\r\n\r\n')
		socket.write('Hello world')
		socket.end()
	}

	const server = net.createServer(socket => {
		let data = ""
		socket.on("data", stream => {
			data += stream.toString("utf8")
			if (data.includes("\r\n\r\n")) {
				if (data.includes("GET /state")) {
					sendState(socket);
				} else {
					sendHello(socket);
				}
			}
		})
	});
	server.listen(port);
});