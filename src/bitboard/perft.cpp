#include <chrono>
#include <cstring>
#include <iostream>
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
	ull result = perft::perft(bitboard::board, depth);
	ull end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	std::cout << "total nodes: " << result << "\n";
	std::cout << "time: " << (end - start) << "ms\n";
	std::cout << "nodes per second: " << (result / (end - start) * 1000) << "\n";
	
	return { std::to_string(result), (int) (end - start) };
}

ull perft::perft(bitboard::Position &board, int depth) {
	std::vector<int> moves;
	moveGen::moveGen(board, moves);

	if (depth == 1)
		return moves.size();

	ull positions = 0;

	for (int move : moves) {
		bitboard::Position newBoard;
		memcpy(&newBoard, &board, sizeof(board));
		move::makeMove(newBoard, move);
		positions += perft::perft(newBoard, depth - 1);
	}

	return positions;
}
