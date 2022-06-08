import Board from "./Board.js";
import MoveGen from "./MoveGen.js";
import Moves from "./Moves.js";
import Pieces from "./Pieces.js";

Board.decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); 

var height = window.innerHeight ||
document.getElementsByTagName("html")[0].clientHeight  ||
document.getElementsByTagName("body")[0].clientHeight  ||
screen.availHeight;

const SIZE: number = Math.floor(height * 0.7 / 8);

const DARK = "#b58863";
const LIGHT = "#f0d9b5";

const SEL_DARK = "#e0c434";
const SEL_LIGHT = "#f8ec5c";

var rows: HTMLTableRowElement[] = [];
var current = 0;

var selected: number | null = null;

var moves: Moves.Move[] = MoveGen.moveGen();
var movesFromSelected: Moves.Move[] = [];

document.getElementById("perft")!.onclick = () => window.open("./perft.html");

function init(): void {   
    for (let i = 0; i < 8; i++) {
        rows[i] = document.createElement("tr");
        for (let j = 0; j < 4; j++) {
            const td1: HTMLTableCellElement = document.createElement("td");
            const td2: HTMLTableCellElement = document.createElement("td");

            const light: HTMLDivElement = document.createElement("div");

            let notation: number = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            light.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${LIGHT};`);
            light.className = "square";
            light.id = notation.toString();
            light.onclick = () => {
                click(parseInt(light.id));
            }
            if (Board.board[notation] > 0) {
                const image: HTMLImageElement = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
                image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
                image.className = "piece";
                light.appendChild(image);
            }

            current++;

            const dark: HTMLDivElement = document.createElement("div");

            notation = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            dark.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${DARK};`);
            dark.className = "square";
            dark.id = notation.toString();
            dark.onclick = function() {
                click(parseInt(dark.id));
            }
            if (Board.board[notation] > 0) {
                const image: HTMLImageElement = document.createElement("img");
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
            const td1: HTMLTableCellElement = document.createElement("td");
            const td2: HTMLTableCellElement = document.createElement("td");

            const dark: HTMLDivElement = document.createElement("div");

            let notation: number = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            dark.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${DARK};`);
            dark.className = "square";
            dark.id = notation.toString();
            dark.onclick = () => {
                click(parseInt(dark.id));
            }
            if (Board.board[notation] > 0) {
                const image: HTMLImageElement = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
                image.src = `./Assets/${Pieces.getImage(Board.board[notation])}.png`;
                image.className = "piece";
                dark.appendChild(image);
            }

            current++;

            const light: HTMLDivElement = document.createElement("div");

            notation = Board.XYToNotation(`${Math.floor(current / 8)}${current % 8}`);
            light.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${LIGHT};`);
            light.className = "square";
            light.id = notation.toString();
            light.onclick = () => {
                click(parseInt(light.id));
            }
            if (Board.board[notation] > 0) {
                const image: HTMLImageElement = document.createElement("img");
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

function update(): void {
    for (let piece of document.querySelectorAll(".piece"))
        piece.remove();

    for (let i = 0; i < 144; i++) {
        if (Board.board[i] <= 0)
            continue;

        const image: HTMLImageElement = document.createElement("img");
        image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px;`);
        image.src = `./Assets/${Pieces.getImage(Board.board[i])}.png`;
        image.className = "piece";
        document.getElementById(i.toString())?.appendChild(image);
    }
}    

function click(current: number): void {
    if (selected == null) {
        if (Board.board[current] > 0 && Pieces.getColor(Board.board[current]) == Board.turn) {
            document.getElementById(current.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(current) ? SEL_LIGHT : SEL_DARK}`);
            selected = current;
            movesFromSelected.length = 0;
            for (let move of moves) {
                if (move.source == current) {
                    movesFromSelected.push(move);
                    const image: HTMLImageElement = document.createElement("img");
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
            Moves.makeMove(move);
            document.getElementById(selected.toString())?.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; background-color: ${isLight(selected) ? LIGHT : DARK}`);
            selected = null;
            for (let piece of document.querySelectorAll(".highlight"))
                piece.remove();
            update();
            movesFromSelected.length = 0;
            moves = MoveGen.moveGen();
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
                const image: HTMLImageElement = document.createElement("img");
                image.setAttribute("style", `width: ${SIZE}px; height: ${SIZE}px; opacity: 60%`);
                image.src = Board.board[move.dest] > 0 ? "./Assets/capture.png" : "./Assets/highlight.png";
                image.className = "highlight";
                document.getElementById(move.dest.toString())?.appendChild(image);
            }
        }
    }
    else
        selected = null;
}

function isLight(notation: number): boolean {
    let coord: string = Board.notationToXY(notation);
    return (parseInt(coord[0]) + parseInt(coord[1])) % 2 == 0;
}

init();