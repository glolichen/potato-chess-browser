#include <vector>
#include <algorithm>

#include "board.h"
#include "constants.h"
#include "moves.h"
#include "bishop.h"
#include "moveGen.h"
#include "checks.h"

std::vector<moves::Move> bishop::bmoves(int loc, std::vector<checks::Check> pinned) {
    std::vector<moves::Move> moves;

    for (const int &offset: constants::BISHOP) {
        int axis = -1;
        for (const checks::Check &c: pinned) {
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

            moves.push_back(moves::Move{loc, i, board::board[i], 0, 0, 0, 0});

            if (board::board[i])
                break;

            i += offset;
        }
    }

    return moves;
}

std::vector<checks::Check> bishop::bchecks(int loc) {
    std::vector<checks::Check> attacked;
    char opponentKing = board::turn ? 'k' : 'K';
    for (const int &offset: constants::BISHOP) {
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