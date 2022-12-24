import passport from "passport"
import GoogleStrategy from "passport-google-oauth20"
import keys from "../private/keys.js"
import whitelist from "../private/whitelist.js"

passport.serializeUser((user, done) => {
	done(null, user.id)
})

passport.deserializeUser((userId, done) => {
	const user = whitelist.get(userId)
	done(null, user)
})

passport.use(
	new GoogleStrategy({
		...keys.google,
		callbackURL: "https://internet-remote-controler.herokuapp.com/auth/google/redirect",
	}, (accessToken, refreshToken, profile, done) => {
		if (whitelist.has(profile.id)) {
			done(null, whitelist.get(profile.id))
		} else {
			console.log("---- Unknown user ----")
			console.log(profile)
			console.log("----------------------")
		}
	})
)