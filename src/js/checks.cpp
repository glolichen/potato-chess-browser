#include <cmath>
#include <cstdlib>
#include <vector>

#include "board.h"
#include "checks.h"
#include "constants.h"
#include "moveGen.h"

void checks::getAttacked(std::vector<checks::Check>* attacked) {
	for (int index : board::pieces[!board::turn]) {
		char piece = tolower(board::board[index]);

		switch (piece) {
			case 'q': {
				checks::rchecks(index, attacked);
				checks::bchecks(index, attacked);
				break;
			}

			case 'r': {
				checks::rchecks(index, attacked);
				break;
			}

			case 'b': {
				checks::bchecks(index, attacked);
				break;
			}

			case 'n': {
				checks::nchecks(index, attacked);
				break;
			}

			case 'p': {
				checks::pchecks(index, attacked);
				break;
			}

			case 'k': {
				checks::kchecks(index, attacked);
				break;
			}
		}
	}
}
std::vector<checks::Check> checks::getAttackedForJS() {
	std::vector<checks::Check> attacked;
	checks::getAttacked(&attacked);
	return attacked;
}

void checks::pchecks(int loc, std::vector<checks::Check>* attacked) {
	for (int offset : constants::PAWN_CAPTURE[!board::turn]) {
		int i = loc + offset;
		if (board::board[i] == -1)
			continue;

		attacked->push_back({ i, loc, -10 });
	}
}
void checks::nchecks(int loc, std::vector<checks::Check>* attacked) {
	for (int offset : constants::KNIGHT) {
		int i = loc + offset;
		if (board::board[i] == -1)
			continue;

		attacked->push_back({ i, loc, -10 });
	}
}
void checks::bchecks(int loc, std::vector<checks::Check>* attacked) {
	char opponentKing = board::turn ? 'k' : 'K';
	for (int offset : constants::BISHOP) {
		int i = loc + offset;
		while (board::board[i] != -1) {
			attacked->push_back({ i, loc, -offset });

			if (board::board[i] && board::board[i] != opponentKing)
				break;

			i += offset;
		}
	}
}
void checks::rchecks(int loc, std::vector<checks::Check>* attacked) {
	char opponentKing = board::turn ? 'k' : 'K';
	for (int offset : constants::ROOK) {
		int i = loc + offset;
		while (board::board[i] != -1) {
			attacked->push_back({ i, loc, -offset });

			if (board::board[i] && board::board[i] != opponentKing)
				break;

			i += offset;
		}
	}
}
void checks::kchecks(int loc, std::vector<checks::Check>* attacked) {
	for (int offset : constants::KING) {
		int i = loc + offset;
		if (board::board[i] == -1)
			continue;

		attacked->push_back({ i, loc, -10 });
	}
}

bool checks::enPassantLegal() {
	int kingPos = board::king[board::turn];
	for (int offset : constants::ROOK) {
		int i = kingPos;
		while (board::board[i] != -1) {
			i += offset;

			if (board::board[i] == 0 || board::board[i] == -1)
				continue;

			if ((bool) islower(board::board[i]) != board::turn && (tolower(board::board[i]) == 'r' || tolower(board::board[i]) == 'q'))
				return false;
			else
				break;
		}
	}
	for (int offset : constants::BISHOP) {
		int i = kingPos;
		while (board::board[i] != -1) {
			i += offset;

			if (board::board[i] == 0 || board::board[i] == -1)
				continue;

			if ((bool) islower(board::board[i]) != board::turn && (tolower(board::board[i]) == 'b' || tolower(board::board[i]) == 'q'))
				return false;
			else
				break;
		}
	}
	return true;
}

void checks::getPinned(std::vector<checks::Check>* pinned) {
	int kingPos = board::king[board::turn];

	for (int piece : board::pieces[!board::turn]) {
		char pieceType = std::tolower(board::board[piece]);

		if (pieceType != 'r' && pieceType != 'b' && pieceType != 'q')
			continue;

		int offset = 0;
		if (pieceType == 'r' || pieceType == 'q') {
			if (piece / 12 == kingPos / 12)
				offset = (kingPos - piece) / std::abs(kingPos - piece);
			else if (piece % 12 == kingPos % 12)
				offset = (kingPos - piece) / std::abs(kingPos - piece) * 12;
		}
		
		if (pieceType == 'b' || pieceType == 'q') {
			if ((kingPos - piece) % 11 == 0)
				offset = (kingPos - piece) / std::abs(kingPos - piece) * 11;
			else if ((kingPos - piece) % 13 == 0)
				offset = (kingPos - piece) / std::abs(kingPos - piece) * 13;
		}

		if (offset == 0)
			continue;
		
		bool isPinned = false;
		bool used = false;

		checks::Check pin = { 0, 0, 0 };

		for (int newSquare = piece + offset; board::board[newSquare] != -1; newSquare += offset) {
			if (board::board[newSquare] == 0 || board::board[newSquare] == -1)
				continue;

			if ((bool) islower(board::board[newSquare]) == board::turn) {
				if (tolower(board::board[newSquare]) == 'k') {
					if (used)
						isPinned = true;
					break;
				}

				if (used)
					break;
				used = true;
				pin = { newSquare, -1, std::abs(offset) };
			}
			else
				break;
		}
		if (isPinned)
			pinned->push_back(pin);
	}
}

void checks::getBlocks(std::vector<int>* blocks, std::vector<checks::Check>* attacked) {
	for (checks::Check check : *attacked) {
		if (check.coord != board::king[board::turn])
			continue;

		if (check.axis == -10) {
			blocks->push_back(check.original);
			break;
		}

		int i = check.coord + check.axis;
		while (!board::board[i]) {
			blocks->push_back(i);
			i += check.axis;
		}
		blocks->push_back(i);
	}
}
