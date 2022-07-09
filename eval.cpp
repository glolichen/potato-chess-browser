#include <iostream>
#include <unordered_map>

#include "board.h"
#include "eval.h"

int midValues[6] = { 100, 320, 333, 510, 880,  0 }; // alphazero piece values
int endValues[6] = { 120, 310, 340, 510, 1000,  0 };

// piece table values from Rofchade by Ronald Friederich
// https://www.chessprogramming.org/RofChade

int midPawnTable[64] = {
	  0,   0,   0,   0,   0,   0,  0,   0,
	 98, 134,  61,  95,  68, 126, 34, -11,
	 -6,   7,  26,  31,  65,  56, 25, -20,
	-14,  13,   6,  21,  23,  12, 17, -23,
	-27,  -2,  -5,  18,  20,   6, 10, -25,
	-26,  -4,  -4, -10,   3,   3, 33, -12,
	-35,  -1, -20, -23, -15,  24, 38, -22,
	  0,   0,   0,   0,   0,   0,  0,   0,
};
int endPawnTable[64] = {
	  0,   0,   0,   0,   0,   0,   0,   0,
	178, 173, 158, 134, 147, 132, 165, 187,
	 94, 100,  85,  67,  56,  53,  82,  84,
	 32,  24,  13,   5,  -2,   4,  17,  17,
	 13,   9,  -3,  -7,  -7,  -8,   3,  -1,
	  4,   7,  -6,   1,   0,  -5,  -1,  -8,
	 13,   8,   8,  10,  13,   0,   2,  -7,
	  0,   0,   0,   0,   0,   0,   0,   0,
};

int midKnightTable[64] = {
	-167, -89, -34, -49,  61, -97, -15, -107,
	 -73, -41,  72,  36,  23,  62,   7,  -17,
	 -47,  60,  37,  65,  84, 129,  73,   44,
	  -9,  17,  19,  53,  37,  69,  18,   22,
	 -13,   4,  16,  13,  28,  19,  21,   -8,
	 -23,  -9,  12,  10,  19,  17,  25,  -16,
	 -29, -53, -12,  -3,  -1,  18, -14,  -19,
	-105, -21, -58, -33, -17, -28, -19,  -23,
};
int endKnightTable[64] = {
	-58, -38, -13, -28, -31, -27, -63, -99,
	-25,  -8, -25,  -2,  -9, -25, -24, -52,
	-24, -20,  10,   9,  -1,  -9, -19, -41,
	-17,   3,  22,  22,  22,  11,   8, -18,
	-18,  -6,  16,  25,  16,  17,   4, -18,
	-23,  -3,  -1,  15,  10,  -3, -20, -22,
	-42, -20, -10,  -5,  -2, -20, -23, -44,
	-29, -51, -23, -15, -22, -18, -50, -64,
};

