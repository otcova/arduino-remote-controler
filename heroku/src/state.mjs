import fs from "fs/promises"

const stateFilePath = "./views/state.txt"

export const getState = async () => {
	return (await fs.readFile(stateFilePath)).toString()
}

export const changeState = async () => {
	const newState = (await getState()) == "0"? "1" : "0"
	fs.writeFile(stateFilePath, newState)
}