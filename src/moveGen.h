#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <string>
#include <vector>
#include "move.h"

namespace moveGen {
	ull get_checks(const bitboard::Position &board, bool color);
	void move_gen(const bitboard::Position &board, std::vector<int> &moves);
	void move_gen_with_ordering(const bitboard::Position &board, std::vector<int> &moves);

	bool get_checks_for_JS(std::string fen);
	std::vector<move::Move> move_gen_for_JS(std::string fen);
}

#endif