#include <vector>
#include <algorithm>
#include <cstdlib>

#include "board.h"
#include "rook.h"
#include "constants.h"
#include "moves.h"
#include "moveGen.h"
#include "checks.h"

std::vector<moves::Move> rook::rmoves(int loc, std::vector<checks::Check> pinned, bool isRook) {
    std::vector<moves::Move> moves;

    for (const int &offset: constants::ROOK) {
        int axis = -1;
        for (checks::Check c : pinned) {
            if (c.coord == loc) {
                axis = c.axis;
                break;
            }
        }

        if (axis != -1 && axis != abs(offset))
            continue;

        int i = loc + offset;
        while (board::board[i] != -1) {
            if (board::board[i] && (bool) islower(board::board[i]) == board::turn)
                break;

            char signal = 0;
            if (isRook) {
                if (board::turn) {
                    if (board::k & (loc % 12 == 9))
                        signal = 'k';
                    if (board::q & (loc % 12 == 2))
                        signal = 'q';
                } else {
                    if (board::K & (loc % 12 == 9))
                        signal = 'K';
                    if (board::Q & (loc % 12 == 2))
                        signal = 'Q';
                }
            }

            moves.push_back(moves::Move{loc, i, board::board[i], 0, 0, signal, 0});

            if (board::board[i])
                break;

            i += offset;
        }
    }

    return moves;
}

std::vector<checks::Check> rook::rchecks(int loc) {
    std::vector<checks::Check> attacked;
    char opponentKing = board::turn ? 'k' : 'K';
    for (const int &offset: constants::ROOK) {
        int i = loc + offset;
        while (board::board[i] != -1) {
            attacked.push_back(checks::Check{i, loc, -offset});

            if (board::board[i] && board::board[i] != opponentKing)
                break;

            i += offset;
        }
    }
    return attacked;
}
