#include <vector>
#include <algorithm>
#include <cstdlib>

#include "board.h"
#include "pawn.h"
#include "constants.h"
#include "moves.h"
#include "moveGen.h"
#include "checks.h"

std::vector<moves::Move> pawn::pmoves(int loc, std::vector<checks::Check> pinned) {
    std::vector<moves::Move> moves;

    int axis = -1;
    for (checks::Check c : pinned) {
        if (c.coord == loc) {
            axis = c.axis;
            break;
        }
    }

    bool starting = false;
    if (board::turn)
        starting = (loc / 12 == 3);
    else
        starting = (loc / 12 == 8);

    int offset = loc + constants::PAWN_MOVE[board::turn][0];
    if (board::board[offset] == 0 && (axis == -1 || axis == abs(constants::PAWN_MOVE[board::turn][0]))) {
        if (board::turn && offset / 12 == 9) {
            moves.push_back(moves::Move{loc, offset, 0, 0, 'q', 0, 0});
            moves.push_back(moves::Move{loc, offset, 0, 0, 'r', 0, 0});
            moves.push_back(moves::Move{loc, offset, 0, 0, 'b', 0, 0});
            moves.push_back(moves::Move{loc, offset, 0, 0, 'n', 0, 0});
        } else if (!board::turn && offset / 12 == 2) {
            moves.push_back(moves::Move{loc, offset, 0, 0, 'Q', 0, 0});
            moves.push_back(moves::Move{loc, offset, 0, 0, 'R', 0, 0});
            moves.push_back(moves::Move{loc, offset, 0, 0, 'B', 0, 0});
            moves.push_back(moves::Move{loc, offset, 0, 0, 'N', 0, 0});
        } else
            moves.push_back(moves::Move{loc, offset, 0, 0, 0, 0, 0});

        if (starting && board::board[loc + constants::PAWN_START[board::turn][0]] == 0)
            moves.push_back(moves::Move{loc, loc + constants::PAWN_START[board::turn][0], 0, 0, 0, 0, 0});
    }

    for (int offset2 : constants::PAWN_CAPTURE[board::turn]) {
        int i = loc + offset2;
        if (!board::board[i] || board::board[i] == -1)
            continue;

        bool turn = board::turn;
        bool pieceIsLower = islower(board::board[i]);

        if (pieceIsLower != turn && (axis == -1 || axis == abs(offset2))) {
            if (board::turn && i / 12 == 9) {
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'q', 0, 0});
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'r', 0, 0});
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'b', 0, 0});
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'n', 0, 0});
            } else if (!board::turn && i / 12 == 2) {
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'Q', 0, 0});
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'R', 0, 0});
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'B', 0, 0});
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 'N', 0, 0});
            } else
                moves.push_back(moves::Move{loc, i, board::board[i], 0, 0, 0, 0});
        }
    }

    return moves;
}

std::vector<checks::Check> pawn::pchecks(int loc) {
    std::vector<checks::Check> attacked;
    for (const int &offset: constants::PAWN_CAPTURE[!board::turn]) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked.push_back(checks::Check{i, loc, -10});
    }
    return attacked;
}
