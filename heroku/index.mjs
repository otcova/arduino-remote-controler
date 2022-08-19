import net from "net"
import fetch from "node-fetch"

const port = process.env.PORT || 80
console.log("PORT: ", port)

const getState = async () => {
	const repo_content = (await (await fetch(
		'https://api.github.com/repos/otcova/internet-remote-controler/contents/state.txt',
		{
		  method: 'GET',
		  headers: {
			accept: 'application/vnd.github.v3+json',
		  },
		}
	)).json()).content
	return Buffer.from(repo_content, "base64").toString().charAt(0)
}

const server = net.createServer(socket => {
	let data = ""
	socket.on("data", async stream => {
		data += stream.toString("utf8")
		if (data.includes("\r\n\r\n")) {
			if (data.includes("GET /state")) {
				let data = await getState()
				if (socket.writable) {
					socket.write('HTTP/1.0 200 OK\r\n\r\n' + data)
					console.log("Sended state:", data)
				}
			} else if (data.includes("favicon.ico")) {
				socket.write('HTTP/1.0 200 OK\r\n\r\n')
				console.log("Sended ICON")
			} else {
				socket.write('HTTP/1.0 200 OK\r\n\r\nHello world')
				console.log("Sended hello")
			}
			socket.end()
		}
	})
})

server.listen(port)