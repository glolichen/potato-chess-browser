#include <vector>
#include <algorithm>
#include <iostream>
#include <array>
#include <unordered_map>
#include <chrono>
#include <cstdlib>
#include <limits.h>

#include "search.h"
#include "eval.h"
#include "moveGen.h"
#include "moves.h"
#include "board.h"
#include "hashing.h"
#include "book.h"
#include "printing.h"
#include "constants.h"

struct TTEntry {
    int depth;
    int eval;
};

const int SEARCH_EXPIRED = INT_MIN + 500;
const int BOOK_MOVE = INT_MAX - 500;

bool topMoveNull;
moves::Move search::topMove; // top move stored through iterative deepening
std::unordered_map<unsigned long long, TTEntry> transposition; // transposition table

unsigned long long limit; // time limit

bool sameMove(moves::Move move1, moves::Move move2) { // are two moves the same
    return (move1.capture == move2.capture) &&
           (move1.castle == move2.castle) &&
           (move1.dest == move2.dest) &&
           (move1.isEp == move2.isEp) &&
           (move1.promote == move2.promote) &&
           (move1.signal == move2.signal) &&
           (move1.source == move2.source);
}

int search::minimax(int depth, int alpha, int beta, int depthFromStart) {
    //check if we have reached the time limit and should end the search
    unsigned long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    if (millis >= limit)
        return SEARCH_EXPIRED;

    std::string truncatedFEN = board::truncateFEN(board::encode());

    //opening book
    if (depthFromStart == 0 && board::halfMoveClock <= 8) {
        for (const auto &pair: book::openingBook) {
            if (pair.first == truncatedFEN) {
                std::vector<moves::SimpleMove> candidateMoves = pair.second; //all the possible moves from the opening book
                srand(time(0));
                int number = rand() % (candidateMoves.size()); //choose random move from book
                moves::SimpleMove bookMove = candidateMoves[number];

                std::vector<moves::Move> moves = moveGen::moveGen();
                for (const moves::Move &move: moves) {
                    if (move.source == bookMove.source &&
                        move.dest == bookMove.dest) {
                        search::topMove = move;
                        return BOOK_MOVE;
                    }
                }
            }
        }
    }

    int evaluation = 0;
    if (depth == 0)
        return eval::evaluate();

    std::vector<moves::Move> moves = moveGen::moveGenWithOrdering();

    if (depthFromStart == 0 && !topMoveNull) {
        for (int i = 0; i < static_cast<int>(moves.size()); i++) {
            if (sameMove(moves[i], topMove)) {
                moves.erase(moves.begin() + i);
                break;
            }
        }
        moves.insert(moves.begin(), topMove);
    }

    unsigned long long hashCode = hashing::getZobristHash();

    if (moves.size() == 0) { // check whether there are legal moves
        char king = board::turn ? 'k' : 'K';
        bool inCheck = false;

        for (checks::Check c : checks::getAttacked()) {
            if (board::board[c.coord] == king) {
                inCheck = true;
                break;
            }
        }

        int multiply = 1;
        if (board::turn == constants::WHITE)
            multiply = -1;

        if (inCheck)
            return INT_MAX * multiply - multiply * depthFromStart; // checkmate
        else
            return 0; // stalemate (draw)
    }

    if (transposition.count(hashCode)) {
        auto found = transposition.at(hashCode);
        if (found.depth >= depth)
            return found.eval;
    }

    if (board::turn == constants::WHITE && depth > 0) { // white's turn - computer tries to maximize evaluation
        evaluation = INT_MIN;

        std::vector<std::pair<moves::Move, int>> allEval;

        for (const moves::Move &m: moves) {
            makeMove(m);

            int curEval = minimax(depth - 1, alpha, beta, depthFromStart + 1);

            unmakeMove(m);

            if (curEval == SEARCH_EXPIRED)
                return SEARCH_EXPIRED;

            evaluation = std::max(curEval, evaluation);

            if (depthFromStart == 0)
                allEval.push_back({m, curEval});

            alpha = std::max(curEval, alpha);
            if (beta <= alpha)
                break;
        }

        if (depthFromStart == 0) {
            int topEval = INT_MIN;
            for (const auto &p: allEval) {
                if (p.second > topEval) {
                    topEval = p.second;
                    topMove = p.first;
                }
            }
            return topEval;
        }
    }
    else if (board::turn == constants::BLACK && depth > 0) { // black's turn - computer tries to minimize evaluation
        evaluation = INT_MAX;

        std::vector<std::pair<moves::Move, int>> allEval;

        for (const moves::Move &m: moves) {
            makeMove(m);

            int curEval = minimax(depth - 1, alpha, beta, depthFromStart + 1);

            unmakeMove(m);

            if (curEval == SEARCH_EXPIRED)
                return SEARCH_EXPIRED;

            evaluation = std::min(curEval, evaluation);

            if (depthFromStart == 0)
                allEval.push_back({m, curEval});

            beta = std::min(curEval, beta);
            if (beta <= alpha)
                break;
        }

        if (depthFromStart == 0) {
            int topEval = INT_MAX;
            for (const auto &p: allEval) {
                if (p.second < topEval) {
                    topEval = p.second;
                    topMove = p.first;
                }
            }
            return topEval;
        }
    }

    if (transposition.count(hashCode) == 0)
        transposition.insert({hashCode, {depth, evaluation}});
    else {
        auto previous = transposition.at(hashCode);
        if (depth > previous.depth)
            transposition.at(hashCode) = {depth, evaluation};
    }

    return evaluation;
}

std::pair<moves::Move, int> search::search(int timeMS) {
    //iterative deepening
    //search with depth of one ply first
    //then increase depth until time runs out
    //if time runs out in the middle of a search, terminate the search
    //and use the result from the previous search

    //it may seem like that the program is wasting a lot of time searching previously searched positions
    //but we can use alpha and beta values from before to speed up pruning
    //and we can search the best move first in the deeper search
    //transposition table is still intact which means that we can still use alpha and beta values from before

    topMoveNull = true;
    limit = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() +
            timeMS;

    std::pair<moves::Move, int> best;
    for (int depth = 1; depth < INT_MAX; depth++) {
        int eval = search::minimax(depth, INT_MIN, INT_MAX, 0);
        topMoveNull = false;

        if (eval == SEARCH_EXPIRED) {
            std::cout << "Depth " << depth << std::endl;
            break;
        }

        best.first = topMove;
        best.second = eval;

        if (evalIsMate(eval)) {
            std::cout << "Depth " << depth << std::endl;
            break;
        }
        if (eval == BOOK_MOVE) {
            std::cout << "Book Move" << std::endl;
            break;
        }
    }

    transposition.clear();

    return best;
}

bool search::evalIsMate(int eval) {
    return (eval > INT_MAX - 250) || (eval < INT_MIN + 250);
}
