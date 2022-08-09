#include <iostream>
#include <string>
#include <vector>

#include "bitboard.h"

std::string bitboard::squares[64] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};
std::string bitboard::pieces = "PNBRQKpnbrqk";
bitboard::Position bitboard::board;

std::vector<std::string> bitboard::split(std::string str, char splitOn) {
	std::vector<std::string> result;

	int left = 0;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == splitOn) {
			result.push_back(str.substr(left, i - left));
			left = i + 1;
		}
	}

	result.push_back(str.substr(left, str.length() - left));
	return result;
}

void bitboard::decode(std::string fen) {
	for (int i = 0; i < 64; i++)
		board.mailbox[i] = -1;
	board.allPieces = 0ull;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 7; j++)
			board.pieces[i][j] = 0ull;
	}
	for (int i = 0; i < 4; i++)
		board.castle[i] = false;
	board.turn = 0;
	board.enPassant = -1;
	board.fiftyMoveClock = 0;
	board.fullMove = 0;

	std::vector<std::string> splitted = bitboard::split(fen, ' ');

	if (!splitted[1].compare("w"))
		board.turn = WHITE;
	else
		board.turn = BLACK;

	try {
		board.fiftyMoveClock = stoi(splitted[4]);
		board.fullMove = stoi(splitted[5]);
	}
	catch (...) { }


	std::string castle = splitted[2];
	if (castle.find('K') != std::string::npos)
		board.castle[0] = true;
	if (castle.find('Q') != std::string::npos)
		board.castle[1] = true;
	if (castle.find('k') != std::string::npos)
		board.castle[2] = true;
	if (castle.find('q') != std::string::npos)
		board.castle[3] = true;

	std::string ep = splitted[3];
	if (ep.compare("-")) {
		for (int i = 0; i < 64; i++) {
			if (ep == squares[i]) {
				board.enPassant = 63 - i;
				break;
			}
		}
	}
	else
		board.enPassant = -1;

	std::vector<std::string> line = split(splitted[0], '/');

	for (int i = 0; i < 8; i++) {
		int cur = 0;
		std::string curRank = line[i];
		for (int j = 0; j < curRank.size(); j++) {
			if (isdigit(curRank[j])) {
				cur += curRank[j] - '0';
				continue;
			}

			SET1(board.allPieces, 63 - (i * 8 + cur));

			int piece = bitboard::pieces.find(curRank[j]);
			board.mailbox[63 - (i * 8 + cur)] = piece;
			if (piece >= 6)
				SET1(board.pieces[BLACK][piece - 6], 63 - (i * 8 + cur));
			else
				SET1(board.pieces[WHITE][piece], 63 - (i * 8 + cur));

			cur++;
		}
	}

	for (int i = 0; i < 6; i++) {
		board.pieces[WHITE][ALL] |= board.pieces[WHITE][i];
		board.pieces[BLACK][ALL] |= board.pieces[BLACK][i];
	}
}
std::string bitboard::encode(Position *board) {
	std::string fen = "";
	for (int i = 0; i < 8; i++) {
		int empty = 0;
		for (int j = 0; j < 8; j++) {
			int index = 63 - (i * 8 + j);
			if (QUERY(board->allPieces, index)) {
				if (empty != 0)
					fen += empty + '0';
				empty = 0;
				for (int k = 0; k < 6; k++) {
					if (QUERY(board->pieces[WHITE][k], index)) {
						fen += bitboard::pieces[k];
						break;
					}
				}
				for (int k = 0; k < 6; k++) {
					if (QUERY(board->pieces[BLACK][k], index)) {
						fen += bitboard::pieces[k + 6];
						break;
					}
				}
			}
			else
				empty++;
		}

		if (empty != 0)
			fen += empty + '0';

		if (i < 7)
			fen += "/";
	}
	fen += " ";

	fen += board->turn ? 'b' : 'w';
	fen += " ";

	std::string castleRights = "";
	if (board->castle[0])
		castleRights += "K";
	if (board->castle[1])
		castleRights += "Q";
	if (board->castle[2])
		castleRights += "k";
	if (board->castle[3])
		castleRights += "q";
	if (castleRights == "")
		castleRights = "-";

	fen += castleRights + " ";

	if (board->enPassant == -1)
		fen += "-";
	else
		fen += bitboard::squares[board->enPassant];
		
	fen += " " + std::to_string(board->fiftyMoveClock);
	fen += " " + std::to_string(board->fullMove);

	return fen;
}

void bitboard::printBoard(Position *board) {
	std::cout << "╭───┬───┬───┬───┬───┬───┬───┬───╮" << "\n";

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int index = 63 - (i * 8 + j);
			char print = ' ';

			if (QUERY(board->allPieces, index)) {
				for (int k = 0; k < 6; k++) {
					if (QUERY(board->pieces[0][k], index)) {
						std::cout << "│ " << bitboard::pieces[k] << " ";
						break;
					}
				}
				for (int k = 0; k < 6; k++) {
					if (QUERY(board->pieces[1][k], index)) {
						std::cout << "│ " << bitboard::pieces[k + 6] << " ";
						break;
					}
				}
			}
			else
				std::cout << "│   ";
		}

		std::cout << "│";

		if (i < 7)
			std::cout << "\n├───┼───┼───┼───┼───┼───┼───┼───┤\n";
		else
			std::cout << "\n╰───┴───┴───┴───┴───┴───┴───┴───╯\n";
	}

	std::cout << "\nFEN: " << encode(board);
	std::cout << "\nWhite Kingside: " << board->castle[0];
	std::cout << "    White Queenside: " << board->castle[1];
	std::cout << "\nBlack Kingside: " << board->castle[2];
	std::cout << "    Black Queenside: " << board->castle[3];

	std::cout << "\nEn Passant Square: " << (board->enPassant == -1 ? "None" : bitboard::squares[board->enPassant]);
	std::cout << "\nTurn: " << (board->turn ? "Black" : "White");

	std::cout << "\n";
}