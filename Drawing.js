const height = window.innerHeight ||
	document.getElementsByTagName("html")[0].clientHeight  ||
	document.getElementsByTagName("body")[0].clientHeight  ||
	screen.availHeight;

const SIZE = Math.floor(height * 0.08);

const DARK = "#b58863";
const LIGHT = "#f0d9b5";

const SEL_DARK = "#e0c434";
const SEL_LIGHT = "#f8ec5c";

const LAST_MOVE_DARK = "#aaa23a";
const LAST_MOVE_LIGHT = "#cdd26a";

decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
console.clear();

document.getElementById("fen").value = encode();

var freezeBoard = true;
var humanSide = false;

var rows = [];

var selected = null;
var sel1 = null;
var sel2 = null;

var moves = [];
var movesFromSelected = [];

var book = new Map();

const TIME = 1500;

var initialized = false;
var promotionSquare = -1;

function init() {
	document.getElementById("perft").onclick = () => window.location.replace("./perft.html");

	let current = 0;
	document.getElementById("board").innerHTML = "";

	for (let i = 0; i < 8; i++) {
		rows[i] = document.createElement("tr");
		for (let j = 0; j < 4; j++) {
			const td1 = document.createElement("td");
			const td2 = document.createElement("td");

			const light = document.createElement("div");

			let notation = XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
			if (humanSide == true)
				notation = XYToNotation(`${Math.floor((63-current) / 8)}${(63-current) % 8}`);
			
			light.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${LIGHT};`);
			light.className = "square";
			light.id = notation.toString();
			light.onclick = () => {
				click(parseInt(light.id));
			}
			if (board[notation] > 0) {
				const image = document.createElement("img");
				image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
				image.src = `./Assets/${getImage(board[notation])}.png`;
				image.className = "piece";
				light.appendChild(image);
			}

			current++;

			const dark = document.createElement("div");

			notation = XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
			if (humanSide == true)
				notation = XYToNotation(`${Math.floor((63-current) / 8)}${(63-current) % 8}`);
			dark.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${DARK};`);
			dark.className = "square";
			dark.id = notation.toString();
			dark.onclick = function() {
				click(parseInt(dark.id));
			}
			if (board[notation] > 0) {
				const image = document.createElement("img");
				image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
				image.src = `./Assets/${getImage(board[notation])}.png`;
				image.className = "piece";
				dark.appendChild(image);
			}

			current++;

			td1.append(light);
			td2.append(dark);

			rows[i].appendChild(td1);
			rows[i].appendChild(td2);
		}

		i++;

		rows[i] = document.createElement("tr");
		for (let j = 0; j < 4; j++) {
			const td1 = document.createElement("td");
			const td2 = document.createElement("td");

			const dark = document.createElement("div");

			let notation = XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
			if (humanSide == true)
				notation = XYToNotation(`${Math.floor((63-current) / 8)}${(63-current) % 8}`);
			dark.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${DARK};`);
			dark.className = "square";
			dark.id = notation.toString();
			dark.onclick = () => {
				click(parseInt(dark.id));
			}
			if (board[notation] > 0) {
				const image = document.createElement("img");
				image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
				image.src = `./Assets/${getImage(board[notation])}.png`;
				image.className = "piece";
				dark.appendChild(image);
			}

			current++;

			const light = document.createElement("div");

			notation = XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
			if (humanSide == true)
				notation = XYToNotation(`${Math.floor((63-current) / 8)}${(63-current) % 8}`);
			light.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${LIGHT};`);
			light.className = "square";
			light.id = notation.toString();
			light.onclick = () => {
				click(parseInt(light.id));
			}
			if (board[notation] > 0) {
				const image = document.createElement("img");
				image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
				image.src = `./Assets/${getImage(board[notation])}.png`;
				image.className = "piece";
				light.appendChild(image);
			}

			current++;

			td1.append(dark);
			td2.append(light);

			rows[i].appendChild(td1);
			rows[i].appendChild(td2);
		}
	}

	for (let row of rows)
		document.getElementById("board")?.appendChild(row);

	document.getElementById("queen").onclick = () => {
		freezeBoard = false;
		board[promotionSquare] = getColor(board[promotionSquare]) ? PIECES.indexOf("q") : PIECES.indexOf("Q");
		update();
		computerMove();
		document.getElementById("promotion").removeAttribute("open");
	}
	document.getElementById("rook").onclick = () => {
		freezeBoard = false;
		board[promotionSquare] = getColor(board[promotionSquare]) ? PIECES.indexOf("r") : PIECES.indexOf("R");
		update();
		computerMove();
		document.getElementById("promotion").removeAttribute("open");
	}
	document.getElementById("bishop").onclick = () => {
		freezeBoard = false;
		board[promotionSquare] = getColor(board[promotionSquare]) ? PIECES.indexOf("b") : PIECES.indexOf("B");
		update();
		computerMove();
		document.getElementById("promotion").removeAttribute("open");
	}
	document.getElementById("knight").onclick = () => {
		freezeBoard = false;
		board[promotionSquare] = getColor(board[promotionSquare]) ? PIECES.indexOf("n") : PIECES.indexOf("N");
		update();
		computerMove();
		document.getElementById("promotion").removeAttribute("open");
	}

	document.getElementById("close").onclick = () => {
		document.getElementById("result").removeAttribute("open");
	}

	document.getElementById("resetFenInput").value = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}
