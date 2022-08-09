#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "checks.h"
#include "move.h"

namespace moveGen {
	void pmoves(int loc, std::vector<move::Move>* moves, std::vector<checks::Check>* pinned);
	void nmoves(int loc, std::vector<move::Move>* moves, std::vector<checks::Check>* pinned);
	void bmoves(int loc, std::vector<move::Move>* moves, std::vector<checks::Check>* pinned);
	void rmoves(int loc, std::vector<move::Move>* moves, std::vector<checks::Check>* pinned, bool isRook);
	void kmoves(int loc, std::vector<move::Move>* moves, std::vector<checks::Check>* attacked);
	
	void moveGen(std::vector<move::Move>* moves);
	std::vector<move::Move> moveGenForJS();
	void moveGenWithOrdering(std::vector<move::Move>* moves);
}

#endif
