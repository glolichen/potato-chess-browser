function moveGen() {
	let out = Module.moveGen(encode());

	let legalMoves = [];
	for (let i = 0; i < out.size(); i++)
		legalMoves.push(out.get(i));

	if (legalMoves.length == 0) {
		gameOver = true;
		document.getElementById("resultText").textContent = Module.getAttacked(encode()) ? "Checkmate" : "Stalemate";
		setTimeout(() => document.getElementById("result").setAttribute("open", ""), 100);
	}

	return legalMoves;
}
function moveToUCI(move) {
	let asString = "";
    asString += notationToSAN(move.source);
    asString += notationToSAN(move.dest);
    if (move.promote != 0)
        asString += PIECES[move.promote].toLowerCase();
    return asString;
}

class Move {
    constructor(source, dest, castle, promote, isEp) { 
        this.source = source;
        this.dest = dest;
        this.castle = castle;
        this.promote = promote;
        this.isEp = isEp;
    }
}