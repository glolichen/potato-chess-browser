#include <iostream>

#include "bitboard.h"
#include "move.h"

void move::printMove(int move, bool newLine) {
	std::cout << bitboard::squares[63 - SOURCE(move)];

	int dest = DEST(move);
	std::cout << bitboard::squares[63 - dest];

	int promote = PROMOTE(move);
	if (promote)
		std::cout << bitboard::pieces[PROMOTE(move) + bool(dest / 8) * 6];

	if (newLine)
		std::cout << "\n";
}

void move::makeMove(bitboard::Position &board, int move) {
	int source = SOURCE(move);
	int dest = DEST(move);
	int promote = PROMOTE(move);
	board.enPassant = -1;

	if (IS_EP(move)) {
		int captured = dest + (board.turn ? 8 : -8);
		SET0(board.allPieces, captured);
		if (board.turn) {
			SET0(board.pieces[WHITE][PAWN], captured);
			SET0(board.pieces[WHITE][ALL], captured);
		}
		else {
			SET0(board.pieces[BLACK][PAWN], captured);
			SET0(board.pieces[BLACK][ALL], captured);
		}

		board.mailbox[captured] = -1;
	}

	if (board.turn && board.mailbox[source] == PAWN + 6 && source - dest == 16) {
		if (board.mailbox[dest - 1] == PAWN || board.mailbox[dest + 1] == PAWN)
			board.enPassant = dest + 8;
	}
	else if (!board.turn && board.mailbox[source] == PAWN && dest - source == 16) {
		if (board.mailbox[dest - 1] == PAWN + 6 || board.mailbox[dest + 1] == PAWN + 6)
			board.enPassant = dest - 8;
	}

	if (promote) {
		SET1(board.allPieces, dest);
		SET0(board.allPieces, source);
		if (board.turn) {
			if (board.mailbox[dest] != -1) {
				SET0(board.pieces[WHITE][board.mailbox[dest]], dest);
				SET0(board.pieces[WHITE][ALL], dest);
			}
			SET1(board.pieces[BLACK][promote], dest);
			SET0(board.pieces[BLACK][PAWN], source);
			SET1(board.pieces[BLACK][ALL], dest);
			SET0(board.pieces[BLACK][ALL], source);
			promote += 6;
		}
		else {
			if (board.mailbox[dest] != -1) {
				SET0(board.pieces[BLACK][board.mailbox[dest] - 6], dest);
				SET0(board.pieces[BLACK][ALL], dest);
			}
			SET1(board.pieces[WHITE][promote], dest);
			SET0(board.pieces[WHITE][PAWN], source);
			SET1(board.pieces[WHITE][ALL], dest);
			SET0(board.pieces[WHITE][ALL], source);
		}

		if (dest == 0 && board.mailbox[dest] == ROOK)
			board.castle[0] = false;
		else if (dest == 7 && board.mailbox[dest] == ROOK)
			board.castle[1] = false;
		else if (dest == 56 && board.mailbox[dest] == ROOK + 6)
			board.castle[2] = false;
		else if (dest == 63 && board.mailbox[dest] == ROOK + 6)
			board.castle[3] = false;

		board.mailbox[dest] = promote;
		board.mailbox[source] = -1;

		board.turn = !board.turn;
		return;
	}

	SET1(board.allPieces, dest);
	SET0(board.allPieces, source);
	if (board.turn) {
		if (board.mailbox[dest] != -1) {
			SET0(board.pieces[WHITE][board.mailbox[dest]], dest);
			SET0(board.pieces[WHITE][ALL], dest);
		}
		SET1(board.pieces[BLACK][board.mailbox[source] - 6], dest);
		SET0(board.pieces[BLACK][board.mailbox[source] - 6], source);
		SET1(board.pieces[BLACK][ALL], dest);
		SET0(board.pieces[BLACK][ALL], source);
	}
	else {
		if (board.mailbox[dest] != -1) {
			SET0(board.pieces[BLACK][board.mailbox[dest] - 6], dest);
			SET0(board.pieces[BLACK][ALL], dest);
		}
		SET1(board.pieces[WHITE][board.mailbox[source]], dest);
		SET0(board.pieces[WHITE][board.mailbox[source]], source);
		SET1(board.pieces[WHITE][ALL], dest);
		SET0(board.pieces[WHITE][ALL], source);
	}

	if (board.mailbox[source] == KING || board.mailbox[source] == KING + 6) {
		board.castle[0 + board.turn * 2] = false;
		board.castle[1 + board.turn * 2] = false;
	}
	else if (source == 0 && board.mailbox[source] == ROOK)
		board.castle[0] = false;
	else if (source == 7 && board.mailbox[source] == ROOK)
		board.castle[1] = false;
	else if (source == 56 && board.mailbox[source] == ROOK + 6)
		board.castle[2] = false;
	else if (source == 63 && board.mailbox[source] == ROOK + 6)
		board.castle[3] = false;
	else if (dest == 0 && board.mailbox[dest] == ROOK)
		board.castle[0] = false;
	else if (dest == 7 && board.mailbox[dest] == ROOK)
		board.castle[1] = false;
	else if (dest == 56 && board.mailbox[dest] == ROOK + 6)
		board.castle[2] = false;
	else if (dest == 63 && board.mailbox[dest] == ROOK + 6)
		board.castle[3] = false;

	int castle = CASTLE(move);
	if (castle) {
		int rookSource, rookDest;
		if (board.turn) {
			rookSource = castle == 1 ? 56 : 63;
			rookDest = castle == 1 ? 58 : 60;
			SET1(board.pieces[BLACK][ROOK], rookDest);
			SET0(board.pieces[BLACK][ROOK], rookSource);
			SET1(board.pieces[BLACK][ALL], rookDest);
			SET0(board.pieces[BLACK][ALL], rookSource);
		}
		else {
			rookSource = castle == 1 ? 0 : 7;
			rookDest = castle == 1 ? 2 : 4;
			SET1(board.pieces[WHITE][ROOK], rookDest);
			SET0(board.pieces[WHITE][ROOK], rookSource);
			SET1(board.pieces[WHITE][ALL], rookDest);
			SET0(board.pieces[WHITE][ALL], rookSource);
		}

		SET1(board.allPieces, rookDest);
		SET0(board.allPieces, rookSource);

		board.mailbox[rookDest] = ROOK + (board.turn ? 6 : 0);
		board.mailbox[rookSource] = -1;
	}

	board.mailbox[dest] = board.mailbox[source];
	board.mailbox[source] = -1;

	board.turn = !board.turn;
}