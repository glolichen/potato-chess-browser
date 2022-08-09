const FILES = "abcdefgh";
const NUMBERS = "0123456789";

var board = [];
var king = [];

var enPassant = -1;

var K = false; // White Kingside
var Q = false; // White Queenside
var k = false; // Black Kingside
var q = false; // Black Queenside

var turn = false; // false = White, true = Black

var moveClock = 1;
var fiftyMoveClock = 0;

function decode(fen) {
	document.getElementById("pgn").textContent = "";

	board.length = 144;
	board.fill(-1);

	king.length = 2;

	let result = fen.split(" ");

	turn = result[1] == "b";

	fiftyMoveClock = parseInt(result[4]);
	moveClock = parseInt(result[5]);

	let castle = result[2];
	if (castle.includes("K"))
		K = true;
	if (castle.includes("Q"))
		Q = true;
	if (castle.includes("k"))
		k = true;
	if (castle.includes("q"))
		q = true;

	let ep = result[3];
	if (ep != "-") {
		enPassant = SANToNotation(ep);
	}

	let lines = result[0].split("/");

	for (let i = 0; i < 8; i++) {
		let cur = 2;
		let curRank = lines[i];
		for (let j = 0; j < curRank.length; j++) {
			if (NUMBERS.includes(curRank[j])) {
				cur += NUMBERS.indexOf(curRank[j]);
				continue;
			}

			board[i * 12 + cur + 24] = PIECES.indexOf(curRank[j]);
			cur++;
		}
	}

	for (let i = 2; i < 10; i++) {
		for (let j = 2; j < 10; j++) {
			let index = i * 12 + j;
			if (board[index] == -1) {
				board[index] = 0;
				continue;
			}

			if (PIECES[board[index]] == "K")
				king[0] = index;
			if (PIECES[board[index]] == "k")
				king[1] = index;
		}
	}

	if (turn) {
		document.getElementById("pgn").textContent = moveClock + "...";
		moveClock++;
	}
}

function encode() {
	let fen = "";
	for (let i = 2; i < 10; i++) {
		let empty = 0;
		for (let j = 2; j < 10; j++) {
			if (board[i * 12 + j] == 0)
				empty++;
			else {
				if (empty != 0)
					fen += empty;
				empty = 0;
				fen += PIECES[board[i * 12 + j]];
			}
		}

		if (empty != 0)
			fen += empty;

		if (i != 9)
			fen += "/";
	}
	fen += " ";

	fen += turn ? 'b' : 'w';
	fen += " ";

	let castleRights = "";
	if (K)
		castleRights += "K";
	if (Q)
		castleRights += "Q";
	if (k)
		castleRights += "k";
	if (q)
		castleRights += "q";
	if (castleRights == "")
		castleRights = "-";
	fen += castleRights;
	fen += " ";

	if (enPassant == -1)
		fen += "-";
	else {
		let ep = notationToSAN(enPassant);
		fen += ep;
	}
	fen += ` ${fiftyMoveClock} ${moveClock}`;

	return fen;
}

function validFEN(fen) {
	fen = fen.trim();

	let parts = fen.split(" ");
	if (parts.length != 6)
		return false;
	
	let ranks = parts[0].split("/");
	if (ranks.length != 8)
		return false;

	for (let line of ranks) {
		let length = 0;
		for (let char of line) {
			if (NUMBERS.includes(char))
				length += parseInt(char);
			else {
				if (PIECES.includes(char))
					length++;
				else
					return false;
			}
		}
		if (length != 8)
			return false;
	}

	if (!["w", "b"].includes(parts[1]))
		return false;

	let castle = parts[2];
	let used = new Map();
	if (castle != "-") {
		for (let char of castle) {
			if (!["K", "Q", "k", "q"].includes(char) || used.get(char))
				return false;
			used.set(char, true);
		}
	}

	let ep = parts[3];
	if (ep != "-") {
		let file = ep[0];
		let rank = parseInt(ep[1]);

		if (!FILES.includes(file))
			return false;
		if (rank < 1 || rank > 8)
			return false;
	}

	for (let i = 4; i <= 5; i++) {
		if (Number.isNaN(Number(parts[i])))
			return false;
	}

	return true;
}

function notationToXY(coord) {
	if (coord == -1 || board[coord] == -1)
		return "-1";

	coord -= 26;
	coord -= Math.floor(coord / 12) * 4;

	let x = Math.floor(coord / 8).toString();
	let y = (coord % 8).toString();

	return x + y;
}

function notationToSAN(coord) {
	let xy = notationToXY(coord);

	let y = 8-NUMBERS.indexOf(xy[0]);
	let x = FILES[NUMBERS.indexOf(xy[1])];

	return x + y;
}

function SANToNotation(san) {
	let y = FILES.indexOf(san[0]) + 2;
	let x = 8 - parseInt(san[1]) + 2;
	return x * 12 + y;
}

function XYToNotation(xy) {
	return (parseInt(xy[1]) + 2) + parseInt(xy[0]) * 12 + 24;
}

function printBoard() {
	console.log("+---+---+---+---+---+---+---+---+");

	for (let i = 2; i < 10; i++) {
		let line = "|";
		for (let j = 2; j < 10; j++) {
			let index = i * 12 + j;
			let print = " ";

			if (board[index] != 0)
				print = PIECES[board[index]];

			line += ` ${print} |`;
		}
		console.log(line);
		console.log("+---+---+---+---+---+---+---+---+");
	}

	console.log(`FEN: ${encode()}`);

	console.log(`White Kingside: ${K}	   White Queenside: ${Q}`);
	console.log(`Black Kingside: ${k}	   Black Queenside: ${q}`);

	console.log(`En Passant Square: ${enPassant == -1 ? "None" : notationToSAN(enPassant)}`);
	console.log(`Turn: ${turn ? "Black" : "White"}`);
}