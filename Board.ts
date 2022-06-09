import Constants from "./Constants.js";
import Pieces from "./Pieces.js";

namespace Board {
    export var board: number[] = [];
    export var king: number[] = [];

    export var enPassant: number = -1;

    export var pieces: number[][] = [[], []];

    export var K: boolean = false; // White Kingside
    export var Q: boolean = false; // White Queenside
    export var k: boolean = false; // Black Kingside
    export var q: boolean = false; // Black Queenside

    export var turn: boolean = false; // false = White, true = Black

    export var halfMoveClock: number = 0;

    export function decode(fen: string): void {
        board.length = 144;
        board.fill(-1);

        king.length = 2;

        let result: string[] = fen.split(" ");

        turn = result[1] == "b";

        halfMoveClock = parseInt(result[4]);

        let castle: string = result[2];
        if (castle.includes("K"))
            K = true;
        if (castle.includes("Q"))
            Q = true;
        if (castle.includes("k"))
            k = true;
        if (castle.includes("q"))
            q = true;

        let ep: string = result[3];
        if (ep != "-") {
            enPassant = SANToNotation(ep);
        }

        let lines: string[] = result[0].split("/");

        for (let i = 0; i < 8; i++) {
            let cur = 2;
            let curRank: string = lines[i];
            for (let j = 0; j < curRank.length; j++) {
                if (Constants.NUMBERS.includes(curRank[j])) {
                    cur += Constants.NUMBERS.indexOf(curRank[j]);
                    continue;
                }

                board[i * 12 + cur + 24] = Pieces.PIECES.indexOf(curRank[j]);
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

                let color = Pieces.getColor(board[index]) ? 1 : 0;

                if (Pieces.PIECES[board[index]] == "K")
                    king[0] = index;
                if (Pieces.PIECES[board[index]] == "k")
                    king[1] = index;

                pieces[color].push(index);
            }
        }
    }

    export function notationToXY(coord: number): string {
        if (coord == -1 || board[coord] == -1)
            return "-1";

        coord -= 26;
        coord -= Math.floor(coord / 12) * 4;

        let x: string = Math.floor(coord / 8).toString();
        let y: string = (coord % 8).toString();

        return x + y;
    }

    export function notationToSAN(coord: number): string {
        let xy: string = notationToXY(coord);

        let y: number = 8-Constants.NUMBERS.indexOf(xy[0]);
        let x: string = Constants.FILES[Constants.NUMBERS.indexOf(xy[1])];

        return x + y;
    }

    export function SANToNotation(san: string): number {
        let y: number = Constants.FILES.indexOf(san[0]) + 2;
        let x: number = 8 - parseInt(san[1]) + 2;
        return x * 12 + y;
    }

    export function XYToNotation(xy: string): number {
        return (parseInt(xy[1]) + 2) + parseInt(xy[0]) * 12 + 24;
    }

    export function print(): void {
        console.log("+---+---+---+---+---+---+---+---+");

        for (let i = 2; i < 10; i++) {
            let line: string = "|";
            for (let j = 2; j < 10; j++) {
                let index = i * 12 + j;
                let print: string = " ";

                if (board[index] != 0)
                    print = Pieces.PIECES[board[index]];

                line += ` ${print} |`;
            }
            console.log(line);
            console.log("+---+---+---+---+---+---+---+---+");
        }

        console.log(`White Kingside: ${K}       White Queenside: ${Q}`);
        console.log(`Black Kingside: ${k}       Black Queenside: ${q}`);

        console.log(`En Passant Square: ${enPassant == -1 ? "None" : notationToSAN(enPassant)}`);
        console.log(`Turn: ${turn ? "Black" : "White"}`);
    }
}

export default Board;