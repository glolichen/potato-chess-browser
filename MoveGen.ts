import Attacked from "./Attacked.js";
import Board from "./Board.js";
import Moves from "./Moves.js";
import Pieces from "./Pieces.js";
import PieceMoves from "./PieceMoves.js";

namespace MoveGen {
    export function moveGen(): Moves.Move[] {
        let moves: Moves.Move[] = [];

        Attacked.getAttacked();

        if (Board.enPassant != -1) {
            let dir: number = 1;
            if (Board.turn)
                dir = -1;

            let center: number = Board.enPassant + (dir * 12);
            let pawn: number = Pieces.PIECES.indexOf(Board.turn ? "p" : "P");

            if (Board.board[center - 1] == pawn)
                moves.push(new Moves.Move(center - 1, Board.enPassant, Board.board[center], 0, 0, 0, 1));
            if (Board.board[center + 1] == pawn)
                moves.push(new Moves.Move(center + 1, Board.enPassant, Board.board[center], 0, 0, 0, 1));
        }

        for (let i of Board.pieces[Board.turn ? 1 : 0]) {
            let piece: string = Pieces.PIECES[Board.board[i]].toLowerCase();

            switch (piece) {
                case "q": {
                    let rook: Moves.Move[] = PieceMoves.rmoves(i, false);
                    moves = moves.concat(rook);
                    let bishop: Moves.Move[] = PieceMoves.bmoves(i);
                    moves = moves.concat(bishop);
                    break;
                }

                case "r": {
                    let rook: Moves.Move[] = PieceMoves.rmoves(i, true);
                    moves = moves.concat(rook);
                    break;
                }

                case "b": {
                    let bishop: Moves.Move[] = PieceMoves.bmoves(i);
                    moves = moves.concat(bishop);
                    break;
                }

                case "n": {
                    let knight: Moves.Move[] = PieceMoves.nmoves(i);
                    moves = moves.concat(knight);
                    break;
                }

                case "p": {
                    let pawn: Moves.Move[] = PieceMoves.pmoves(i);
                    moves = moves.concat(pawn);
                    break;
                }

                case "k": {
                    let king: Moves.Move[] = PieceMoves.kmoves(i);
                    moves = moves.concat(king);
                    break;
                }
            }
        }

        let legal: Moves.Move[] = [];
        for (let move of moves) {
            Moves.makeMove(move);

            Board.turn = !Board.turn;
            let kingLocation: number = Board.king[Board.turn ? 1 : 0];

            Attacked.getAttacked();
            if (!Attacked.attacked.has(kingLocation))
                legal.push(move);

            Board.turn = !Board.turn;
            Moves.unmakeMove(move);
        }

        return legal;
    }
}

export default MoveGen;