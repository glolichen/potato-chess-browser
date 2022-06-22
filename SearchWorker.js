onmessage = e => {
    importScripts("./engine.js");
    importScripts("./Pieces.js");
    importScripts("./Moves.js");

    Module["onRuntimeInitialized"] = () => {
        console.log(e.data[0]);
        Module["decode"](e.data[0]);
        console.log(Module["encode"]());
        
        let out = Module["search"](e.data[1]);

        let move = new Move(out.source, out.dest, charToPiece(out.capture), out.castle, 
        charToPiece(out.promote), convertSignal(out.signal), out.isEp);

        postMessage(move);
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