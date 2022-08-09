onmessage = e => {
	importScripts("./engine.js");
	importScripts("./pieces.js");
	importScripts("./moves.js");

	Module["onRuntimeInitialized"] = () => {		
		let out = Module.search(e.data[0], e.data[1]);
		postMessage([convert(out.move), out.depth, out.eval, out.mateFound]);
	};
}

var squares = [
	26,  27,  28,  29,  30,  31,  32,  33,
	38,  39,  40,  41,  42,  43,  44,  45,
	50,  51,  52,  53,  54,  55,  56,  57,
	62,  63,  64,  65,  66,  67,  68,  69,
	74,  75,  76,  77,  78,  79,  80,  81,
	86,  87,  88,  89,  90,  91,  92,  93,
	98,  99,  100, 101, 102, 103, 104, 105,
	110, 111, 112, 113, 114, 115, 116, 117,
];

function convert(move) {
	let source = squares[63 - ((move >> 12) & 0b111111)];
	let dest = squares[63 - ((move >> 6) & 0b111111)];
	let castle = (move >> 4) & 0b11
	let promote = (move >> 1) & 0b111;
	let isEp = (move) & 0b1;
	return new Move(source, dest, 0, castle, promote, 0, isEp);
}