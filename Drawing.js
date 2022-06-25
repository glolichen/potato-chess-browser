// decode("4k3/4r3/8/b7/7q/2P5/4RP2/1Q2K3 w - - 0 1");
decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); 
console.clear();

// THIS POSITION DOES NOT WORK 8/8/8/5k2/4Pp2/8/8/4K3 b - e3 0 1
// en passant out of check is not recognized

var height = window.innerHeight ||
	document.getElementsByTagName("html")[0].clientHeight  ||
	document.getElementsByTagName("body")[0].clientHeight  ||
	screen.availHeight;

var gameOver = true;

var humanSide = false;

const SIZE = Math.floor(height * 0.08);

const DARK = "#b58863";
const LIGHT = "#f0d9b5";

const SEL_DARK = "#e0c434";
const SEL_LIGHT = "#f8ec5c";

const LAST_MOVE_DARK = "#aaa23a";
const LAST_MOVE_LIGHT = "#cdd26a";

var rows = [];

var selected = null;

var sel1 = null;
var sel2 = null;

var moves = [];
var movesFromSelected = [];

const TIME = 1500;

document.getElementById("perft").onclick = () => window.location.replace("./perft.html");

function moveGen() {
    Module["decode"](encode());
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

function charToPiece(char) {
    let piece = PIECES.indexOf(String.fromCharCode(char));
    return piece == -1 ? 0 : piece;
}

function init() {
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
    if (gameOver)
        return;

    if (selected == null) {
        if (board[current] > 0 && getColor(board[current]) == turn && getColor(board[current]) == humanSide) {
            document.getElementById(current.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(current) ? SEL_LIGHT : SEL_DARK}`);
            selected = current;
            movesFromSelected.length = 0;
            for (let move of moves) {
                if (move.source == current) {
                    movesFromSelected.push(move);
                    if (move.promote != 0 && move.promote != 3 && move.promote != 5)
                        continue;
                    const image = document.createElement("img");
                    image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 50%`);
                    image.src = board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
                    image.className = "highlight";
                    document.getElementById(move.dest.toString())?.appendChild(image);
                }
            }
        }
        return;
    }

    if (current == selected)
        return;

    for (let move of movesFromSelected) {
        if (move.dest == current) {
            move.source = selected;
            if (move.promote != 0) {
                move.promote = 0;
                pickPromotion(move.dest);
            }

            document.getElementById(selected.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; 
                background-color: ${isLight(selected) ? LIGHT : DARK}`);
            makeMove(move);
            selected = null;

            highlightLastMove(move);

            for (let piece of document.querySelectorAll(".highlight"))
                piece.remove();

            update();
            movesFromSelected.length = 0;

            const worker = new Worker("./SearchWorker.js");
            worker.postMessage([encode(), TIME]);
            worker.onmessage = e => {
                makeMove(e.data[0]);
                highlightLastMove(e.data[0]);

                document.getElementById("depth").innerHTML = `<b>Depth: ${e.data[1]} ${
                    e.data[3] ? `<span class="red">(Mate in ${Math.round(e.data[1]/2)} found)</span>` : ""}</b>`;
                document.getElementById("eval").innerHTML = `<b>Eval: ${e.data[2]}</b>`;

                update();
                moves = moveGen();
            }

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
                if (move.promote != 0 && move.promote != 3 && move.promote != 5)
                    continue;
                const image = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 50%`);
                image.src = board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
                image.className = "highlight";
                document.getElementById(move.dest.toString())?.appendChild(image);
            }
        }
    }
    else
        selected = null;
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

function isLight(notation) {
    let coord = notationToXY(notation);
    return (parseInt(coord[0]) + parseInt(coord[1])) % 2 == 0;
}

function pickSide() {
    const whiteButton = document.createElement("button");
    whiteButton.onclick = () => {
        gameOver = false;
        colorDialog.removeAttribute("open");
        moves = moveGen();
        init();
    }
    whiteButton.textContent = "White";

    const blackButton = document.createElement("button");
    blackButton.onclick = () => {
        humanSide = true;
        gameOver = false;
        colorDialog.removeAttribute("open");
        moves = moveGen();
        init();
    }
    blackButton.textContent = "Black";

    const colorDialog = document.getElementById("color");
    colorDialog.appendChild(whiteButton);
    colorDialog.appendChild(blackButton);
    colorDialog.setAttribute("open", "");
}

function pickPromotion(square) {
    gameOver = true;

    const queen = document.createElement("button");
    queen.onclick = () => {
        gameOver = false;
        board[square] = getColor(board[square]) ? PIECES.indexOf("q") : PIECES.indexOf("Q");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    }
    queen.textContent = "Queen";

    const rook = document.createElement("button");
    rook.onclick = () => {
        gameOver = false;
        board[square] = getColor(board[square]) ? PIECES.indexOf("r") : PIECES.indexOf("R");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    }
    rook.textContent = "Rook";

    const bishop = document.createElement("button");
    bishop.onclick = () => {
        gameOver = false;
        board[square] = getColor(board[square]) ? PIECES.indexOf("b") : PIECES.indexOf("B");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    }
    bishop.textContent = "Bishop";

    const knight = document.createElement("button");
    knight.onclick = () => {
        gameOver = false;
        board[square] = getColor(board[square]) ? PIECES.indexOf("n") : PIECES.indexOf("N");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    }
    knight.textContent = "Knight";

    const promotionDialog = document.getElementById("promotion");
    promotionDialog.innerHTML = "<p>Select promotion piece:</p>";
    promotionDialog.appendChild(queen);
    promotionDialog.appendChild(rook);
    promotionDialog.appendChild(bishop);
    promotionDialog.appendChild(knight);
    promotionDialog.setAttribute("open", "");
}

init();
pickSide();