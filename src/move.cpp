#include <iostream>

#include "bitboard.h"
#include "move.h"

void move::print_move(int move, bool newLine) {
	std::cout << bitboard::squares[63 - SOURCE(move)];

	int dest = DEST(move);
	std::cout << bitboard::squares[63 - dest];

	int promote = PROMOTE(move);
	if (promote)
		std::cout << bitboard::pieces[PROMOTE(move) + bool(dest / 8) * 6];

	if (newLine)
		std::cout << "\n";
}

void move::make_move(bitboard::Position &board, int move) {
	int source = SOURCE(move);
	int dest = DEST(move);
	int promote = PROMOTE(move);
	board.en_passant = -1;

	if (IS_EP(move)) {
		int captured = dest + (board.turn ? 8 : -8);
		SET0(board.all_pieces, captured);
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
			board.en_passant = dest + 8;
	}
	else if (!board.turn && board.mailbox[source] == PAWN && dest - source == 16) {
		if (board.mailbox[dest - 1] == PAWN + 6 || board.mailbox[dest + 1] == PAWN + 6)
			board.en_passant = dest - 8;
	}

	if (promote) {
		SET1(board.all_pieces, dest);
		SET0(board.all_pieces, source);
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

	SET1(board.all_pieces, dest);
	SET0(board.all_pieces, source);
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
		int rook_source, rook_dest;
		if (board.turn) {
			rook_source = castle == 1 ? 56 : 63;
			rook_dest = castle == 1 ? 58 : 60;
			SET1(board.pieces[BLACK][ROOK], rook_dest);
			SET0(board.pieces[BLACK][ROOK], rook_source);
			SET1(board.pieces[BLACK][ALL], rook_dest);
			SET0(board.pieces[BLACK][ALL], rook_source);
		}
		else {
			rook_source = castle == 1 ? 0 : 7;
			rook_dest = castle == 1 ? 2 : 4;
			SET1(board.pieces[WHITE][ROOK], rook_dest);
			SET0(board.pieces[WHITE][ROOK], rook_source);
			SET1(board.pieces[WHITE][ALL], rook_dest);
			SET0(board.pieces[WHITE][ALL], rook_source);
		}

		SET1(board.all_pieces, rook_dest);
		SET0(board.all_pieces, rook_source);

		board.mailbox[rook_dest] = ROOK + (board.turn ? 6 : 0);
		board.mailbox[rook_source] = -1;
	}

	board.mailbox[dest] = board.mailbox[source];
	board.mailbox[source] = -1;

	board.turn = !board.turn;
}

std::string move::make_move_for_JS(std::string fen, move::Move move) {
	bitboard::decode(fen);

	move.source = 63 - move.source;
	move.dest = 63 - move.dest;
	int moveInt = NEW_MOVE(move.source, move.dest, move.castle, move.promote, move.isEp);

	move::make_move(bitboard::board, moveInt);
	return bitboard::encode(bitboard::board);
}