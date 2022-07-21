onmessage = e => {
    importScripts("./engine.js");
    importScripts("./pieces.js");
    importScripts("./moves.js");

    Module["onRuntimeInitialized"] = () => {
        Module["decode"](e.data[0]);
        
        let out = Module["search"](e.data[1]);

        let move = new Move(out.move.source, out.move.dest, charToPiece(out.move.capture), out.move.castle, 
            charToPiece(out.move.promote), convertSignal(out.move.signal), out.move.isEp);

        postMessage([move, out.depth, out.eval, out.mateFound]);
    };
}

function convertSignal(char) {
    let string = String.fromCharCode(char);
    return string == "\u0000" ? 0 : string;
}

function charToPiece(char) {
    let piece = PIECES.indexOf(String.fromCharCode(char));
    return piece == -1 ? 0 : piece;
}