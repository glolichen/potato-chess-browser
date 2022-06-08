var Pieces;
(function (Pieces) {
    Pieces.PIECES = " PNBRQKpnbrqk";
    function getColor(piece) {
        return piece > 6;
    }
    Pieces.getColor = getColor;
    function getImage(piece) {
        let color = getColor(piece) ? "b" : "w";
        let pieceImage = Pieces.PIECES[piece].toLowerCase();
        return color + pieceImage;
    }
    Pieces.getImage = getImage;
    let Offsets;
    (function (Offsets) {
        Offsets.ROOK = [1, -1, 12, -12];
        Offsets.BISHOP = [11, -11, 13, -13];
        Offsets.QUEEN = [1, -1, 12, -12, 11, -11, 13, -13];
        Offsets.KNIGHT = [10, -10, 14, -14, 23, -23, 25, -25];
        Offsets.KING = [1, -1, 12, -12, 11, -11, 13, -13];
        Offsets.PAWN_MOVE = [[-12], [12]];
        Offsets.PAWN_CAPTURE = [[-11, -13], [11, 13]];
        Offsets.PAWN_START = [[-24], [24]];
    })(Offsets = Pieces.Offsets || (Pieces.Offsets = {}));
})(Pieces || (Pieces = {}));
export default Pieces;
