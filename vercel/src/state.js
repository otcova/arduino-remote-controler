import fs from "fs/promises"

const stateFilePath = "./views/state.txt"

export const getState = async () => {
	return (await fs.readFile(stateFilePath)).toString()
}

export const changeState = async () => {
	const newState = (await getState()) == "A"? "B" : "A"
	fs.writeFile(stateFilePath, newState)
}