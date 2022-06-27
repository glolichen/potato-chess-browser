#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "board.h"
#include "checks.h"
#include "constants.h"
#include "moveGen.h"

bool checks::enPassantLegal() {
    int kingPos = board::king[board::turn];
    for (int offset : constants::ROOK) {
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
    for (int offset : constants::BISHOP) {
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

void checks::getPinned(std::vector<checks::Check>* pinned) {
    int kingPos = board::king[board::turn];

    for (int piece : board::pieces[!board::turn]) {
        char pieceType = std::tolower(board::board[piece]);

        if (pieceType != 'r' && pieceType != 'b' && pieceType != 'q')
            continue;

        int offset = 0;
        if (pieceType == 'r' || pieceType == 'q') {
            if (piece / 12 == kingPos / 12)
                offset = (kingPos - piece) / std::abs(kingPos - piece);
            else if (piece % 12 == kingPos % 12)
                offset = (kingPos - piece) / std::abs(kingPos - piece) * 12;
        }
        
        if (pieceType == 'b' || pieceType == 'q') {
            if ((kingPos - piece) % 11 == 0)
                offset = (kingPos - piece) / std::abs(kingPos - piece) * 11;
            else if ((kingPos - piece) % 13 == 0)
                offset = (kingPos - piece) / std::abs(kingPos - piece) * 13;
        }

        if (offset == 0)
            continue;
        
        bool isPinned = false;
        bool used = false;

        checks::Check pin = { 0, 0, 0 };

        for (int newSquare = piece + offset; board::board[newSquare] != -1; newSquare += offset) {
            if (board::board[newSquare] == 0 || board::board[newSquare] == -1)
                continue;

            if ((bool) islower(board::board[newSquare]) == board::turn) {
                if (tolower(board::board[newSquare]) == 'k') {
                    if (used)
                        isPinned = true;
                    break;
                }

                if (used)
                    break;
                used = true;
                pin = { newSquare, -1, std::abs(offset) };
            }
            else
                break;
        }
        if (isPinned)
            pinned->push_back(pin);
    }
}

void checks::getBlocks(std::vector<int>* blocks, std::vector<checks::Check>* attacked) {
    for (checks::Check check : *attacked) {
        if (check.coord != board::king[board::turn])
            continue;

        if (check.axis == -10) {
            blocks->push_back(check.original);
            break;
        }

        int i = check.coord + check.axis;
        while (!board::board[i]) {
            blocks->push_back(i);
            i += check.axis;
        }
        blocks->push_back(i);
    }
}
