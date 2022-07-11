#ifndef PIECEMOVES_H
#define PIECEMOVES_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace pieceMoves {
	void pmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned);
	void nmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned);
	void bmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned);
	void rmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned, bool isRook);
	void kmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* attacked);
}

#endif
