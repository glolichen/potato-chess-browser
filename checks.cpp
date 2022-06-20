#include <vector>
#include <iostream>
#include <cstdlib>

#include "board.h"
#include "checks.h"
#include "constants.h"
#include "moveGen.h"

bool checks::enPassantLegal() {
    int kingPos = board::king[board::turn];
    for (const int &offset: constants::ROOK) {
        int i = kingPos;
        while (board::board[i] != -1) {
            i += offset;

            if (board::board[i] == 0 || board::board[i] == -1)
                continue;

            if ((bool) islower(board::board[i]) != board::turn && (tolower(board::board[i]) == 'r' || tolower(board::board[i]) == 'q'))
                return false;
            else
                break;
        }
    }

    for (const int &offset: constants::BISHOP) {
        int i = kingPos;
        while (board::board[i] != -1) {
            i += offset;

            if (board::board[i] == 0 || board::board[i] == -1)
                continue;

            if ((bool) islower(board::board[i]) != board::turn && (tolower(board::board[i]) == 'b' || tolower(board::board[i]) == 'q'))
                return false;
            else
                break;
        }
    }

    return true;
}

std::vector<checks::Check> checks::getPinned() {
    std::vector<checks::Check> pinned;
    int kingPos = board::king[board::turn];
    for (const int &offset: constants::ROOK) {
        int i = kingPos;

        bool isPinned = false;
        bool used = false;

        checks::Check pin = {0, 0, 0};

        while (board::board[i] != -1) {
            i += offset;

            if (!board::board[i] || board::board[i] == -1)
                continue;

            if ((bool) islower(board::board[i]) == board::turn) {
                if (used)
                    break;
                used = true;
                pin = checks::Check{i, -1, abs(offset)};
            } else {
                if (tolower(board::board[i]) == 'r' || tolower(board::board[i]) == 'q')
                    isPinned = true;
                break;
            }
        }
        if (isPinned && pin.original != 0)
            pinned.push_back(pin);
    }

    for (const int &offset: constants::BISHOP) {
        int i = kingPos;

        bool isPinned = false;
        bool used = false;

        checks::Check pin = {0, 0, 0};

        while (board::board[i] != -1) {
            i += offset;

            if (!board::board[i] || board::board[i] == -1)
                continue;

            if ((bool) islower(board::board[i]) == board::turn) {
                if (used)
                    break;
                used = true;
                pin = checks::Check{i, -1, abs(offset)};
            } else {
                if (tolower(board::board[i]) == 'b' || tolower(board::board[i]) == 'q')
                    isPinned = true;
                break;
            }
        }
        if (isPinned && pin.original != 0)
            pinned.push_back(pin);
    }

    return pinned;
}

std::vector<int> checks::getBlocks(std::vector<checks::Check> attacked) {
    std::vector<int> blocks;
    for (const checks::Check &c: attacked) {
        if (c.coord != board::king[board::turn])
            continue;

        if (c.axis == -10) {
            blocks.push_back(c.original);
            break;
        }

        int i = c.coord + c.axis;
        while (!board::board[i]) {
            blocks.push_back(i);
            i += c.axis;
        }
        blocks.push_back(i);
    }
    return blocks;
}
