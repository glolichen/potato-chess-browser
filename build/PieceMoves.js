import Attacked from "./Attacked.js";
import Board from "./Board.js";
import Constants from "./Constants.js";
import Moves from "./Moves.js";
import Pieces from "./Pieces.js";
var PieceMoves;
(function (PieceMoves) {
    function pmoves(loc) {
        let moves = [];
        let starting = false;
        if (Board.turn)
            starting = (Math.floor(loc / 12) == 3);
        else
            starting = (Math.floor(loc / 12) == 8);
        let offset = loc + Pieces.Offsets.PAWN_MOVE[Board.turn ? 1 : 0][0];
        if (Board.board[offset] == 0) {
            if (Board.turn && Math.floor(offset / 12) == 9) {
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("q"), 0, 0));
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("r"), 0, 0));
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("b"), 0, 0));
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("n"), 0, 0));
            }
            else if (!Board.turn && Math.floor(offset / 12) == 2) {
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("Q"), 0, 0));
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("R"), 0, 0));
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("B"), 0, 0));
                moves.push(new Moves.Move(loc, offset, 0, 0, Constants.SIGNALS.indexOf("N"), 0, 0));
            }
            else
                moves.push(new Moves.Move(loc, offset, 0, 0, 0, 0, 0));
            if (starting && Board.board[loc + Pieces.Offsets.PAWN_START[Board.turn ? 1 : 0][0]] == 0)
                moves.push(new Moves.Move(loc, loc + Pieces.Offsets.PAWN_START[Board.turn ? 1 : 0][0], 0, 0, 0, 0, 0));
        }
        for (let offset2 of Pieces.Offsets.PAWN_CAPTURE[Board.turn ? 1 : 0]) {
            let i = loc + offset2;
            if (!Board.board[i] || Board.board[i] == -1)
                continue;
            let turn = Board.turn;
            let pieceColor = Pieces.getColor(Board.board[i]);
            if (pieceColor != turn) {
                if (Board.turn && Math.floor(i / 12) == 9) {
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("q"), 0, 0));
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("r"), 0, 0));
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("b"), 0, 0));
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("n"), 0, 0));
                }
                else if (!Board.turn && Math.floor(i / 12) == 2) {
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("Q"), 0, 0));
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("R"), 0, 0));
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("B"), 0, 0));
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, Constants.SIGNALS.indexOf("N"), 0, 0));
                }
                else
                    moves.push(new Moves.Move(loc, i, Board.board[i], 0, 0, 0, 0));
            }
        }
        return moves;
    }
    PieceMoves.pmoves = pmoves;
    function nmoves(loc) {
        let moves = [];
        for (let offset of Pieces.Offsets.KNIGHT) {
            let i = loc + offset;
            if (Board.board[i] == -1)
                continue;
            if (Board.board[i] != 0 && Pieces.getColor(Board.board[i]) == Board.turn)
                continue;
            moves.push(new Moves.Move(loc, i, Board.board[i], 0, 0, 0, 0));
        }
        return moves;
    }
    PieceMoves.nmoves = nmoves;
    function bmoves(loc) {
        let moves = [];
        for (let offset of Pieces.Offsets.BISHOP) {
            let i = loc + offset;
            while (Board.board[i] != -1) {
                if (Board.board[i] != 0 && Pieces.getColor(Board.board[i]) == Board.turn)
                    break;
                moves.push(new Moves.Move(loc, i, Board.board[i], 0, 0, 0, 0));
                if (Board.board[i])
                    break;
                i += offset;
            }
        }
        return moves;
    }
    PieceMoves.bmoves = bmoves;
    function rmoves(loc, isRook) {
        let moves = [];
        for (let offset of Pieces.Offsets.ROOK) {
            let i = loc + offset;
            while (Board.board[i] != -1) {
                if (Board.board[i] != 0 && Pieces.getColor(Board.board[i]) == Board.turn)
                    break;
                let signal = 0;
                if (isRook) {
                    if (Board.turn == Constants.BLACK) {
                        if (Board.k && (loc % 12 == 9))
                            signal = Constants.SIGNALS.indexOf("k");
                        if (Board.q && (loc % 12 == 2))
                            signal = Constants.SIGNALS.indexOf("q");
                    }
                    else {
                        if (Board.K && (loc % 12 == 9))
                            signal = Constants.SIGNALS.indexOf("K");
                        if (Board.Q && (loc % 12 == 2))
                            signal = Constants.SIGNALS.indexOf("Q");
                    }
                }
                moves.push(new Moves.Move(loc, i, Board.board[i], 0, 0, signal, 0));
                if (Board.board[i])
                    break;
                i += offset;
            }
        }
        return moves;
    }
    PieceMoves.rmoves = rmoves;
    function kmoves(loc) {
        let moves = [];
        for (let offset of Pieces.Offsets.KING) {
            let i = loc + offset;
            if (Board.board[i] == -1)
                continue;
            if (Pieces.getColor(Board.board[i]) == Board.turn && Board.board[i] != 0)
                continue;
            let contains = false;
            for (let c of Attacked.attacked) {
                if (c == i) {
                    contains = true;
                    break;
                }
            }
            if (contains)
                continue;
            let signal = 0;
            if (Board.turn) {
                if (Board.k && Board.q)
                    signal = Constants.SIGNALS.indexOf("x");
                else if (Board.k)
                    signal = Constants.SIGNALS.indexOf("k");
                else if (Board.q)
                    signal = Constants.SIGNALS.indexOf("q");
            }
            else {
                if (Board.K && Board.Q)
                    signal = Constants.SIGNALS.indexOf("X");
                else if (Board.K)
                    signal = Constants.SIGNALS.indexOf("K");
                else if (Board.Q)
                    signal = Constants.SIGNALS.indexOf("Q");
            }
            moves.push(new Moves.Move(loc, i, Board.board[i], 0, 0, signal, 0));
        }
        for (let c of Attacked.attacked) {
            if (c == Board.king[Board.turn ? 1 : 0])
                return moves;
        }
        if (Board.turn == Constants.WHITE) {
            let KLegal = true;
            let QLegal = true;
            for (let c of Attacked.attacked) {
                if (c == 115 || c == 116)
                    KLegal = false;
                if (c == 113 || c == 112)
                    QLegal = false;
            }
            if (Board.K && KLegal && Board.board[114] == Pieces.PIECES.indexOf("K") &&
                Board.board[117] == Pieces.PIECES.indexOf("R") && Board.board[115] == 0 && Board.board[116] == 0) {
                let signal = 0;
                if (Board.K && Board.Q)
                    signal = Constants.SIGNALS.indexOf("X");
                else if (Board.K)
                    signal = Constants.SIGNALS.indexOf("K");
                else if (Board.Q)
                    signal = Constants.SIGNALS.indexOf("Q");
                moves.push(new Moves.Move(114, 116, 0, 1, 0, signal, 0));
            }
            if (Board.Q && QLegal && Board.board[114] == Pieces.PIECES.indexOf("K") && Board.board[110] == Pieces.PIECES.indexOf("R") &&
                Board.board[113] == 0 && Board.board[112] == 0 && Board.board[111] == 0) {
                let signal = 0;
                if (Board.K && Board.Q)
                    signal = Constants.SIGNALS.indexOf("X");
                else if (Board.K)
                    signal = Constants.SIGNALS.indexOf("K");
                else if (Board.Q)
                    signal = Constants.SIGNALS.indexOf("Q");
                moves.push(new Moves.Move(114, 112, 0, 2, 0, signal, 0));
            }
        }
        else {
            let KLegal = true;
            let QLegal = true;
            for (let c of Attacked.attacked) {
                if (c == 31 || c == 32)
                    KLegal = false;
                if (c == 29 || c == 28)
                    QLegal = false;
            }
            if (Board.k && KLegal && Board.board[30] == Pieces.PIECES.indexOf("k") &&
                Board.board[33] == Pieces.PIECES.indexOf("r") && Board.board[31] == 0 && Board.board[32] == 0) {
                let signal = 0;
                if (Board.k && Board.q)
                    signal = Constants.SIGNALS.indexOf("x");
                else if (Board.k)
                    signal = Constants.SIGNALS.indexOf("k");
                else if (Board.q)
                    signal = Constants.SIGNALS.indexOf("q");
                moves.push(new Moves.Move(30, 32, 0, 1, 0, signal, 0));
            }
            if (Board.Q && QLegal && Board.board[30] == Pieces.PIECES.indexOf("k") && Board.board[26] == Pieces.PIECES.indexOf("r") &&
                Board.board[29] == 0 && Board.board[28] == 0 && Board.board[27] == 0) {
                let signal = 0;
                if (Board.k && Board.q)
                    signal = Constants.SIGNALS.indexOf("x");
                else if (Board.k)
                    signal = Constants.SIGNALS.indexOf("k");
                else if (Board.q)
                    signal = Constants.SIGNALS.indexOf("q");
                moves.push(new Moves.Move(30, 28, 0, 2, 0, signal, 0));
            }
        }
        return moves;
    }
    PieceMoves.kmoves = kmoves;
})(PieceMoves || (PieceMoves = {}));
export default PieceMoves;
