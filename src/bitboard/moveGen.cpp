#include <cstring>
#include <iostream>
#include <vector>

#include "bitboard.h"
#include "maps.h"
#include "moveGen.h"
#include "move.h"

ull getPawnMoves(bitboard::Position *board, int square) {
	int multiplier = board->turn ? -1 : 1;
	if (QUERY(board->allPieces, square + 8 * multiplier))
		return 0ull;
	ull moves = (1ull << (square + 8 * multiplier));
	if ((square / 8 == 1 && !board->turn) || (square / 8 == 6 && board->turn))
		moves |= (1ull << (square + 16 * multiplier));

	ull answer = moves & ~board->allPieces;
	return answer;
}

ull getPawnAttacks(int square, bool color) {
	ull attacks = 0;
	if (color) {
		if (square % 8 > 0)
			SET1(attacks, square - 9);
		if (square % 8 < 7)
			SET1(attacks, square - 7);
	}
	else {
		if (square % 8 > 0)
			SET1(attacks, square + 7);
		if (square % 8 < 7)
			SET1(attacks, square + 9);
	}
	return attacks;
}
ull getKnightAttacks(int square) {
	return maps::knight[square];
}
ull getBishopAttacks(bitboard::Position *board, ull *piecesNoKing, int square) {
	ull attacks, blockers;

	blockers = maps::bishop[square][0] & *piecesNoKing;
	if (blockers)
		attacks = ~maps::bishop[__builtin_ctzll(blockers)][0] & maps::bishop[square][0];
	else
		attacks = maps::bishop[square][0];
	
	blockers = maps::bishop[square][1] & *piecesNoKing;
	if (blockers)
		attacks |= ~maps::bishop[63 - __builtin_clzll(blockers)][1] & maps::bishop[square][1];
	else
		attacks |= maps::bishop[square][1];

	blockers = maps::bishop[square][2] & *piecesNoKing;
	if (blockers)
		attacks |= ~maps::bishop[63 - __builtin_clzll(blockers)][2] & maps::bishop[square][2];
	else
		attacks |= maps::bishop[square][2];

	blockers = maps::bishop[square][3] & *piecesNoKing;
	if (blockers)
		attacks |= ~maps::bishop[__builtin_ctzll(blockers)][3] & maps::bishop[square][3];
	else
		attacks |= maps::bishop[square][3];

	return attacks;
}
ull getRookAttacks(bitboard::Position *board, ull *piecesNoKing, int square) {
	ull attacks, blockers;
	
	blockers = maps::rook[square][0] & *piecesNoKing;
	if (blockers)
		attacks = ~maps::rook[__builtin_ctzll(blockers)][0] & maps::rook[square][0];
	else
		attacks = maps::rook[square][0];
	
	blockers = maps::rook[square][1] & *piecesNoKing;
	if (blockers)
		attacks |= ~maps::rook[63 - __builtin_clzll(blockers)][1] & maps::rook[square][1];
	else
		attacks |= maps::rook[square][1];

	blockers = maps::rook[square][2] & *piecesNoKing;
	if (blockers)
		attacks |= ~maps::rook[63 - __builtin_clzll(blockers)][2] & maps::rook[square][2];
	else
		attacks |= maps::rook[square][2];

	blockers = maps::rook[square][3] & *piecesNoKing;
	if (blockers)
		attacks |= ~maps::rook[__builtin_ctzll(blockers)][3] & maps::rook[square][3];
	else
		attacks |= maps::rook[square][3];

	return attacks;
}
ull getQueenAttacks(bitboard::Position *board, ull *piecesNoKing, int square) {
	return getBishopAttacks(board, piecesNoKing, square) | getRookAttacks(board, piecesNoKing, square);
}
ull getKingAttacks(int square) {
	ull attacks = 0;

	bool topEdge = square / 8 > 0;
	bool rightEdge = square % 8 < 7;
	bool bottomEdge = square / 8 < 7;
	bool leftEdge = square % 8 > 0;

	if (topEdge)
		SET1(attacks, square - 8);
	if (rightEdge)
		SET1(attacks, square + 1);
	if (bottomEdge)
		SET1(attacks, square + 8);
	if (leftEdge)
		SET1(attacks, square - 1);
	
	if (topEdge && rightEdge)
		SET1(attacks, square - 7);
	if (bottomEdge && rightEdge)
		SET1(attacks, square + 9);
	if (bottomEdge && leftEdge)
		SET1(attacks, square + 7);
	if (topEdge && leftEdge)
		SET1(attacks, square - 9);
	
	return attacks;
} 

