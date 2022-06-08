namespace Pieces {
    export const PIECES: string = " PNBRQKpnbrqk";
    export function getColor(piece: number): boolean {
        return piece > 6;
    }
    export function getImage(piece: number): string {
        let color: string = getColor(piece) ? "b" : "w";
        let pieceImage: string = PIECES[piece].toLowerCase();
        return color + pieceImage;
    }

    export namespace Offsets {
        export const ROOK: number[] = [1, -1, 12, -12];
        export const BISHOP: number[] = [11, -11, 13, -13];
        export const QUEEN: number[] = [1, -1, 12, -12, 11, -11, 13, -13];
        export const KNIGHT: number[] = [10, -10, 14, -14, 23, -23, 25, -25];
        export const KING: number[] = [1, -1, 12, -12, 11, -11, 13, -13];

        export const PAWN_MOVE: number[][] = [[-12], [12]];
        export const PAWN_CAPTURE: number[][] = [[-11, -13], [11,  13]];
        export const PAWN_START: number[][] = [[-24], [24]];
    }
}

export default Pieces;