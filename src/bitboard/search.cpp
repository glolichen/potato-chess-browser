#include <chrono>
#include <cstring>
#include <iostream>
#include <limits.h>
#include <map>
#include <tuple>
#include <vector>

#include "bitboard.h"
#include "eval.h"
#include "hash.h"
#include "maps.h"
#include "moveGen.h"
#include "search.h"

const int SEARCH_EXPIRED = INT_MIN + 500;

bool topMoveNull;
int bestMove;
std::map<std::tuple<ull, ull, ull>, int> transposition;

ull limit;

int search::minimax(bitboard::Position &board, int depth, int alpha, int beta, int depthFromStart) {
	//check if we have reached the time limit and should end the search
	ull now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (now >= limit)
		return SEARCH_EXPIRED;

	int evaluation = 0;
	if (depth == 0)
		return eval::evaluate(board);

	if (board.fiftyMoveClock >= 50)
		return 0;

	std::vector<int> moves;
	moveGen::moveGenWithOrdering(board, moves);

	// place the previous best move to the top of the list
	// this should speed up alpha-beta pruning by allowing us to prune most if not all branches
	if (depthFromStart == 0 && !topMoveNull) {
		for (int i = 0; i < moves.size(); i++) {
			if (moves[i] == bestMove) {
				moves.erase(moves.begin() + i);
				break;
			}
		}
		moves.insert(moves.begin(), bestMove);
	}

	if (moves.size() == 0) { // check whether there are legal moves
		if (moveGen::getChecks(board, board.turn)) {
			int multiply = board.turn ? 1 : -1;
			return INT_MAX * multiply - multiply * depthFromStart; // checkmate
		}
		return 0;
	}

	std::tuple<ull, ull, ull> hashes = hash::hash(board);
	if (transposition.count(hashes)) {
		int hashMove = transposition.at(hashes);
		for (int i = 0; i < moves.size(); i++) {
			if (moves[i] == hashMove) {
				moves.erase(moves.begin() + i);
				moves.insert(moves.begin(), hashMove);
				break;
			}
		}
	}

	int topMove;

	if (!board.turn && depth) { // white's turn - computer tries to maximize evaluation
		evaluation = INT_MIN;
		for (int move : moves) {
			if (board.mailbox[DEST(move)] != -1 || board.mailbox[SOURCE(move)] == PAWN || board.mailbox[SOURCE(move)] == PAWN + 6)
				board.fiftyMoveClock = 0;
			else
				board.fiftyMoveClock++;

			bitboard::Position newBoard;
			memcpy(&newBoard, &board, sizeof(board));
			move::makeMove(newBoard, move);

			int curEval = search::minimax(newBoard, depth - 1, alpha, beta, depthFromStart + 1);

			if (curEval == SEARCH_EXPIRED)
				return SEARCH_EXPIRED;

			if (curEval > evaluation) {
				evaluation = curEval;
				topMove = move;
			}

			alpha = std::max(curEval, alpha);
			if (beta <= alpha)
				break;
		}
	}
	else if (board.turn && depth) { // black's turn - computer tries to minimize evaluation
		evaluation = INT_MAX;
		for (int move : moves) {
			if (board.mailbox[DEST(move)] != -1 || board.mailbox[SOURCE(move)] == PAWN || board.mailbox[SOURCE(move)] == PAWN + 6)
				board.fiftyMoveClock = 0;
			else
				board.fiftyMoveClock++;

			bitboard::Position newBoard;
			memcpy(&newBoard, &board, sizeof(board));
			move::makeMove(newBoard, move);

			int curEval = search::minimax(newBoard, depth - 1, alpha, beta, depthFromStart + 1);

			if (curEval == SEARCH_EXPIRED)
				return SEARCH_EXPIRED;

			if (curEval < evaluation) {
				evaluation = curEval;
				topMove = move;
			}

			beta = std::min(curEval, beta);
			if (beta <= alpha)
				break;
		}
	}

	if (transposition.count(hashes) == 0)
		transposition.insert({ hashes, topMove });

	if (depthFromStart == 0)
		bestMove = topMove;

	return evaluation;
}

search::SearchResult search::search(std::string fen, int timeMS) {
	//iterative deepening
	//search with depth of one ply first
	//then increase depth until time runs out
	//if time runs out in the middle of a search, terminate the search
	//and use the result from the previous search

	//it may seem like that the program is wasting a lot of time searching previously searched positions
	//but we can use alpha and beta values from before to speed up pruning
	//and we can search the best move first in the deeper search
	//transposition table is still intact which means that we can still use alpha and beta values from before

	bitboard::decode(fen);

	topMoveNull = true;

	eval::init();
	hash::init();
	maps::init();

	std::pair<int, int> best;
	limit = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	limit += timeMS;

	int depth = 1;
	bool isMate = false;

	for (; depth <= 100; depth++) {
		int eval = search::minimax(bitboard::board, depth, INT_MIN, INT_MAX, 0);
		topMoveNull = false;

		if (eval == SEARCH_EXPIRED) {
			depth--;
			break;
		}

		best.first = bestMove;
		best.second = eval;

		if (evalIsMate(eval)) { // checkmate has been found, do not need to search any more
			isMate = true;
			break;
		}
	}

	transposition.clear();

	return { best.first, depth, best.second, isMate };
}

bool search::evalIsMate(int eval) {
	return (eval > INT_MAX - 250) || (eval < INT_MIN + 250);
}
