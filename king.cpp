#include <vector>
#include <algorithm>
#include <iostream>

#include "board.h"
#include "king.h"
#include "constants.h"
#include "moves.h"
#include "moveGen.h"
#include "checks.h"

std::vector<moves::Move> king::kmoves(int loc, std::vector<checks::Check> attacked) {
    std::vector<moves::Move> moves;

    for (const int &offset: constants::KING) {
        int i = loc + offset;

        if (board::board[i] == -1)
            continue;
        if ((bool) islower(board::board[i]) == board::turn && board::board[i])
            continue;

        bool contains = false;
        for (const checks::Check &c: attacked) {
            if (c.coord == i) {
                contains = true;
                break;
            }
        }
        if (contains)
            continue;

        char signal = 0;
        if (board::turn) {
            if (board::k & board::q)
                signal = 'x';
            else if (board::k)
                signal = 'k';
            else if (board::q)
                signal = 'q';
        } else {
            if (board::K & board::Q)
                signal = 'X';
            else if (board::K)
                signal = 'K';
            else if (board::Q)
                signal = 'Q';
        }

        moves.push_back(moves::Move{loc, i, board::board[i], 0, 0, signal, 0});
    }

    for (const checks::Check &c: attacked) {
        if (c.coord == board::king[board::turn])
            return moves;
    }

    if (!board::turn) {
        bool KLegal = true;
        bool QLegal = true;
        for (const checks::Check &c: attacked) {
            if (c.coord == 115 || c.coord == 116)
                KLegal = false;
            if (c.coord == 113 || c.coord == 112)
                QLegal = false;
        }

        if (board::K && KLegal && board::board[114] == 'K' && board::board[117] == 'R' && !board::board[115] && !board::board[116]) {
            char signal = 0;
            if (board::K & board::Q)
                signal = 'X';
            else if (board::K)
                signal = 'K';
            else if (board::Q)
                signal = 'Q';
            moves.push_back(moves::Move{114, 116, 0, 1, 0, signal, 0});
        }
        if (board::Q && QLegal && board::board[114] == 'K' && board::board[110] == 'R' && !board::board[113] && !board::board[112] && !board::board[111]) {
            char signal = 0;
            if (board::K & board::Q)
                signal = 'X';
            else if (board::K)
                signal = 'K';
            else if (board::Q)
                signal = 'Q';
            moves.push_back(moves::Move{114, 112, 0, 2, 0, signal, 0});
        }
    } else {
        bool KLegal = true;
        bool QLegal = true;
        for (const checks::Check &c: attacked) {
            if (c.coord == 31 || c.coord == 32)
                KLegal = false;
            if (c.coord == 29 || c.coord == 28)
                QLegal = false;
        }

        if (board::k && KLegal && board::board[30] == 'k' && board::board[33] == 'r' && !board::board[31] && !board::board[32]) {
            char signal = 0;
            if (board::k & board::q)
                signal = 'x';
            else if (board::k)
                signal = 'k';
            else if (board::q)
                signal = 'q';
            moves.push_back(moves::Move{30, 32, 0, 1, 0, signal, 0});
        }
        if (board::q && QLegal && board::board[30] == 'k' && board::board[26] == 'r' && !board::board[29] && !board::board[28] && !board::board[27]) {
            char signal = 0;
            if (board::k & board::q)
                signal = 'x';
            else if (board::k)
                signal = 'k';
            else if (board::q)
                signal = 'q';
            moves.push_back(moves::Move{30, 28, 0, 2, 0, signal, 0});
        }
    }

    return moves;
}

std::vector<checks::Check> king::kchecks(int loc) {
    std::vector<checks::Check> attacked;
    for (const int offset : constants::KING) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked.push_back(checks::Check{i, loc, -10});
    }
    return attacked;
}