import Constants from "./Constants.js";
import Pieces from "./Pieces.js";
var Board;
(function (Board) {
    Board.board = [];
    Board.king = [];
    Board.enPassant = -1;
    Board.pieces = [[], []];
    Board.K = false;
    Board.Q = false;
    Board.k = false;
    Board.q = false;
    Board.turn = false;
    Board.halfMoveClock = 0;
    function decode(fen) {
        Board.board.length = 144;
        Board.board.fill(-1);
        Board.king.length = 2;
        let result = fen.split(" ");
        Board.turn = result[1] == "b";
        Board.halfMoveClock = parseInt(result[4]);
        let castle = result[2];
        if (castle.includes("K"))
            Board.K = true;
        if (castle.includes("Q"))
            Board.Q = true;
        if (castle.includes("k"))
            Board.k = true;
        if (castle.includes("q"))
            Board.q = true;
        let ep = result[3];
        if (ep != "-") {
            Board.enPassant = SANToNotation(ep);
        }
        let lines = result[0].split("/");
        for (let i = 0; i < 8; i++) {
            let cur = 2;
            let curRank = lines[i];
            for (let j = 0; j < curRank.length; j++) {
                if (Constants.NUMBERS.includes(curRank[j])) {
                    cur += Constants.NUMBERS.indexOf(curRank[j]);
                    continue;
                }
                Board.board[i * 12 + cur + 24] = Pieces.PIECES.indexOf(curRank[j]);
                cur++;
            }
        }
        for (let i = 2; i < 10; i++) {
            for (let j = 2; j < 10; j++) {
                let index = i * 12 + j;
                if (Board.board[index] == -1) {
                    Board.board[index] = 0;
                    continue;
                }
                let color = Pieces.getColor(Board.board[index]) ? 1 : 0;
                if (Pieces.PIECES[Board.board[index]] == "K")
                    Board.king[0] = index;
                if (Pieces.PIECES[Board.board[index]] == "k")
                    Board.king[1] = index;
                Board.pieces[color].push(index);
            }
        }
    }
    Board.decode = decode;
    function notationToXY(coord) {
        if (coord == -1 || Board.board[coord] == -1)
            return "-1";
        coord -= 26;
        coord -= Math.floor(coord / 12) * 4;
        let x = Math.floor(coord / 8).toString();
        let y = (coord % 8).toString();
        return x + y;
    }
    Board.notationToXY = notationToXY;
    function notationToSAN(coord) {
        let xy = notationToXY(coord);
        let y = 8 - Constants.NUMBERS.indexOf(xy[0]);
        let x = Constants.FILES[Constants.NUMBERS.indexOf(xy[1])];
        return x + y;
    }
    Board.notationToSAN = notationToSAN;
    function SANToNotation(san) {
        let y = Constants.FILES.indexOf(san[0]) + 2;
        let x = 8 - parseInt(san[1]) + 2;
        return x * 12 + y;
    }
    Board.SANToNotation = SANToNotation;
    function XYToNotation(xy) {
        return (parseInt(xy[1]) + 2) + parseInt(xy[0]) * 12 + 24;
    }
    Board.XYToNotation = XYToNotation;
    function print() {
        console.log("+---+---+---+---+---+---+---+---+");
        for (let i = 2; i < 10; i++) {
            let line = "|";
            for (let j = 2; j < 10; j++) {
                let index = i * 12 + j;
                let print = " ";
                if (Board.board[index] != 0)
                    print = Pieces.PIECES[Board.board[index]];
                line += ` ${print} |`;
            }
            console.log(line);
            console.log("+---+---+---+---+---+---+---+---+");
        }
        console.log(`White Kingside: ${Board.K}       White Queenside: ${Board.Q}`);
        console.log(`Black Kingside: ${Board.k}       Black Queenside: ${Board.q}`);
        console.log(`En Passant Square: ${Board.enPassant == -1 ? "None" : notationToSAN(Board.enPassant)}`);
        console.log(`Turn: ${Board.turn ? "Black" : "White"}`);
    }
    Board.print = print;
})(Board || (Board = {}));
export default Board;
