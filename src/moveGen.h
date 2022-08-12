#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <string>
#include <vector>
#include "move.h"

namespace moveGen {
	ull getChecks(bitboard::Position &board, bool color);
	void moveGen(bitboard::Position &board, std::vector<int> &moves);
	void moveGenWithOrdering(bitboard::Position &board, std::vector<int> &moves);

	bool getChecksForJS(std::string fen);
	std::vector<move::Move> moveGenForJS(std::string fen);
}

#endif