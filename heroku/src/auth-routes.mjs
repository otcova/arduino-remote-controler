import express from "express"
import passport from "passport"
import { forceSsl } from "./forceSsl.mjs"
import "./passport-setup.mjs"

const router = express.Router()

router.get("/google", forceSsl, passport.authenticate("google", {
	scope: ["profile"],
}))

router.get("/google/redirect", passport.authenticate("google"),
	(request, response) => response.redirect("/")
)

export default router