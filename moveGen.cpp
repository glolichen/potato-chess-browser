#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include "attacked.h"
#include "board.h"
#include "checks.h"
#include "moveGen.h"
#include "moves.h"
#include "pieceMoves.h"
#include "perft.h"

const std::map<char, int> PIECE_TABLE = { { 'p', 0 },
                                    { 'n', 1 },
                                    { 'b', 2 },
                                    { 'r', 3 },
                                    { 'q', 4 },
                                    { 'k', 5 } };
const int PIECE_VALUES[] = { 100, 305, 333, 563, 950, 100 };

bool sortMoveOrder(std::pair<moves::Move, int> o1, std::pair<moves::Move, int> o2) {
    return o1.second > o2.second;
}

std::vector<moves::Move> moveGen::moveGen() {
    std::vector<moves::Move> moves;

    std::vector<checks::Check> attacked = attacked::getAttacked();
    std::vector<checks::Check> pinned = checks::getPinned();

    if (board::enPassant != -1) {
        int dir = 1;
        if (board::turn)
            dir = -1;

        int center = board::enPassant + (dir * 12);

        int pawn = 'P';
        if (board::turn)
            pawn = 'p';

        if (board::board[center - 1] == pawn) {
            moves::Move move = { center - 1, board::enPassant, board::board[center], 0, 0, 0, 1 };
            makeMove(move);
            board::turn = !board::turn;
            if (checks::enPassantLegal())
                moves.push_back(move);
            board::turn = !board::turn;
            unmakeMove(move);
        }
        if (board::board[center + 1] == pawn) {
            moves::Move move = { center + 1, board::enPassant, board::board[center], 0, 0, 0, 1 };
            makeMove(move);
            board::turn = !board::turn;
            if (checks::enPassantLegal())
                moves.push_back(move);
            board::turn = !board::turn;
            unmakeMove(move);
        }
    }

    for (int i : board::pieces[board::turn]) {
        char piece = tolower(board::board[i]);

        switch (piece) {
            case 'q': {
                pieceMoves::rmoves(i, &moves, pinned, false);
                pieceMoves::bmoves(i, &moves, pinned);
                break;
            }

            case 'r': {
                pieceMoves::rmoves(i, &moves, pinned, true);
                break;
            }

            case 'b': {
                pieceMoves::bmoves(i, &moves, pinned);
                break;
            }

            case 'n': {
                pieceMoves::nmoves(i, &moves, pinned);
                break;
            }

            case 'p': {
                pieceMoves::pmoves(i, &moves, pinned);
                break;
            }

            case 'k': {
                pieceMoves::kmoves(i, &moves, attacked);
                break;
            }
        }
    }

    char king = board::turn ? 'k' : 'K';
    int timesChecked = 0;
    for (checks::Check c : attacked) {
        if (board::board[c.coord] == king)
            timesChecked++;
    }

    if (timesChecked == 0)
        return moves;

    std::vector<int> blocks = checks::getBlocks(attacked);
    std::vector<moves::Move> result;

    if (timesChecked == 2)
        blocks.clear();

    for (moves::Move move : moves) {
        if (board::board[move.source] == king)
            result.push_back(move);
        else if (count(blocks.begin(), blocks.end(), move.dest))
            result.push_back(move);
        else if (move.isEp)
            result.push_back(move);
    }

    return result;
}

std::vector<moves::Move> moveGen::moveGenWithOrdering() {
    std::vector<moves::Move> moves = moveGen();

    std::vector<std::pair<moves::Move, int>> score;
    for (moves::Move m : moves) {
        int estimatedScore = 0;

        if (m.capture) {
            int movedPiece = PIECE_TABLE.find(tolower(board::board[m.source]))->second;
            int capturedPiece = PIECE_TABLE.find(tolower(m.capture))->second;

            estimatedScore = std::max(capturedPiece - movedPiece, 1) * capturedPiece;;
        }
        if (m.promote)
            estimatedScore += PIECE_TABLE.find(tolower(m.promote))->second;

        score.push_back({m, estimatedScore});
    }

    sort(score.begin(), score.end(), sortMoveOrder);

    std::vector<moves::Move> result;
    for (std::pair<moves::Move, int> p : score)
        result.push_back(p.first);

    return result;
}