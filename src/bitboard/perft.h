#ifndef PERFT_H
#define PERFT_H

#include "bitboard.h"

namespace perft {
	struct PerftResult {
		std::string answer;
		int time;
	};
	PerftResult test(std::string fen, int depth);
	ull perft(bitboard::Position *board, int depth, bool first);
}

#endif
