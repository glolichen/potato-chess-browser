#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <unordered_map>
#include <vector>

#include "attacked.h"
#include "board.h"
#include "constants.h"
#include "eval.h"
#include "hashing.h"
#include "moveGen.h"
#include "moves.h"
#include "search.h"

struct TTEntry {
    int depth;
    int eval;
};

const int SEARCH_EXPIRED = INT_MIN + 500;

bool topMoveNull;
moves::Move search::topMove; // top move stored through iterative deepening
std::unordered_map<unsigned long long, TTEntry> transposition; // transposition table

int limit;

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
    int now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (now >= limit)
        return SEARCH_EXPIRED;

    int evaluation = 0;
    if (depth == 0)
        return eval::evaluate();

    std::vector<moves::Move> moves = moveGen::moveGenWithOrdering();

    // place the previous best move to the top of the list
    // this should speed up alpha-beta pruning by allowing us to prune most if not all branches
    if (depthFromStart == 0 && !topMoveNull) {
        for (int i = 0; i < static_cast<int>(moves.size()); i++) {
            if (sameMove(moves[i], topMove)) {
                moves.erase(moves.begin() + i);
                break;
            }
        }
        moves.insert(moves.begin(), topMove);
    }

    // unsigned long long hashCode = hashing::getZobristHash();

    if (moves.size() == 0) { // check whether there are legal moves
        char king = board::turn ? 'k' : 'K';
        bool inCheck = false;

        for (checks::Check c : attacked::getAttacked()) {
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

    // if (transposition.count(hashCode)) {
    //     auto found = transposition.at(hashCode);
    //     if (found.depth >= depth)
    //         return found.eval;
    // }

    if (board::turn == constants::WHITE && depth > 0) { // white's turn - computer tries to maximize evaluation
        evaluation = INT_MIN;

        std::vector<std::pair<moves::Move, int>> allEval;

        for (const moves::Move &m: moves) {
            moves::makeMove(m);

            int curEval = search::minimax(depth - 1, alpha, beta, depthFromStart + 1);

            moves::unmakeMove(m);

            if (curEval == SEARCH_EXPIRED)
                return SEARCH_EXPIRED;

            evaluation = std::max(curEval, evaluation);

            if (depthFromStart == 0)
                allEval.push_back({ m, curEval });

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
            moves::makeMove(m);

            int curEval = search::minimax(depth - 1, alpha, beta, depthFromStart + 1);

            moves::unmakeMove(m);

            if (curEval == SEARCH_EXPIRED)
                return SEARCH_EXPIRED;

            evaluation = std::min(curEval, evaluation);

            if (depthFromStart == 0)
                allEval.push_back({ m, curEval });

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

    // if (transposition.count(hashCode) == 0)
    //     transposition.insert({ hashCode, { depth, evaluation } });
    // else {
    //     auto previous = transposition.at(hashCode);
    //     if (depth > previous.depth)
    //         transposition.at(hashCode) = { depth, evaluation };
    // }

    return evaluation;
}

moves::Move search::search(int timeMS) {
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

    std::pair<moves::Move, int> best;
    limit = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    limit += timeMS;
    std::cout << "Depth: ";

    hashing::initZobristTable();
    eval::initPieceTables();

    for (int depth = 1; depth < INT_MAX; depth++) {
        int eval = search::minimax(depth, INT_MIN, INT_MAX, 0);
        topMoveNull = false;

        if (eval == SEARCH_EXPIRED) {
            std::cout << depth - 1 << "\n";
            break;
        }

        best.first = topMove;
        best.second = eval;

        if (evalIsMate(eval)) { // checkmate has been found, do not need to search any more
            std::cout << depth << " (Mate in " << depth - 1 << " found)\n";
            break;
        }
    }

    transposition.clear();
    
    std::cout << "Move: ";
    best.first.print(true);

    return best.first;
}

bool search::evalIsMate(int eval) {
    return (eval > INT_MAX - 250) || (eval < INT_MIN + 250);
}
