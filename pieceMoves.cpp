#include <algorithm>
#include <cstdlib>
#include <vector>

#include "board.h"
#include "checks.h"
#include "constants.h"
#include "moves.h"
#include "moveGen.h"
#include "pieceMoves.h"

void pieceMoves::pmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned) {
	int axis = -1;
	for (checks::Check c : *pinned) {
		if (c.coord == loc) {
			axis = c.axis;
			break;
		}
	}

	bool starting = false;
	if (board::turn)
		starting = (loc / 12 == 3);
	else
		starting = (loc / 12 == 8);

	int offset = loc + constants::PAWN_MOVE[board::turn][0];
	if (board::board[offset] == 0 && (axis == -1 || axis == abs(constants::PAWN_MOVE[board::turn][0]))) {
		if (board::turn && offset / 12 == 9) {
			moves->push_back({ loc, offset, 0, 0, 'q', 0, 0 });
			moves->push_back({ loc, offset, 0, 0, 'r', 0, 0 });
			moves->push_back({ loc, offset, 0, 0, 'b', 0, 0 });
			moves->push_back({ loc, offset, 0, 0, 'n', 0, 0 });
		} else if (!board::turn && offset / 12 == 2) {
			moves->push_back({ loc, offset, 0, 0, 'Q', 0, 0 });
			moves->push_back({ loc, offset, 0, 0, 'R', 0, 0 });
			moves->push_back({ loc, offset, 0, 0, 'B', 0, 0 });
			moves->push_back({ loc, offset, 0, 0, 'N', 0, 0 });
		} else
			moves->push_back({ loc, offset, 0, 0, 0, 0, 0 });

		if (starting && board::board[loc + constants::PAWN_START[board::turn][0]] == 0)
			moves->push_back({ loc, loc + constants::PAWN_START[board::turn][0], 0, 0, 0, 0, 0 });
	}

	for (int offset2 : constants::PAWN_CAPTURE[board::turn]) {
		int i = loc + offset2;
		if (!board::board[i] || board::board[i] == -1)
			continue;

		bool turn = board::turn;
		bool pieceIsLower = islower(board::board[i]);

		if (pieceIsLower != turn && (axis == -1 || axis == abs(offset2))) {
			if (board::turn && i / 12 == 9) {
				moves->push_back({ loc, i, board::board[i], 0, 'q', 0, 0 });
				moves->push_back({ loc, i, board::board[i], 0, 'r', 0, 0 });
				moves->push_back({ loc, i, board::board[i], 0, 'b', 0, 0 });
				moves->push_back({ loc, i, board::board[i], 0, 'n', 0, 0 });
			} else if (!board::turn && i / 12 == 2) {
				moves->push_back({ loc, i, board::board[i], 0, 'Q', 0, 0 });
				moves->push_back({ loc, i, board::board[i], 0, 'R', 0, 0 });
				moves->push_back({ loc, i, board::board[i], 0, 'B', 0, 0 });
				moves->push_back({ loc, i, board::board[i], 0, 'N', 0, 0 });
			} else
				moves->push_back({ loc, i, board::board[i], 0, 0, 0, 0 });
		}
	}
}
void pieceMoves::nmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned) {
	for (checks::Check c : *pinned) {
		if (c.coord == loc)
			return;
	}

	for (int offset : constants::KNIGHT) {
		int i = loc + offset;

		if (board::board[i] == -1)
			continue;
		if ((bool) islower(board::board[i]) == board::turn && board::board[i])
			continue;

		moves->push_back({ loc, i, board::board[i], 0, 0, 0, 0 });
	}
}
void pieceMoves::bmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned) {
	for (int offset : constants::BISHOP) {
		int axis = -1;
		for (checks::Check c : *pinned) {
			if (c.coord == loc) {
				axis = c.axis;
				break;
			}
		}

		if (axis != -1 && axis != abs(offset))
			continue;

		int i = loc + offset;
		while (board::board[i] != -1) {
			if (board::board[i] && (bool) islower(board::board[i]) == board::turn)
				break;

			moves->push_back({ loc, i, board::board[i], 0, 0, 0, 0 });

			if (board::board[i])
				break;

			i += offset;
		}
	}
}
void pieceMoves::rmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* pinned, bool isRook) {
	for (int offset : constants::ROOK) {
		int axis = -1;
		for (checks::Check c : *pinned) {
			if (c.coord == loc) {
				axis = c.axis;
				break;
			}
		}

		if (axis != -1 && axis != abs(offset))
			continue;

		int i = loc + offset;
		while (board::board[i] != -1) {
			if (board::board[i] && (bool) islower(board::board[i]) == board::turn)
				break;

			char signal = 0;
			if (isRook) {
				if (board::turn) {
					if (board::k & (loc % 12 == 9))
						signal = 'k';
					if (board::q & (loc % 12 == 2))
						signal = 'q';
				} else {
					if (board::K & (loc % 12 == 9))
						signal = 'K';
					if (board::Q & (loc % 12 == 2))
						signal = 'Q';
				}
			}

			moves->push_back({ loc, i, board::board[i], 0, 0, signal, 0 });

			if (board::board[i])
				break;

			i += offset;
		}
	}
}
void pieceMoves::kmoves(int loc, std::vector<moves::Move>* moves, std::vector<checks::Check>* attacked) {
	for (int offset : constants::KING) {
		int i = loc + offset;

		if (board::board[i] == -1)
			continue;
		if ((bool) islower(board::board[i]) == board::turn && board::board[i])
			continue;

		bool contains = false;
		for (checks::Check c : *attacked) {
			if (c.coord == i) {
				contains = true;
				break;
			}
		}
		if (contains)
			continue;

		char signal = 0;
		if (board::turn) {
			if (board::k & board::q)
				signal = 'x';
			else if (board::k)
				signal = 'k';
			else if (board::q)
				signal = 'q';
		} else {
			if (board::K & board::Q)
				signal = 'X';
			else if (board::K)
				signal = 'K';
			else if (board::Q)
				signal = 'Q';
		}

		moves->push_back({ loc, i, board::board[i], 0, 0, signal, 0 });
	}

	for (checks::Check c : *attacked) {
		if (c.coord == board::king[board::turn])
			return;
	}

	if (!board::turn) {
		bool KLegal = true;
		bool QLegal = true;
		for (checks::Check c : *attacked) {
			if (c.coord == 115 || c.coord == 116)
				KLegal = false;
			if (c.coord == 113 || c.coord == 112)
				QLegal = false;
		}

		if (board::K && KLegal && board::board[114] == 'K' && board::board[117] == 'R' && !board::board[115] && !board::board[116]) {
			char signal = 0;
			if (board::K & board::Q)
				signal = 'X';
			else if (board::K)
				signal = 'K';
			else if (board::Q)
				signal = 'Q';
			moves->push_back({ 114, 116, 0, 1, 0, signal, 0 });
		}
		if (board::Q && QLegal && board::board[114] == 'K' && board::board[110] == 'R' && !board::board[113] && !board::board[112] && !board::board[111]) {
			char signal = 0;
			if (board::K & board::Q)
				signal = 'X';
			else if (board::K)
				signal = 'K';
			else if (board::Q)
				signal = 'Q';
			moves->push_back({ 114, 112, 0, 2, 0, signal, 0 });
		}
	} else {
		bool KLegal = true;
		bool QLegal = true;
		for (checks::Check c: *attacked) {
			if (c.coord == 31 || c.coord == 32)
				KLegal = false;
			if (c.coord == 29 || c.coord == 28)
				QLegal = false;
		}

		if (board::k && KLegal && board::board[30] == 'k' && board::board[33] == 'r' && !board::board[31] && !board::board[32]) {
			char signal = 0;
			if (board::k & board::q)
				signal = 'x';
			else if (board::k)
				signal = 'k';
			else if (board::q)
				signal = 'q';
			moves->push_back({ 30, 32, 0, 1, 0, signal, 0 });
		}
		if (board::q && QLegal && board::board[30] == 'k' && board::board[26] == 'r' && !board::board[29] && !board::board[28] && !board::board[27]) {
			char signal = 0;
			if (board::k & board::q)
				signal = 'x';
			else if (board::k)
				signal = 'k';
			else if (board::q)
				signal = 'q';
			moves->push_back({ 30, 28, 0, 2, 0, signal, 0 });
		}
	}
}