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

        let paragraph = document.createElement("p");
        paragraph.textContent = inCheck ? "Checkmate" : "Stalemate";
        
        let button = document.createElement("button");
        button.onclick = () => {
            result.removeAttribute("open");
        }
        button.textContent = "OK";

        result.append(paragraph);
        result.append(button);

		setTimeout(() => result.setAttribute("open", ""), 100);
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

function charToPiece(char) {
    let piece = PIECES.indexOf(String.fromCharCode(char));
    return piece == -1 ? 0 : piece;
}

function isLight(notation) {
    let coord = notationToXY(notation);
    return (parseInt(coord[0]) + parseInt(coord[1])) % 2 == 0;
}

