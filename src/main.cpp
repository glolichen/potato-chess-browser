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
// 	maps::init();
// 	eval::init();

// 	bitboard::decode("8/R1p3p1/3k1p2/1B2r2p/r6K/8/8/8 w - - 0 37");
// 	std::vector<int> moves;
// 	moveGen::move_gen(bitboard::board, moves);
// 	for (const int &move : moves)
// 		move::print_move(move, true);
// 	std::cout << moves.size();

// 	// auto search = search::search("rn1qkbnr/pbpp1ppp/1p2p3/3P4/4P3/5N2/PPP2PPP/RNBQKB1R b KQkq - 0 5", 5000);
// 	// move::print_move(search.move, true);
// 	// std::cout << search.depth;

// 	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// 	// auto hashes1 = hashing::hash(&bitboard::board);

// 	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
// 	// auto hashes2 = hashing::hash(&bitboard::board);

// 	// std::cout << (hashes1 == hashes2) << "\n";

// 	// move::printMove(211648, true);

// 	// perft::test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);

// 	return 0;
// }
