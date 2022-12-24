import { Octokit, App } from "https://cdn.skypack.dev/octokit";

function get_auth_token(password) {
	try {
		const encrypted_token = 'U2FsdGVkX19tr7tCWjnMN3nF0U3AezCz2aJ9e8JZiVRPVks2O7C8yMCIQhaoV4F663hhIk6RSL5MqbbJJIz6Eg=='
		const token = CryptoJS.AES.decrypt(encrypted_token, password).toString(CryptoJS.enc.Utf8)
		if (!token.startsWith("ghp_")) return null
		return token
	} catch (e) {
		return null
	}
}

let token
const input = document.getElementById("input_token_password")
const button = document.getElementById("btn")
button.hidden = true

button.onclick = async () => {
	if (buttonClick) return

	const octokit = new Octokit({ auth: token });

	const owner = 'otcova'
	const repo = 'internet-remote-controler'
	const path = 'docs/state.txt'

	let state_file = await octokit.request(`GET /repos/${owner}/${repo}/contents/${path}`, {
		owner,
		repo,
		path,
	})
	let state = atob(state_file.data.content)
	if (state.length != 7) return
	if (!state.startsWith("state=")) return
	let new_state = "state=" + String.fromCharCode(state.charCodeAt(6) + 1)
	if (new_state.charAt(6) < 'A' || 'Z' < new_state.charAt(6)) new_state = "state=A"

	await octokit.request(`PUT /repos/${owner}/${repo}/contents/${path}`, {
		owner,
		repo,
		path,
		message: 'change state',
		committer: {
			name: 'otcova-helper',
			email: 'otcova.helper@gmail.com'
		},
		content: btoa(new_state),
		sha: state_file.data.sha,
	})

	buttonClick = true
}

let seed = 2;
function random() {
	var x = Math.sin(seed++) * 10000;
	return x - Math.floor(x);
}
const arrows = []
for (let i = 0; i < 200; ++i) {
	const img = document.createElement("img")
	document.body.append(img)
	img.src = "./cursor.png"
	img.width = 100
	img.className = "cursor"

	let x, y;
	let collision = true;
	while (collision) {
		x = random() * 2000 - 1000
		y = random() * 2000 - 1000
		collision = x ** 2 < 200 ** 2 && y ** 2 < 80 ** 2;
		for (const arrow of arrows) {
			if ((x - arrow.x) ** 2 + (y - arrow.y) ** 2 < 50 ** 2) {
				collision = true;
				break;
			}
		}
	}

	img.style.translate = `${x}px ${y}px`
	const angle = random() * 2 * Math.PI - Math.PI
	img.style.rotate = angle + "rad"
	arrows.push({ x, y, img, offset: { x: 0, y: 0 }, angle })
}

function on_correct_password() {
	button.hidden = false
	button.style.display = "inline-block"
	input.hidden = true



	update_scene();
}

let mouseX = 0, mouseY = 0;
let buttonDown = false;
let buttonClick = false;

function update_scene() {
	let x = 50 * (mouseX - window.innerWidth / 2) / window.innerWidth
	let y = 50 * (mouseY - window.innerHeight / 2) / window.innerHeight
	if (buttonClick) {
		x = 0;
		y = 0;
	}
	button.style.boxShadow = `${1 - .5 * x}px ${1 - .5 * y}px #000`
	button.style.translate = `${x}px ${y}px`

	seed = 1

	for (const arrow of arrows) {
		let speed = 0.3 * random() + 1.02
		if (buttonClick) {
			arrow.offset.x = ((arrow.offset.x + 1.1 * arrow.x) / speed) - 1.1 * arrow.x;
			arrow.offset.y = ((arrow.offset.y + 1.1 * arrow.y) / speed) - 1.1 * arrow.y;
		} else if (buttonDown) {
			arrow.offset.x = ((arrow.offset.x + 0.1 * arrow.x) / 1.3 / speed) - 0.1 * arrow.x;
			arrow.offset.y = ((arrow.offset.y + 0.1 * arrow.y) / 1.3 / speed) - 0.1 * arrow.y;
		} else {
			arrow.offset.x /= 1.1;
			arrow.offset.y /= 1.1;
		}

		let arrow_x = arrow.x + arrow.offset.x, arrow_y = arrow.y + arrow.offset.y
		if (buttonClick && arrow_x ** 2 + arrow_y ** 2 < 50 ** 2) {
			arrow.img.remove()
			continue;
		}

		let angle_target = Math.atan2(arrow_y - y, arrow_x - x) - Math.PI / 3
		let angle_dist = (angle_target - arrow.angle + Math.PI) % (Math.PI * 2) - Math.PI
		arrow.angle += angle_dist * 0.05

		arrow.img.style.translate = `${arrow_x}px ${arrow_y}px`
		arrow.img.style.rotate = arrow.angle + "rad"
	}

	requestAnimationFrame(update_scene);
}

document.onmousemove = event => {
	mouseX = event.clientX;
	mouseY = event.clientY;
}

document.ontouchmove = event => {
	mouseX = event.touches[0].clientX;
	mouseY = event.touches[0].clientY;
}

button.ontouchstart = () => buttonDown = true
document.ontouchend = () => { buttonDown = false }

button.onmousedown = () => buttonDown = true
document.onmouseup = () => buttonDown = false


input.oninput = () => {
	token = get_auth_token(input.value)
	if (token) on_correct_password()
}