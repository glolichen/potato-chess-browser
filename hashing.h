#ifndef HASHING_H
#define HASHING_H

namespace hashing {
	using ull = unsigned long long;
	void initZobristTables();
	std::tuple<ull, ull, ull> getZobristHash();
	int toPiece(char piece);
}

#endif
