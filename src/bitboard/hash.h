#ifndef HASH_H
#define HASH_H

#include <tuple>
#include "bitboard.h"

namespace hash {
	void init();
	std::tuple<ull, ull, ull> hash(bitboard::Position *board);
}

#endif
