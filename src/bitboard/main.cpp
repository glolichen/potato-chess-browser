// #include <iostream>
// #include <tuple>

// #include "bitboard.h"
// #include "eval.h"
// #include "maps.h"
// #include "move.h"
// #include "search.h"

// int main() {
// 	maps::init();
// 	eval::init();

// 	auto search = search::search("r2q2k1/pp1r1pp1/n1p4p/6N1/3PR3/3Q4/PP3PPP/4R1K1 w - - 0 1", 2000);
// 	move::printMove(search.move, true);
// 	std::cout << search.depth;

// 	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// 	// auto hashes1 = hashing::hash(&bitboard::board);

// 	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// 	// auto hashes2 = hashing::hash(&bitboard::board);

// 	// std::cout << (hashes1 == hashes2) << "\n";

// 	return 0;
// }
