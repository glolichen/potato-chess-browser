#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include <vector>

using ull = unsigned long long;
enum Color { WHITE, BLACK };
enum Pieces { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, ALL };

#define SET1(bitboard, pos) (bitboard |= 1ull << (pos))
#define SET0(bitboard, pos) (bitboard ^= 1ull << (pos))
#define QUERY(bitboard, pos) (bitboard >> (pos) & 1)

namespace bitboard {
	struct Position {
		ull allPieces;
		ull pieces[2][7];
		int mailbox[64];

		bool turn;
		int enPassant;
		bool castle[4];

		int fiftyMoveClock;
		int fullMove;
	};

	extern Position board;
	
	extern std::string squares[];
	extern std::string pieces;

	std::vector<std::string> split(std::string str, char splitOn);

	void decode(std::string fen);
	std::string encode(Position *board);

	void printBoard(Position *board);
}

#endif