function update() {
	for (let piece of document.querySelectorAll(".piece"))
		piece.remove();

	for (let i = 0; i < 144; i++) {
		let notation = i;
		if (humanSide)
			notation = 143-i;
		if (board[notation] <= 0)
			continue;

		const image = document.createElement("img");
		image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
		image.src = `./Assets/${getImage(board[notation])}.png`;
		image.className = "piece";
		document.getElementById(notation.toString())?.appendChild(image);
	}
}	

function click(current) {
	if (freezeBoard)
		return;

	if (selected == null) {
		if (board[current] > 0 && getColor(board[current]) == turn && getColor(board[current]) == humanSide) {
			document.getElementById(current.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(current) ? SEL_LIGHT : SEL_DARK}`);
			selected = current;
			movesFromSelected.length = 0;
			for (let move of moves) {
				if (move.source == current) {
					movesFromSelected.push(move);
					if (move.promote == 0 || move.promote == 5 || move.promote == 11) {
						const image = document.createElement("img");
						image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 50%`);
						image.src = board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
						image.className = "highlight";
						document.getElementById(move.dest.toString())?.appendChild(image);
					}
				}
			}
		}
		return;
	}

	if (current == selected)
		return;

	for (let move of movesFromSelected) {
		if (move.dest == current) {
			let promote = false;

			move.source = selected;
			if (move.promote != 0) {
				promote = true;
				move.promote = 0;
				freezeBoard = true;
				promotionSquare = move.dest;
				document.getElementById("promotion").setAttribute("open", "");
			}

			document.getElementById(selected.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; 
				background-color: ${isLight(selected) ? LIGHT : DARK}`);
			makeMove(move);

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
			selected = null;

			highlightLastMove(move);

			for (let piece of document.querySelectorAll(".highlight"))
				piece.remove();

			update();
			movesFromSelected.length = 0;

			moves = moveGen();

			if (!promote)
				computerMove();

			return;
		}
	}

	document.getElementById(selected.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(selected) ? LIGHT : DARK}`);
	for (let piece of document.querySelectorAll(".highlight"))
		piece.remove();

	if (board[current] > 0 && getColor(board[current]) == turn) {
		document.getElementById(current.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(current) ? SEL_LIGHT : SEL_DARK}`);
		selected = current;
		movesFromSelected.length = 0;
		for (let move of moves) {
			if (move.source == current) {
				movesFromSelected.push(move);
				if (move.promote == 0 || move.promote == 5 || move.promote == 11) {
					const image = document.createElement("img");
					image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 50%`);
					image.src = board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
					image.className = "highlight";
					document.getElementById(move.dest.toString())?.appendChild(image);
				}
			}
		}
	}
	else
		selected = null;
}

