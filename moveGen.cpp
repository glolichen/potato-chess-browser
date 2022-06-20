#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

#include "attacked.h"
#include "board.h"
#include "checks.h"
#include "moveGen.h"
#include "moves.h"
#include "pieceMoves.h"
#include "perft.h"

const std::map<char, int> PIECE_TABLE = {{'p', 0},
                                    {'n', 1},
                                    {'b', 2},
                                    {'r', 3},
                                    {'q', 4},
                                    {'k', 5}};
const int PIECE_VALUES[] = {100, 305, 333, 563, 950, 100};

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
            moves::Move move = {center - 1, board::enPassant, board::board[center], 0, 0, 0, 1};
            makeMove(move);
            board::turn = !board::turn;
            if (checks::enPassantLegal())
                moves.push_back(move);
            board::turn = !board::turn;
            unmakeMove(move);
        }
        if (board::board[center + 1] == pawn) {
            moves::Move move = {center + 1, board::enPassant, board::board[center], 0, 0, 0, 1};
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
                std::vector<moves::Move> rook = pieceMoves::rmoves(i, pinned, false);
                moves.insert(moves.end(), rook.begin(), rook.end());
                std::vector<moves::Move> bishop = pieceMoves::bmoves(i, pinned);
                moves.insert(moves.end(), bishop.begin(), bishop.end());
                break;
            }

            case 'r': {
                std::vector<moves::Move> temp = pieceMoves::rmoves(i, pinned, true);
                moves.insert(moves.end(), temp.begin(), temp.end());
                break;
            }

            case 'b': {
                std::vector<moves::Move> temp = pieceMoves::bmoves(i, pinned);
                moves.insert(moves.end(), temp.begin(), temp.end());
                break;
            }

            case 'n': {
                std::vector<moves::Move> temp = pieceMoves::nmoves(i, pinned);
                moves.insert(moves.end(), temp.begin(), temp.end());
                break;
            }

            case 'p': {
                std::vector<moves::Move> temp = pieceMoves::pmoves(i, pinned);
                moves.insert(moves.end(), temp.begin(), temp.end());
                break;
            }

            case 'k': {
                std::vector<moves::Move> temp = pieceMoves::kmoves(i, attacked);
                moves.insert(moves.end(), temp.begin(), temp.end());
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

    if (!timesChecked)
        return moves;

    std::vector<int> blocks = checks::getBlocks(attacked);
    std::vector<moves::Move> result;

    if (timesChecked == 2)
        blocks.clear();

    for (moves::Move m : moves) {
        if (board::board[m.source] == king) {
            result.push_back(m);
            continue;
        }
        if (count(blocks.begin(), blocks.end(), m.dest))
            result.push_back(m);
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