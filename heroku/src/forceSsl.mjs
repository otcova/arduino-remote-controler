export const forceSsl = (request, response, next) => {
	if (request.headers.host != "localhost:3000" &&
		request.headers['x-forwarded-proto'] !== 'https') {
		return response.redirect(['https://', request.get('Host'), request.url].join(''))
	}
	return next()
}