int midBishopTable[64] = {
	-29,   4, -82, -37, -25, -42,   7,  -8,
	-26,  16, -18, -13,  30,  59,  18, -47,
	-16,  37,  43,  40,  35,  50,  37,  -2,
	 -4,   5,  19,  50,  37,  37,   7,  -2,
	 -6,  13,  13,  26,  34,  12,  10,   4,
	  0,  15,  15,  15,  14,  27,  18,  10,
	  4,  15,  16,   0,   7,  21,  33,   1,
	-33,  -3, -14, -21, -13, -12, -39, -21,
};
int endBishopTable[64] = {
	-14, -21, -11,  -8, -7,  -9, -17, -24,
	 -8,  -4,   7, -12, -3, -13,  -4, -14,
	  2,  -8,   0,  -1, -2,   6,   0,   4,
	 -3,   9,  12,   9, 14,  10,   3,   2,
	 -6,   3,  13,  19,  7,  10,  -3,  -9,
	-12,  -3,   8,  10, 13,   3,  -7, -15,
	-14, -18,  -7,  -1,  4,  -9, -15, -27,
	-23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int midRookTable[64] = {
	 32,  42,  32,  51, 63,  9,  31,  43,
	 27,  32,  58,  62, 80, 67,  26,  44,
	 -5,  19,  26,  36, 17, 45,  61,  16,
	-24, -11,   7,  26, 24, 35,  -8, -20,
	-36, -26, -12,  -1,  9, -7,   6, -23,
	-45, -25, -16, -17,  3,  0,  -5, -33,
	-44, -16, -20,  -9, -1, 11,  -6, -71,
	-19, -13,   1,  17, 16,  7, -37, -26,
};
int endRookTable[64] = {
	13, 10, 18, 15, 12,  12,   8,   5,
	11, 13, 13, 11, -3,   3,   8,   3,
	 7,  7,  7,  5,  4,  -3,  -5,  -3,
	 4,  3, 13,  1,  2,   1,  -1,   2,
	 3,  5,  8,  4, -5,  -6,  -8, -11,
	-4,  0, -5, -1, -7, -12,  -8, -16,
	-6, -6,  0,  2, -9,  -9, -11,  -3,
	-9,  2,  3, -1, -5, -13,   4, -20,
};

int midQueenTable[64] = {
	-28,   0,  29,  12,  59,  44,  43,  45,
	-24, -39,  -5,   1, -16,  57,  28,  54,
	-13, -17,   7,   8,  29,  56,  47,  57,
	-27, -27, -16, -16,  -1,  17,  -2,   1,
	 -9, -26,  -9, -10,  -2,  -4,   3,  -3,
	-14,   2, -11,  -2,  -5,   2,  14,   5,
	-35,  -8,  11,   2,   8,  15,  -3,   1,
	 -1, -18,  -9,  20, -15, -25, -31, -50,
};
int endQueenTable[64] = {
	 -9,  22,  22,  27,  27,  19,  10,  20,
	-17,  20,  32,  41,  58,  25,  30,   0,
	-20,   6,   9,  49,  47,  35,  19,   9,
	  3,  22,  24,  45,  57,  40,  57,  36,
	-18,  28,  19,  47,  31,  34,  39,  23,
	-16, -27,  15,   6,   9,  17,  10,   5,
	-22, -23, -30, -16, -16, -23, -36, -32,
	-33, -28, -22, -43,  -5, -32, -20, -41,
};

int midKingTable[64] = {
	-65,  23,  16, -15, -56, -34,   2,  13,
	 29,  -1, -20,  -7,  -8,  -4, -38, -29,
	 -9,  24,   2, -16, -20,   6,  22, -22,
	-17, -20, -12, -27, -30, -25, -14, -36,
	-49,  -1, -27, -39, -46, -44, -33, -51,
	-14, -14, -22, -46, -44, -30, -15, -27,
	  1,   7,  -8, -64, -43, -16,   9,   8,
	-15,  36,  12, -54,   8, -28,  24,  14,
};
int endKingTable[64] = {
	-74, -35, -18, -18, -11,  15,   4, -17,
	-12,  17,  14,  17,  17,  38,  23,  11,
	 10,  17,  23,  15,  20,  45,  44,  13,
	 -8,  22,  24,  27,  26,  33,  26,   3,
	-18,  -4,  21,  24,  27,  23,   9, -11,
	-19,  -3,  11,  21,  23,  16,   7,  -9,
	-27, -11,   4,  13,  14,   4,  -5, -17,
	-53, -34, -21, -11, -28, -14, -24, -43
};

int* tempMidTables[6] = {
	midPawnTable,
	midKnightTable,
	midBishopTable,
	midRookTable,
	midQueenTable,
	midKingTable
};
int* tempEndTables[6] = {
	endPawnTable,
	endKnightTable,
	endBishopTable,
	endRookTable,
	endQueenTable,
	endKingTable
};

std::unordered_map<char, int> PIECES = {
	{ 'P', 0  }, { 'N', 1  }, { 'B', 2  }, { 'R', 3  }, { 'Q', 4  }, { 'K', 5  },
	{ 'p', 6  }, { 'n', 7  }, { 'b', 8  }, { 'r', 9  }, { 'q', 10 }, { 'k', 11 }
};

int gamephaseInc[12] = { 0, 0, 1, 1, 1, 1, 2, 2, 4, 4, 0, 0 };
int midgameTables[12][64];
int endgameTables[12][64];

void flip(int* tableIn, int* tableOut) {
	int rows[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			rows[i][j] = tableIn[i * 8 + j];
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			tableOut[i * 8 + j] = rows[7 - i][j];
	}
}

void eval::initPieceTables() {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 64; j++)
			midgameTables[i][j] = tempMidTables[i][j];
	}
	for (int i = 0; i < 6; i++) {
		int flipped[64];
		flip(tempMidTables[i], flipped);
		for (int j = 0; j < 64; j++)
			midgameTables[i + 6][j] = flipped[j];
	}

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 64; j++)
			endgameTables[i][j] = tempEndTables[i][j];
	}
	for (int i = 0; i < 6; i++) {
		int flipped[64];
		flip(tempEndTables[i], flipped);
		for (int j = 0; j < 64; j++)
			endgameTables[i + 6][j] = flipped[j];
	}
}

