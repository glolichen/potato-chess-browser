#ifndef PERFT_H
#define PERFT_H

#include <map>
#include <string>
#include <vector>

#include "moves.h"

namespace perft {
	using ui = unsigned int;
	struct Entry {
		moves::Move moves;
		ui possible;
	};
	ui perft(int depth, bool first);
	std::vector<Entry> runPerft(std::string fen, int depth);
}

#endif
