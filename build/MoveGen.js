import Attacked from "./Attacked.js";
import Board from "./Board.js";
import Moves from "./Moves.js";
import Pieces from "./Pieces.js";
import PieceMoves from "./PieceMoves.js";
var MoveGen;
(function (MoveGen) {
    function moveGen() {
        let moves = [];
        Attacked.getAttacked();
        if (Board.enPassant != -1) {
            let dir = 1;
            if (Board.turn)
                dir = -1;
            let center = Board.enPassant + (dir * 12);
            let pawn = Pieces.PIECES.indexOf(Board.turn ? "p" : "P");
            if (Board.board[center - 1] == pawn)
                moves.push(new Moves.Move(center - 1, Board.enPassant, Board.board[center], 0, 0, 0, 1));
            if (Board.board[center + 1] == pawn)
                moves.push(new Moves.Move(center + 1, Board.enPassant, Board.board[center], 0, 0, 0, 1));
        }
        for (let i of Board.pieces[Board.turn ? 1 : 0]) {
            let piece = Pieces.PIECES[Board.board[i]].toLowerCase();
            switch (piece) {
                case "q": {
                    let rook = PieceMoves.rmoves(i, false);
                    moves = moves.concat(rook);
                    let bishop = PieceMoves.bmoves(i);
                    moves = moves.concat(bishop);
                    break;
                }
                case "r": {
                    let rook = PieceMoves.rmoves(i, true);
                    moves = moves.concat(rook);
                    break;
                }
                case "b": {
                    let bishop = PieceMoves.bmoves(i);
                    moves = moves.concat(bishop);
                    break;
                }
                case "n": {
                    let knight = PieceMoves.nmoves(i);
                    moves = moves.concat(knight);
                    break;
                }
                case "p": {
                    let pawn = PieceMoves.pmoves(i);
                    moves = moves.concat(pawn);
                    break;
                }
                case "k": {
                    let king = PieceMoves.kmoves(i);
                    moves = moves.concat(king);
                    break;
                }
            }
        }
        let legal = [];
        for (let move of moves) {
            Moves.makeMove(move);
            Board.turn = !Board.turn;
            let kingLocation = Board.king[Board.turn ? 1 : 0];
            Attacked.getAttacked();
            if (!Attacked.attacked.has(kingLocation))
                legal.push(move);
            Board.turn = !Board.turn;
            Moves.unmakeMove(move);
        }
        return legal;
    }
    MoveGen.moveGen = moveGen;
})(MoveGen || (MoveGen = {}));
export default MoveGen;