int eval::evaluate() {
	int midgame[2], endgame[2];
	int midgameMaterial[2], endgameMaterial[2];
	int gamePhase = 0;

	midgame[0] = 0, midgame[1] = 0;
	endgame[0] = 0, endgame[1] = 0;
	midgameMaterial[0] = 0, midgameMaterial[1] = 0;
	endgameMaterial[0] = 0, endgameMaterial[1] = 0;

	std::vector<char> pieces;
	for (int side = 0; side <= 1; side++) {
		for (int i : board::pieces[side]) {
			int piece = board::board[i];
			pieces.push_back(piece);

			int pieceNumber = PIECES.at(piece);
			int pieceColor = (bool) islower(piece);
			int square = i - 26;
			square -= (square / 12) * 4;

			midgame[pieceColor] += midgameTables[pieceNumber][square];
			endgame[pieceColor] += endgameTables[pieceNumber][square];

			midgameMaterial[pieceColor] += midValues[pieceNumber % 6];
			endgameMaterial[pieceColor] += endValues[pieceNumber % 6];

			gamePhase += gamephaseInc[pieceNumber];
		}
	}

	if (eval::insufMat(&pieces))
		return 0;

	/* tapered eval */
	int midgameMaterialScore = midgameMaterial[0] - midgameMaterial[1];
	int endgameMaterialScore = endgameMaterial[0] - endgameMaterial[1];
	int midgameScore = (midgame[0] - midgame[1]) + midgameMaterialScore * 1.5;
	int endgameScore = (endgame[0] - endgame[1]) + endgameMaterialScore * 1.5;

	int midPhase = std::min(gamePhase, 24);
	int endPhase = 24 - midPhase;

	// std::cout << endPhase << "\n";

	return (midgameScore * midPhase + endgameScore * endPhase) / 24 + eval::kingLocationEval(endPhase);
}

bool eval::insufMat(std::vector<char>* pieces) {
	if (pieces->size() >= 4)
		return false;
	
	for (char piece : *pieces) {
		piece = tolower(piece);
		if (piece == 'r' || piece == 'q' || piece == 'p')
			return false;
	}

	return true;
}

int eval::kingLocationEval(int endPhase) {
	int ownKing = board::king[board::turn] - 26;
	ownKing -= (ownKing / 12) * 4;
	int ownKingX = ownKing / 12;
	int ownKingY = ownKing % 12;

	int enemyKing = board::king[!board::turn] - 26;
	enemyKing -= (enemyKing / 12) * 4;
	int enemyKingX = enemyKing / 12;
	int enemyKingY = enemyKing % 12;

	int enemyKingDstFromEdgeX = 3 - std::min(abs(3 - enemyKingX), abs(enemyKingX - 4));
	int enemyKingDstFromEdgeY = 3 - std::min(abs(3 - enemyKingY), abs(enemyKingY - 4));
	int enemyKingDstFromEdge = enemyKingDstFromEdgeX + enemyKingDstFromEdgeY;

	int enemyKingDstFromOwn = std::abs(ownKingX - enemyKingX) + std::abs(ownKingY - enemyKingY);

	int weight = endPhase * 10;
	return (enemyKingDstFromEdge + (enemyKingDstFromOwn * 0.75)) * weight;
}