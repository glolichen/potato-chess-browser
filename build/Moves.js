import Board from "./Board.js";
import Constants from "./Constants.js";
import Pieces from "./Pieces.js";
var Moves;
(function (Moves) {
    function del(side, square) {
        while (Board.pieces[side ? 1 : 0].includes(square)) {
            Board.pieces[side ? 1 : 0].splice(Board.pieces[side ? 1 : 0].indexOf(square), 1);
        }
    }
    Moves.del = del;
    function makeMove(move) {
        Board.enPassant = -1;
        Board.turn = !Board.turn;
        if (move.signal == Constants.SIGNALS.indexOf("X")) {
            Board.K = false;
            Board.Q = false;
        }
        if (move.signal == Constants.SIGNALS.indexOf("K"))
            Board.K = false;
        if (move.signal == Constants.SIGNALS.indexOf("Q"))
            Board.Q = false;
        if (move.signal == Constants.SIGNALS.indexOf("x")) {
            Board.k = false;
            Board.q = false;
        }
        if (move.signal == Constants.SIGNALS.indexOf("k"))
            Board.k = false;
        if (move.signal == Constants.SIGNALS.indexOf("q"))
            Board.q = false;
        if (move.castle) {
            castle(move.castle);
            return;
        }
        if (move.isEp) {
            Board.board[move.dest] = Board.board[move.source];
            Board.board[move.source] = 0;
            let dir = 1;
            if (Board.turn == Constants.WHITE)
                dir = -1;
            dir *= 12;
            Board.board[move.dest + dir] = 0;
            del(Board.turn, move.dest + dir);
            del(!Board.turn, move.source);
            Board.pieces[!Board.turn ? 1 : 0].push(move.dest);
            return;
        }
        if (move.promote) {
            Board.board[move.dest] = move.promote;
            Board.board[move.source] = 0;
            del(!Board.turn, move.source);
            del(Board.turn, move.dest);
            Board.pieces[!Board.turn ? 1 : 0].push(move.dest);
            return;
        }
        if (Board.turn && Board.board[move.source] == Pieces.PIECES.indexOf("P") && move.source - move.dest == 24) {
            if (Board.board[move.dest - 1] == Pieces.PIECES.indexOf("p"))
                Board.enPassant = move.dest + 12;
            if (Board.board[move.dest + 1] == Pieces.PIECES.indexOf("p"))
                Board.enPassant = move.dest + 12;
        }
        else if (!Board.turn && Board.board[move.source] == Pieces.PIECES.indexOf("p") && move.dest - move.source == 24) {
            if (Board.board[move.dest - 1] == Pieces.PIECES.indexOf("P"))
                Board.enPassant = move.dest - 12;
            if (Board.board[move.dest + 1] == Pieces.PIECES.indexOf("P"))
                Board.enPassant = move.dest - 12;
        }
        if (Pieces.PIECES[Board.board[move.source]].toLowerCase() == "k")
            Board.king[!Board.turn ? 1 : 0] = move.dest;
        del(!Board.turn, move.source);
        del(Board.turn, move.dest);
        Board.pieces[!Board.turn ? 1 : 0].push(move.dest);
        Board.board[move.dest] = Board.board[move.source];
        Board.board[move.source] = 0;
    }
    Moves.makeMove = makeMove;
    function castle(dir) {
        if (dir == 1) {
            if (Board.turn) {
                Board.board[114] = 0;
                Board.board[117] = 0;
                Board.board[116] = Pieces.PIECES.indexOf("K");
                Board.board[115] = Pieces.PIECES.indexOf("R");
                Board.king[0] = 116;
                del(Constants.WHITE, 114);
                del(Constants.WHITE, 117);
                Board.pieces[0].push(116);
                Board.pieces[0].push(115);
            }
            else {
                Board.board[30] = 0;
                Board.board[33] = 0;
                Board.board[32] = Pieces.PIECES.indexOf("k");
                Board.board[31] = Pieces.PIECES.indexOf("r");
                Board.king[1] = 32;
                del(Constants.BLACK, 30);
                del(Constants.BLACK, 33);
                Board.pieces[1].push(32);
                Board.pieces[1].push(31);
            }
        }
        else if (dir == 2) {
            if (Board.turn) {
                Board.board[114] = 0;
                Board.board[110] = 0;
                Board.board[112] = Pieces.PIECES.indexOf("K");
                Board.board[113] = Pieces.PIECES.indexOf("R");
                Board.king[0] = 112;
                del(Constants.WHITE, 114);
                del(Constants.WHITE, 110);
                Board.pieces[0].push(112);
                Board.pieces[0].push(113);
            }
            else {
                Board.board[30] = 0;
                Board.board[26] = 0;
                Board.board[28] = Pieces.PIECES.indexOf("k");
                Board.board[29] = Pieces.PIECES.indexOf("r");
                Board.king[1] = 28;
                del(Constants.BLACK, 30);
                del(Constants.BLACK, 26);
                Board.pieces[1].push(28);
                Board.pieces[1].push(29);
            }
        }
    }
    Moves.castle = castle;
    function unmakeMove(move) {
        Board.enPassant = -1;
        Board.turn = !Board.turn;
        if (move.signal == Constants.SIGNALS.indexOf("X")) {
            Board.K = true;
            Board.Q = true;
        }
        if (move.signal == Constants.SIGNALS.indexOf("K"))
            Board.K = true;
        if (move.signal == Constants.SIGNALS.indexOf("Q"))
            Board.Q = true;
        if (move.signal == Constants.SIGNALS.indexOf("x")) {
            Board.k = true;
            Board.q = true;
        }
        if (move.signal == Constants.SIGNALS.indexOf("k"))
            Board.k = true;
        if (move.signal == Constants.SIGNALS.indexOf("q"))
            Board.q = true;
        if (move.castle) {
            uncastle(move.castle);
        }
        if (move.isEp) {
            Board.enPassant = move.dest;
            Board.board[move.source] = Board.board[move.dest];
            Board.board[move.dest] = 0;
            let pawn = move.capture;
            let dir = -1;
            if (Board.turn == Constants.WHITE)
                dir = 1;
            dir *= 12;
            Board.board[move.dest + dir] = pawn;
            Board.pieces[Board.turn ? 1 : 0].push(move.source);
            Board.pieces[!Board.turn ? 1 : 0].push(move.dest + dir);
            del(Board.turn, move.dest);
            return;
        }
        if (move.promote) {
            let pawn = Pieces.PIECES.indexOf(Board.turn ? "p" : "P");
            Board.board[move.source] = pawn;
            Board.board[move.dest] = move.capture;
            if (move.capture)
                Board.pieces[!Board.turn ? 1 : 0].push(move.dest);
            del(Board.turn, move.dest);
            Board.pieces[Board.turn ? 1 : 0].push(move.source);
            return;
        }
        if (Pieces.PIECES[Board.board[move.dest]].toLowerCase() == "k")
            Board.king[Board.turn ? 1 : 0] = move.source;
        del(Board.turn, move.dest);
        Board.pieces[Board.turn ? 1 : 0].push(move.source);
        if (move.capture)
            Board.pieces[!Board.turn ? 1 : 0].push(move.dest);
        Board.board[move.source] = Board.board[move.dest];
        Board.board[move.dest] = move.capture;
    }
    Moves.unmakeMove = unmakeMove;
    function uncastle(dir) {
        if (dir == 1) {
            if (!Board.turn) {
                Board.board[115] = 0;
                Board.board[117] = Pieces.PIECES.indexOf("R");
                del(Constants.WHITE, 115);
                Board.pieces[0].push(117);
            }
            else {
                Board.board[31] = 0;
                Board.board[33] = Pieces.PIECES.indexOf("r");
                del(Constants.BLACK, 31);
                Board.pieces[1].push(33);
            }
        }
        else if (dir == 2) {
            if (!Board.turn) {
                Board.board[113] = 0;
                Board.board[110] = Pieces.PIECES.indexOf("R");
                del(Constants.WHITE, 113);
                Board.pieces[0].push(110);
            }
            else {
                Board.board[29] = 0;
                Board.board[26] = Pieces.PIECES.indexOf("r");
                del(Constants.BLACK, 29);
                Board.pieces[1].push(26);
            }
        }
    }
    Moves.uncastle = uncastle;
    class Move {
        source;
        dest;
        capture;
        castle;
        promote;
        signal;
        isEp;
        constructor(source, dest, capture, castle, promote, signal, isEp) {
            this.source = source;
            this.dest = dest;
            this.capture = capture;
            this.castle = castle;
            this.promote = promote;
            this.signal = signal;
            this.isEp = isEp;
        }
    }
    Moves.Move = Move;
})(Moves || (Moves = {}));
export default Moves;
