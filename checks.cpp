#include <vector>
#include <iostream>
#include <cstdlib>

#include "checks.h"
#include "board.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
#include "king.h"
#include "moveGen.h"
#include "constants.h"

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

std::vector<checks::Check> checks::getAttacked() {
    std::vector<checks::Check> attacks;

    for (int index : board::pieces[!board::turn]) {
        char piece = tolower(board::board[index]);

        switch (piece) {
            case 'q': {
                std::vector<checks::Check> rook = rook::rchecks(index);
                attacks.insert(attacks.end(), rook.begin(), rook.end());
                std::vector<checks::Check> bishop = bishop::bchecks(index);
                attacks.insert(attacks.end(), bishop.begin(), bishop.end());
                break;
            }

            case 'r': {
                std::vector<checks::Check> temp = rook::rchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'b': {
                std::vector<checks::Check> temp = bishop::bchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'n': {
                std::vector<checks::Check> temp = knight::nchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'p': {
                std::vector<checks::Check> temp = pawn::pchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'k': {
                std::vector<checks::Check> temp = king::kchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }
        }
    }
    return attacks;
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
