#ifndef EVAL_H
#define EVAL_H

#include <vector>

namespace eval {
	void init();

	int evaluate(bitboard::Position *board);

	int kingLocationEval(bitboard::Position *board, int endPhase);
}

#endif