ull getAttacked(bitboard::Position *board, bool color) {
	ull piecesNoKing = board->allPieces ^ board->pieces[color][KING];
	color = !color;
	ull pieces, attacks = 0;

	pieces = board->pieces[color][PAWN];
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		attacks |= getPawnAttacks(pos, color);
		SET0(pieces, pos);
	}

	pieces = board->pieces[color][KNIGHT];
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		attacks |= getKnightAttacks(pos);
		SET0(pieces, pos);
	}
	
	pieces = board->pieces[color][BISHOP];
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		attacks |= getBishopAttacks(board, &piecesNoKing, pos);
		SET0(pieces, pos);
	}

	pieces = board->pieces[color][ROOK];
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		attacks |= getRookAttacks(board, &piecesNoKing, pos);
		SET0(pieces, pos);
	}
	
	pieces = board->pieces[color][QUEEN];
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		attacks |= getQueenAttacks(board, &piecesNoKing, pos);
		SET0(pieces, pos);
	}
	
	pieces = board->pieces[color][KING];
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		attacks |= getKingAttacks(pos);
		SET0(pieces, pos);
	}

	return attacks;
}
ull moveGen::getChecks(bitboard::Position *board, bool color) {
	int kingPos = __builtin_ctzll(board->pieces[color][KING]);

	ull checks = getPawnAttacks(kingPos, color) & board->pieces[!color][PAWN];
	checks |= getKnightAttacks(kingPos) & board->pieces[!color][KNIGHT];
	checks |= getBishopAttacks(board, &board->allPieces, kingPos) &	(board->pieces[!color][BISHOP] | board->pieces[!color][QUEEN]);
	checks |= getRookAttacks(board, &board->allPieces, kingPos) & (board->pieces[!color][ROOK] | board->pieces[!color][QUEEN]);
	
	return checks;
}
ull getPinned(bitboard::Position *board, bool color) {
	int pinnedPiece, kingPos = __builtin_ctzll(board->pieces[color][KING]);;
	ull blockers, pinned = 0;
	ull ourPieces = board->pieces[color][ALL];
	ull rookqueen = board->pieces[!color][ROOK] | board->pieces[!color][QUEEN];
	ull bishopqueen = board->pieces[!color][BISHOP] | board->pieces[!color][QUEEN];
	
	blockers = maps::rook[kingPos][0] & board->allPieces;
	if (blockers) {
		pinnedPiece = __builtin_ctzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(rookqueen, 63 - __builtin_clzll(~maps::rook[__builtin_ctzll(blockers)][0] & maps::rook[kingPos][0]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::rook[kingPos][1] & board->allPieces;
	if (blockers) {
		pinnedPiece = 63 - __builtin_clzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(rookqueen, __builtin_ctzll(~maps::rook[63 - __builtin_clzll(blockers)][1] & maps::rook[kingPos][1]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::rook[kingPos][2] & board->allPieces;
	if (blockers) {
		pinnedPiece = 63 - __builtin_clzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(rookqueen, __builtin_ctzll(~maps::rook[63 - __builtin_clzll(blockers)][2] & maps::rook[kingPos][2]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::rook[kingPos][3] & board->allPieces;
	if (blockers) {
		pinnedPiece = __builtin_ctzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(rookqueen, 63 - __builtin_clzll(~maps::rook[__builtin_ctzll(blockers)][3] & maps::rook[kingPos][3]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::bishop[kingPos][0] & board->allPieces;
	if (blockers) {
		pinnedPiece = __builtin_ctzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(bishopqueen, 63 - __builtin_clzll(~maps::bishop[__builtin_ctzll(blockers)][0] & maps::bishop[kingPos][0]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::bishop[kingPos][1] & board->allPieces;
	if (blockers) {
		pinnedPiece = 63 - __builtin_clzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(bishopqueen, __builtin_ctzll(~maps::bishop[63 - __builtin_clzll(blockers)][1] & maps::bishop[kingPos][1]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::bishop[kingPos][2] & board->allPieces;
	if (blockers) {
		pinnedPiece = 63 - __builtin_clzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(bishopqueen, __builtin_ctzll(~maps::bishop[63 - __builtin_clzll(blockers)][2] & maps::bishop[kingPos][2]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}
	
	blockers = maps::bishop[kingPos][3] & board->allPieces;
	if (blockers) {
		pinnedPiece = __builtin_ctzll(blockers);
		SET0(blockers, pinnedPiece);
		if (QUERY(bishopqueen, 63 - __builtin_clzll(~maps::bishop[__builtin_ctzll(blockers)][3] & maps::bishop[kingPos][3]))) {
			ull potential = 1ull << pinnedPiece;
			if (potential & ourPieces)
				pinned |= potential;
		}
	}

	return pinned;
}

void moveGen::moveGen(bitboard::Position *board, std::vector<int> *moves) {
	moves->clear();

	ull attacked = getAttacked(board, board->turn);
	ull checks = getChecks(board, board->turn);
	ull pinned = getPinned(board, board->turn);
	ull pieces, blocks;

	int kingPos = __builtin_ctzll(board->pieces[board->turn][KING]);
	if (board->enPassant != -1) {
		int dir = board->turn ? 1 : -1;
		int center = board->enPassant + (dir * 8);

		int pawn = PAWN + (board->turn ? 6 : 0);

		if (board->mailbox[center - 1] == pawn && center % 8 > 0) {
			int move = NEW_MOVE(center - 1, board->enPassant, 0, 0, 1);

			bitboard::Position newBoard;
			memcpy(&newBoard, board, sizeof(*board));

			move::makeMove(&newBoard, move);
			if (!getChecks(&newBoard, !newBoard.turn))
				moves->push_back(move);
		}
		if (board->mailbox[center + 1] == pawn && center % 8 < 7) {
			int move = NEW_MOVE(center + 1, board->enPassant, 0, 0, 1);

			bitboard::Position newBoard;
			memcpy(&newBoard, board, sizeof(*board));

			move::makeMove(&newBoard, move);
			if (!getChecks(&newBoard, !newBoard.turn))
				moves->push_back(move);
		}
	}

	if (checks) {
		if (checks & (checks - 1)) { // double check
			ull attacks = getKingAttacks(kingPos) & ~attacked & ~board->pieces[board->turn][ALL];
			while (attacks) {
				int pos = __builtin_ctzll(attacks);
				moves->push_back(NEW_MOVE(kingPos, pos, 0, 0, 0));
				SET0(attacks, pos);
			}
			return;
		}

		int checkerPos = __builtin_ctzll(checks);
		if (board->mailbox[checkerPos] == KNIGHT || board->mailbox[checkerPos] == KNIGHT + 6)
			blocks = 1ull << checkerPos;
		else {
			try {
				ull filled = (1ull << checkerPos) | (1ull << kingPos);
				filled = maps::fill[__builtin_ctzll(filled)][63 - __builtin_clzll(filled)];
				blocks = maps::pinnedOffsets[kingPos].at(kingPos - checkerPos) & filled & ~board->pieces[board->turn][KING];
			}
			catch (...) {
				blocks = 1ull << checkerPos;
			}
		}
	}
	else
		blocks = ~0ull;

	pieces = board->pieces[board->turn][PAWN] & ~pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = ((getPawnAttacks(pos, board->turn) & board->pieces[!board->turn][ALL]) | 
			getPawnMoves(board, pos)) & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			switch (dest / 8) {
				case 0:
				case 7:
					moves->push_back(NEW_MOVE(pos, dest, 0, QUEEN, 0));
					moves->push_back(NEW_MOVE(pos, dest, 0, ROOK, 0));
					moves->push_back(NEW_MOVE(pos, dest, 0, BISHOP, 0));
					moves->push_back(NEW_MOVE(pos, dest, 0, KNIGHT, 0));
					break;
				default:
					moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			}
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][PAWN] & pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = ((getPawnAttacks(pos, board->turn) & board->pieces[!board->turn][ALL]) | 
			getPawnMoves(board, pos)) & blocks;
		attacks &= maps::pinnedOffsetsAll[kingPos].at(kingPos - pos);
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			switch (dest / 8) {
				case 0:
				case 7:
					moves->push_back(NEW_MOVE(pos, dest, 0, QUEEN, 0));
					moves->push_back(NEW_MOVE(pos, dest, 0, ROOK, 0));
					moves->push_back(NEW_MOVE(pos, dest, 0, BISHOP, 0));
					moves->push_back(NEW_MOVE(pos, dest, 0, KNIGHT, 0));
					break;
				default:
					moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			}
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][KNIGHT] & ~pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getKnightAttacks(pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][BISHOP] & ~pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getBishopAttacks(board, &board->allPieces, pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][BISHOP] & pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getBishopAttacks(board, &board->allPieces, pos) & maps::pinnedOffsetsAll[pos].at(kingPos - pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][ROOK] & ~pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getRookAttacks(board, &board->allPieces, pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][ROOK] & pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getRookAttacks(board, &board->allPieces, pos) & maps::pinnedOffsetsAll[pos].at(kingPos - pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][QUEEN] & ~pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getQueenAttacks(board, &board->allPieces, pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	pieces = board->pieces[board->turn][QUEEN] & pinned;
	while (pieces) {
		int pos = __builtin_ctzll(pieces);
		ull attacks = getQueenAttacks(board, &board->allPieces, pos) & maps::pinnedOffsetsAll[pos].at(kingPos - pos) & ~board->pieces[board->turn][ALL] & blocks;
		while (attacks) {
			int dest = __builtin_ctzll(attacks);
			moves->push_back(NEW_MOVE(pos, dest, 0, 0, 0));
			SET0(attacks, dest);
		}
		SET0(pieces, pos);
	}

	ull attacks = getKingAttacks(kingPos) & ~attacked & ~board->pieces[board->turn][ALL];
	while (attacks) {
		int dest = __builtin_ctzll(attacks);
		moves->push_back(NEW_MOVE(kingPos, dest, 0, 0, 0));
		SET0(attacks, dest);
	}

	if (!checks) {
		if (board->turn) {
			if (board->castle[2] && !(attacked & 432345564227567616ull) && !(board->allPieces & 432345564227567616ull))
				moves->push_back(NEW_MOVE(kingPos, 57, 1, 0, 0));
			if (board->castle[3] && !(attacked & 3458764513820540928ull) && !(board->allPieces & 8070450532247928832ull))
				moves->push_back(NEW_MOVE(kingPos, 61, 2, 0, 0));
		}
		else {
			if (board->castle[0] && !(attacked & 6ull) && !(board->allPieces & 6ull))
				moves->push_back(NEW_MOVE(kingPos, 1, 1, 0, 0));
			if (board->castle[1] && !(attacked & 48ull) && !(board->allPieces & 112ull))
				moves->push_back(NEW_MOVE(kingPos, 5, 2, 0, 0));
		}
	}
}