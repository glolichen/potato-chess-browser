#include <chrono>
#include <cstring>
#include <string>
#include <vector>

#include "bitboard.h"
#include "maps.h"
#include "move.h"
#include "moveGen.h"
#include "perft.h"

perft::PerftResult perft::test(std::string fen, int depth) {
	maps::init();
	bitboard::decode(fen);

	ull start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ull result = perft::perft(&bitboard::board, depth, true);
	ull end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	
	return { std::to_string(result), (int) (end - start) };
}

ull perft::perft(bitboard::Position *board, int depth, bool first) {
	std::vector<int> moves;
	moveGen::moveGen(board, &moves);

	if (depth == 1)
		return moves.size();

	ull positions = 0;

	for (int move : moves) {
		bitboard::Position newBoard;
		memcpy(&newBoard, board, sizeof(*board));
		move::makeMove(&newBoard, move);
		positions += perft(&newBoard, depth - 1, false);
	}

	return positions;
}
