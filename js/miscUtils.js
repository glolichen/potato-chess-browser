function moveGen() {
	try {
		Module["decode"](encode());
	}
	catch {
		return
	}
	let out = Module["moveGen"]();

	let legalMoves = [];
	for (let i = 0; i < out.size(); i++)
		legalMoves.push(out.get(i));

	if (legalMoves.length == 0) {
		gameOver = true;

		let inCheck = false;
		let attacked = Module["getAttacked"]();

		for (let i = 0; i < attacked.size(); i++) {
			if (attacked.get(i).coord == king[turn ? 1 : 0]) {
				inCheck = true;
				break;
			}
		}

		document.getElementById("resultText").textContent = inCheck ? "Checkmate" : "Stalemate";
		setTimeout(() => document.getElementById("result").setAttribute("open", ""), 100);
	}

	return legalMoves;
}

function insufMat() {
	pieces = [];
	for (let piece of board) {
		if (piece != 0 && piece != -1)
			pieces.push(piece);
	}

	if (pieces.length >= 4)
		return false;
	
	for (let piece of pieces) {
		piece = PIECES[piece].toLowerCase();
		if (piece == "r" || piece == "q" || piece == "p")
			return false;
	}

	return true;
}

function initOpeningBook() {
	let result = "";
	let xhr = new XMLHttpRequest();
	xhr.open("GET", "./Assets/book.txt", false);
	xhr.send();
	if (xhr.status == 200)
		result = xhr.responseText;
	
	for (let line of result.split("\n")) {
		let split = line.split(" ");
		let position = split.shift();
		book.set(position, split);
	}
}

function charToPiece(char) {
	let piece = PIECES.indexOf(String.fromCharCode(char));
	return piece == -1 ? 0 : piece;
}

function isLight(notation) {
	let coord = notationToXY(notation);
	return (parseInt(coord[0]) + parseInt(coord[1])) % 2 == 0;
}

