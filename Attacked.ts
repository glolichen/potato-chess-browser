import Board from "./Board.js";
import Pieces from "./Pieces.js";

namespace Attacked {
    export var attacked: Set<number> = new Set();

    export function getAttacked(): void {
        attacked.clear();
    
        for (let index of Board.pieces[!Board.turn ? 1 : 0]) {
            // console.log(index);
            let piece: string = Pieces.PIECES[Board.board[index]].toLowerCase();

            switch (piece) {
                case "q":
                    rook(index);
                    bishop(index);
                    break;
                case "r":
                    rook(index);
                    break;
                case "b":
                    bishop(index);
                    break;
                case "n":
                    knight(index);
                    break;
                case "p":
                    pawn(index);
                    break;
                case "k":
                    king(index);
                    break;
            }
        }
    }

    export function pawn(loc: number): void {
        for (let offset of Pieces.Offsets.PAWN_CAPTURE[!Board.turn ? 1 : 0]) {
            let i: number = loc + offset;
            if (Board.board[i] == -1)
                continue;
            attacked.add(i);
        }
    }

    export function knight(loc: number): void {
        for (let offset of Pieces.Offsets.KNIGHT) {
            let i: number = loc + offset;
            if (Board.board[i] == -1)
                continue;

            attacked.add(i);
        }
    }

    export function bishop(loc: number): void {
        let opponentKing: number = Pieces.PIECES.indexOf(Board.turn ? "k" : "K");
        for (let offset of Pieces.Offsets.BISHOP) {
            let i: number = loc + offset;
            while (Board.board[i] != -1) {
                attacked.add(i);

                if (Board.board[i] && Board.board[i] != opponentKing)
                    break;

                i += offset;
            }
        }
    }

    export function rook(loc: number): void {
        let opponentKing: number = Pieces.PIECES.indexOf(Board.turn ? "k" : "K");
        for (let offset of Pieces.Offsets.ROOK) {
            let i: number = loc + offset;
            while (Board.board[i] != -1) {
                attacked.add(i);

                if (Board.board[i] && Board.board[i] != opponentKing)
                    break;

                i += offset;
            }
        }
    }

    export function king(loc: number): void {
        for (let offset of Pieces.Offsets.KING) {
            let i: number = loc + offset;
            if (Board.board[i] == -1)
                continue;

            attacked.add(i);
        }
    }
}

export default Attacked;