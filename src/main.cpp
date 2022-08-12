#include <iostream>
#include <tuple>

#include "bitboard.h"
#include "eval.h"
#include "maps.h"
#include "move.h"
#include "moveGen.h"
#include "perft.h"
#include "search.h"

int main() {
	// maps::init();
	// eval::init();

	// bitboard::decode("r5Nr/Ppppkppp/1b3nb1/nPP5/BB2P3/q4N2/Pp1P2PP/R2Q1RK1 b - - 2 2");
	// std::vector<int> moves;
	// moveGen::moveGen(bitboard::board, moves);
	// for (const int &move : moves)
	// 	move::printMove(move, true);
	// std::cout << moves.size();

	// auto search = search::search("rnbqkb1r/ppp1pppp/8/8/2B5/2n2Q2/PPPP1PPP/R1B1K1NR b KQkq - 1 5", 5000);
	// move::printMove(search.move, true);
	// std::cout << search.depth;

	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	// auto hashes1 = hashing::hash(&bitboard::board);

	// bitboard::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	// auto hashes2 = hashing::hash(&bitboard::board);

	// std::cout << (hashes1 == hashes2) << "\n";

	// move::printMove(211648, true);

	perft::test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);

	return 0;
}
