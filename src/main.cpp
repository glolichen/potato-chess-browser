// #include <iostream>
// #include <tuple>

// #include "bitboard.h"
// #include "eval.h"
// #include "maps.h"
// #include "move.h"
// #include "moveGen.h"
// #include "perft.h"
// #include "search.h"

// int main() {
// 	// maps::init();
// 	// eval::init();

// 	// bitboard::decode("r1b1kbnr/pppp1p1p/2n2qp1/4p3/4P3/3P1N2/PPP1BPPP/RNBQK2R w KQkq - 0 5");
// 	// std::vector<int> moves;
// 	// moveGen::moveGen(bitboard::board, moves);
// 	// for (const int &move : moves)
// 	// 	move::printMove(move, true);
// 	// std::cout << moves.size();

// 	auto search = search::search("rnbqkb1r/ppp1pppp/8/8/2B5/2n2Q2/PPPP1PPP/R1B1K1NR b KQkq - 1 5", 5000);
// 	move::printMove(search.move, true);
// 	std::cout << search.depth;

// 	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// 	// auto hashes1 = hashing::hash(&bitboard::board);

// 	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// 	// auto hashes2 = hashing::hash(&bitboard::board);

// 	// std::cout << (hashes1 == hashes2) << "\n";

// 	// move::printMove(211648, true);

// 	// perft::test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);

// 	return 0;
// }
