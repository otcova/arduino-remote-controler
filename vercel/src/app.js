import cookieSession from "cookie-session"
import express from "express"
import fs from "fs/promises"
import passport from "passport"
import keys from "../private/keys.js"
import authRoutes from "./auth-routes.js"
import { forceSsl } from "./forceSsl.js"
import { changeState, getState } from "./state.js"

const app = express()

app.set("view engine", "ejs")

app.use(cookieSession({
	maxAge: 24 * 60 * 60 * 1000,
	keys: keys.cookie,
}))

app.use(passport.initialize())
app.use(passport.session())

app.use("/auth", authRoutes)

const backgroundImage = await fs.readFile("./views/background.jpg")
app.get("/background.jpg", (request, response) => {
	response.send(backgroundImage);
})

const faviconImage = await fs.readFile("./views/favicon.png")
app.get("/favicon.png", (request, response) => {
	response.send(faviconImage);
})


app.get("/", forceSsl, async (request, response) => {
	if (!request.user) {
		response.redirect("/auth/google")
		return
	}
	response.render("home", { user: request.user, state: await getState() })
})

app.get("/open-computer", forceSsl, (request, response) => {
	if (request.user) changeState()
	response.redirect("/")
})

app.get("/state", async (request, response) => {
	response.send(await getState())
})

const port = process.env.PORT || 3000
app.listen(port, () => {
	console.log(`Listening on port ${port}`)
})