import Board from "./Board.js";
import Pieces from "./Pieces.js";
var Attacked;
(function (Attacked) {
    Attacked.attacked = new Set();
    function getAttacked() {
        Attacked.attacked.clear();
        for (let index of Board.pieces[!Board.turn ? 1 : 0]) {
            let piece = Pieces.PIECES[Board.board[index]].toLowerCase();
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
    Attacked.getAttacked = getAttacked;
    function pawn(loc) {
        for (let offset of Pieces.Offsets.PAWN_CAPTURE[!Board.turn ? 1 : 0]) {
            let i = loc + offset;
            if (Board.board[i] == -1)
                continue;
            Attacked.attacked.add(i);
        }
    }
    Attacked.pawn = pawn;
    function knight(loc) {
        for (let offset of Pieces.Offsets.KNIGHT) {
            let i = loc + offset;
            if (Board.board[i] == -1)
                continue;
            Attacked.attacked.add(i);
        }
    }
    Attacked.knight = knight;
    function bishop(loc) {
        let opponentKing = Pieces.PIECES.indexOf(Board.turn ? "k" : "K");
        for (let offset of Pieces.Offsets.BISHOP) {
            let i = loc + offset;
            while (Board.board[i] != -1) {
                Attacked.attacked.add(i);
                if (Board.board[i] && Board.board[i] != opponentKing)
                    break;
                i += offset;
            }
        }
    }
    Attacked.bishop = bishop;
    function rook(loc) {
        let opponentKing = Pieces.PIECES.indexOf(Board.turn ? "k" : "K");
        for (let offset of Pieces.Offsets.ROOK) {
            let i = loc + offset;
            while (Board.board[i] != -1) {
                Attacked.attacked.add(i);
                if (Board.board[i] && Board.board[i] != opponentKing)
                    break;
                i += offset;
            }
        }
    }
    Attacked.rook = rook;
    function king(loc) {
        for (let offset of Pieces.Offsets.KING) {
            let i = loc + offset;
            if (Board.board[i] == -1)
                continue;
            Attacked.attacked.add(i);
        }
    }
    Attacked.king = king;
})(Attacked || (Attacked = {}));
export default Attacked;
