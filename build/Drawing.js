import Attacked from "./Attacked.js";
import Board from "./Board.js";
import MoveGen from "./MoveGen.js";
import Moves from "./Moves.js";
import Pieces from "./Pieces.js";
Board.decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
console.clear();
var height = window.innerHeight ||
    document.getElementsByTagName("html")[0].clientHeight ||
    document.getElementsByTagName("body")[0].clientHeight ||
    screen.availHeight;
var paused = true;
var humanSide = false;
const SIZE = Math.floor(height * 0.7 / 8);
const DARK = "#b58863";
const LIGHT = "#f0d9b5";
const SEL_DARK = "#e0c434";
const SEL_LIGHT = "#f8ec5c";
var rows = [];
var selected = null;
var moves = moveGen();
var movesFromSelected = [];
document.getElementById("perft").onclick = () => window.open("./perft.html");
function moveGen() {
    let moves = MoveGen.moveGen();
    if (moves.length == 0) {
        Attacked.getAttacked();
        let inCheck = Attacked.attacked.has(Board.king[Board.turn ? 1 : 0]);
        let result = document.getElementById("result");
        let paragraph = document.createElement("p");
        paragraph.textContent = inCheck ? "Checkmate" : "Stalemate";
        let button = document.createElement("button");
        button.onclick = () => {
            result.removeAttribute("open");
        };
        button.textContent = "OK";
        result.append(paragraph);
        result.append(button);
        result.setAttribute("open", "");
    }
    return moves;
}
function init() {
    let temp = document.getElementById("temp");
    temp.parentNode?.removeChild(temp);
    let current = 0;
    document.getElementById("table").innerHTML = "";
    for (let i = 0; i < 8; i++) {
        rows[i] = document.createElement("tr");
        for (let j = 0; j < 4; j++) {
            const td1 = document.createElement("td");
            const td2 = document.createElement("td");
            const light = document.createElement("div");
            let notation = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            if (humanSide == true)
                notation = Board.XYToNotation(`${Math.floor((63 - current) / 8)}${(63 - current) % 8}`);
            light.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${LIGHT};`);
            light.className = "square";
            light.id = notation.toString();
            light.onclick = () => {
                click(parseInt(light.id));
            };
            if (Board.board[notation] > 0) {
                const image = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
                image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
                image.className = "piece";
                light.appendChild(image);
            }
            current++;
            const dark = document.createElement("div");
            notation = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            if (humanSide == true)
                notation = Board.XYToNotation(`${Math.floor((63 - current) / 8)}${(63 - current) % 8}`);
            dark.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${DARK};`);
            dark.className = "square";
            dark.id = notation.toString();
            dark.onclick = function () {
                click(parseInt(dark.id));
            };
            if (Board.board[notation] > 0) {
                const image = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
                image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
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
            let notation = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            if (humanSide == true)
                notation = Board.XYToNotation(`${Math.floor((63 - current) / 8)}${(63 - current) % 8}`);
            dark.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${DARK};`);
            dark.className = "square";
            dark.id = notation.toString();
            dark.onclick = () => {
                click(parseInt(dark.id));
            };
            if (Board.board[notation] > 0) {
                const image = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
                image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
                image.className = "piece";
                dark.appendChild(image);
            }
            current++;
            const light = document.createElement("div");
            notation = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            if (humanSide == true)
                notation = Board.XYToNotation(`${Math.floor((63 - current) / 8)}${(63 - current) % 8}`);
            light.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${LIGHT};`);
            light.className = "square";
            light.id = notation.toString();
            light.onclick = () => {
                click(parseInt(light.id));
            };
            if (Board.board[notation] > 0) {
                const image = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
                image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
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
    for (let row of rows) {
        document.getElementById("table")?.appendChild(row);
    }
}
function update() {
    for (let piece of document.querySelectorAll(".piece"))
        piece.remove();
    for (let i = 0; i < 144; i++) {
        let notation = i;
        if (humanSide)
            notation = 143 - i;
        if (Board.board[notation] <= 0)
            continue;
        const image = document.createElement("img");
        image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
        image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
        image.className = "piece";
        document.getElementById(notation.toString())?.appendChild(image);
    }
}
function click(current) {
    if (paused)
        return;
    if (selected == null) {
        if (Board.board[current] > 0 && Pieces.getColor(Board.board[current]) == Board.turn) {
            document.getElementById(current.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(current) ? SEL_LIGHT : SEL_DARK}`);
            selected = current;
            movesFromSelected.length = 0;
            for (let move of moves) {
                if (move.source == current) {
                    movesFromSelected.push(move);
                    if (move.promote != 0 && move.promote != 3 && move.promote != 5)
                        continue;
                    const image = document.createElement("img");
                    image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 40%`);
                    image.src = Board.board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
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
            Moves.makeMove(move);
            document.getElementById(selected.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(selected) ? LIGHT : DARK}`);
            selected = null;
            for (let piece of document.querySelectorAll(".highlight"))
                piece.remove();
            update();
            movesFromSelected.length = 0;
            moves = moveGen();
            return;
        }
    }
    document.getElementById(selected.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(selected) ? LIGHT : DARK}`);
    for (let piece of document.querySelectorAll(".highlight"))
        piece.remove();
    if (Board.board[current] > 0 && Pieces.getColor(Board.board[current]) == Board.turn) {
        document.getElementById(current.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(current) ? SEL_LIGHT : SEL_DARK}`);
        selected = current;
        movesFromSelected.length = 0;
        for (let move of moves) {
            if (move.source == current) {
                movesFromSelected.push(move);
                if (move.promote != 0 && move.promote != 3 && move.promote != 5)
                    continue;
                const image = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 40%`);
                image.src = Board.board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
                image.className = "highlight";
                document.getElementById(move.dest.toString())?.appendChild(image);
            }
        }
    }
    else
        selected = null;
}
function isLight(notation) {
    let coord = Board.notationToXY(notation);
    return (parseInt(coord[0]) + parseInt(coord[1])) % 2 == 0;
}
function pickSide() {
    const whiteButton = document.createElement("button");
    whiteButton.onclick = () => {
        paused = false;
        colorDialog.removeAttribute("open");
        init();
    };
    whiteButton.textContent = "White";
    const blackButton = document.createElement("button");
    blackButton.onclick = () => {
        humanSide = true;
        paused = false;
        colorDialog.removeAttribute("open");
        init();
    };
    blackButton.textContent = "Black";
    const colorDialog = document.getElementById("color");
    colorDialog.appendChild(whiteButton);
    colorDialog.appendChild(blackButton);
    colorDialog.setAttribute("open", "");
}
function pickPromotion(square) {
    paused = true;
    const queen = document.createElement("button");
    queen.onclick = () => {
        paused = false;
        Board.board[square] = Pieces.getColor(Board.board[square]) ? Pieces.PIECES.indexOf("q") : Pieces.PIECES.indexOf("Q");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    };
    queen.textContent = "Queen";
    const rook = document.createElement("button");
    rook.onclick = () => {
        paused = false;
        Board.board[square] = Pieces.getColor(Board.board[square]) ? Pieces.PIECES.indexOf("r") : Pieces.PIECES.indexOf("R");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    };
    rook.textContent = "Rook";
    const bishop = document.createElement("button");
    bishop.onclick = () => {
        paused = false;
        Board.board[square] = Pieces.getColor(Board.board[square]) ? Pieces.PIECES.indexOf("b") : Pieces.PIECES.indexOf("B");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    };
    bishop.textContent = "Bishop";
    const knight = document.createElement("button");
    knight.onclick = () => {
        paused = false;
        Board.board[square] = Pieces.getColor(Board.board[square]) ? Pieces.PIECES.indexOf("n") : Pieces.PIECES.indexOf("N");
        update();
        moves = moveGen();
        promotionDialog.removeAttribute("open");
    };
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
