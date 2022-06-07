#include <vector>
#include <algorithm>

#include "board.h"
#include "knight.h"
#include "constants.h"
#include "moves.h"
#include "moveGen.h"
#include "checks.h"

std::vector<moves::Move> knight::nmoves(int loc, std::vector<checks::Check> pinned) {
    std::vector<moves::Move> moves;

    for (checks::Check c : pinned) {
        if (c.coord == loc)
            return moves;
    }

    for (int offset : constants::KNIGHT) {
        int i = loc + offset;

        if (board::board[i] == -1)
            continue;
        if ((bool) islower(board::board[i]) == board::turn && board::board[i])
            continue;

        moves.push_back(moves::Move{loc, i, board::board[i], 0, 0, 0, 0});
    }

    return moves;
}

std::vector<checks::Check> knight::nchecks(int loc) {
    std::vector<checks::Check> attacked;
    for (int offset : constants::KNIGHT) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked.push_back(checks::Check{i, loc, -10});
    }
    return attacked;
}