function computerMove() {
	let bookMoves = book.get(encode().split(" ")[0]);
	if (bookMoves != undefined) {
		let bookMove = bookMoves[Math.floor(Math.random() * bookMoves.length)];
		for (let move of moveGen()) {
			if (moveToString(move) == bookMove) {
				document.getElementById("depth").innerHTML = "<b>Depth: Book Move</b>";
				document.getElementById("eval").innerHTML = "<b>Evaluation: -</b>";
				document.getElementById("move").innerHTML = `<b>Move: ${bookMove}</b>`;

				makeMove(move);
				highlightLastMove(move);
				moves = moveGen();
				update();

				return;
			}
		}
	}

	const worker = new Worker("./SearchWorker.js");
	worker.postMessage([encode(), TIME]);
	worker.onmessage = e => {
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

		document.getElementById("depth").innerHTML = `<b>Depth: ${e.data[1]} ${e.data[3] ? `<span class="red">(Mate in ${Math.round(e.data[1]/2)} found)</span>` : ""}</b>`;
		document.getElementById("eval").innerHTML = `<b>Evaluation: ${e.data[2] > 0 ? "+" : ""}${e.data[2] / 100}</b>`;

		document.getElementById("move").innerHTML = `<b>Move: ${moveToString(e.data[0])}</b>`;

		update();
		moves = moveGen();
	}
}

function highlightLastMove(move) {
	if (sel1 != null) {
		document.getElementById(sel1).setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; 
			background-color: ${isLight(sel1) ? LIGHT : DARK}`);
		document.getElementById(sel2).setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; 
			background-color: ${isLight(sel2) ? LIGHT : DARK}`);
	}

	sel1 = move.source;
	sel2 = move.dest;

	document.getElementById(sel1.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; 
		background-color: ${isLight(sel1) ? LAST_MOVE_LIGHT : LAST_MOVE_DARK}`);
	document.getElementById(sel2.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; 
		background-color: ${isLight(sel2) ? LAST_MOVE_LIGHT : LAST_MOVE_DARK}`);
}

function initSidePicker() {
	document.getElementById("whiteButton").onclick = () => {
		initialized = true;
		freezeBoard = false;
		document.getElementById("color").removeAttribute("open");
		moves = moveGen();
		init();
		if (turn != humanSide)
			computerMove();
	}
	document.getElementById("blackButton").onclick = () => {
		initialized = true;
		humanSide = true;
		freezeBoard = false;
		document.getElementById("color").removeAttribute("open");
		moves = moveGen();
		init();
		if (turn != humanSide)
			computerMove();
	}

	document.getElementById("color").setAttribute("open", "");
}

document.getElementById("reset").onclick = () => {
	if (initialized) {
		document.getElementById("resetInput").setAttribute("open", "");
		init();
		update();
	}
}
document.getElementById("loadButton").onclick = () => {
	let fen = document.getElementById("resetFenInput").value;
	if (!validFEN(fen)) {
		alert("Invalid FEN");
		return;
	}
	decode(fen);
	init();

	document.getElementById("depth").innerHTML = "<b>Depth: </b>"
	document.getElementById("eval").innerHTML = "<b>Eval: </b>"
	document.getElementById("move").innerHTML = "<b>Move: </b>"

	update();
	document.getElementById("resetInput").removeAttribute("open");

	initSidePicker();
}
document.getElementById("closeButton").onclick = () => {
	document.getElementById("resetInput").removeAttribute("open");
}

init();
initSidePicker();
initOpeningBook();