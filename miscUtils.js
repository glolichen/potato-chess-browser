function moveGen() {
	try {
		Module["decode"](encode());
	}
	catch {
		return
	}
	let out = Module["moveGen"]();

	let legalMoves = [];
	for (let i = 0; i < out.size(); i++) {
		let current = out.get(i);

		let move = new Move(current.source, current.dest, charToPiece(current.capture), current.castle, 
		charToPiece(current.promote), String.fromCharCode(current.signal), current.isEp);

		legalMoves.push(move);
	}

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

function computerMove() {
	let bookMoves = book.get(encode().split(" ")[0]);
	if (bookMoves != undefined) {
		let bookMove = bookMoves[Math.floor(Math.random() * bookMoves.length)];
		for (let move of moveGen()) {
			if (moveToUCI(move) == bookMove) {
				document.getElementById("depth").innerHTML = "<b>Depth: Book Move</b>";
				document.getElementById("eval").innerHTML = "<b>Evaluation: -</b>";

				makeMove(move);
				highlightLastMove(move);
				moves = moveGen();
				update();

				return;
			}
		}
	}

	let pieceCount = 0;
	for (let i = 0; i < 144; i++) {
		if (board[i] > 0)
			pieceCount++;
	}

	if (pieceCount <= 7) {
		fetch(`https://tablebase.lichess.ovh/standard?fen=${encode()}`).then(response => response.json()).then(response => {
			let baseMove = response.moves[0].uci;
			let allMoves = moveGen();
			for (let move of allMoves) {
				if (baseMove == moveToUCI(move)) {
					let result = "Draw";
					let category = response.moves[0].category;
					if (category == "win")
						result = `${humanSide ? "Black" : "White"} wins`;
					else if (category == "loss")
						result = `${humanSide ? "White" : "Black"} wins`;

					document.getElementById("depth").innerHTML = "<b>Depth: Endgame Tablebase Move</b>";
					document.getElementById("eval").innerHTML = `<b>Evaluation: ${result}</b>`;
	
					makeMove(move);
					highlightLastMove(move);
					moves = moveGen();
					update();
	
					return;
				}
			}
		});

		return;
	}

	let worker = new Worker("./searchWorker.js");
	let done = false;

	setTimeout(() => {
		if (!done) {
			alert("worker stuck, terminated");
			worker.terminate();
			computerMove();
		}
	}, TIME + 1000);

	worker.postMessage([encode(), TIME]);
	worker.onmessage = e => {
		document.getElementById("depth").innerHTML = `<b>Depth: ${e.data[1]} ${e.data[3] ? `<span class="red">(Mate in ${Math.round(e.data[1]/2)} found)</span>` : ""}</b>`;
		document.getElementById("eval").innerHTML = `<b>Evaluation: ${e.data[2] > 0 ? "+" : ""}${e.data[2] / 100}</b>`;
		
		makeMove(e.data[0]);

		let text = "";
		let gameEnd = false;
		if (fiftyMoveClock >= 50)  {
			text = "Draw by 50 move rule";
			gameEnd = true;
		}
		if (insufMat())  {
			text = "Draw by insufficient material";
			gameEnd = true;
		}
		if (gameEnd) {
			let paragraph = document.createElement("p");
			paragraph.textContent = text;
			
			let button = document.createElement("button");
			button.onclick = () => {
				result.removeAttribute("open");
			}
			button.textContent = "OK";

			result.append(paragraph);
			result.append(button);

			setTimeout(() => result.setAttribute("open", ""), 100);
		}

		document.getElementById("fen").value = encode();
		highlightLastMove(e.data[0]);

		update();
		moves = moveGen();

		done = true;
		worker.terminate();
	}
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

