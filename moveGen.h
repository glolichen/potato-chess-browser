#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace moveGen {
	void moveGen(std::vector<moves::Move>* moves);
	std::vector<moves::Move> moveGenForJS();
	void moveGenWithOrdering(std::vector<moves::Move>* moves);
}

#endif
