#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include "bitboard.h"
#include "move.h"

namespace search {
	struct SearchResult {
		int move;
		int depth;
		int eval;
		bool mateFound;
	};

	int minimax(const bitboard::Position &board, int depth, int alpha, int beta, int depthFromStart);

	SearchResult search(std::string fen, int timeMS);

	bool evalIsMate(int eval);
}

#endif
