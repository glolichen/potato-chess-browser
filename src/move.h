#ifndef MOVE_H
#define MOVE_H

#include <string>

#define NEW_MOVE(source, dest, castle, promote, isEp) (isEp | ((promote) << 1) | ((castle) << 4) | ((dest) << 6) | ((source) << 12))
#define SOURCE(move) (((move) >> 12) & 0b111111)
#define DEST(move) (((move) >> 6) & 0b111111)
#define CASTLE(move) (((move) >> 4) & 0b11)
#define PROMOTE(move) (((move) >> 1) & 0b111)
#define IS_EP(move) ((move) & 0b1)

namespace move {
	struct Move {
		int source;
		int dest;
		int castle;
		int promote;
		bool isEp;
	};
	void printMove(int move, bool newLine);
	void makeMove(bitboard::Position &board, int move);
	std::string makeMoveForJS(std::string fen, move::Move move);
}

#endif