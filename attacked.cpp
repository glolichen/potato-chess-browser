#include <vector>

#include "attacked.h"
#include "board.h"
#include "checks.h"
#include "constants.h"

std::vector<checks::Check> attacked::getAttacked() {
    std::vector<checks::Check> attacks;

    for (int index : board::pieces[!board::turn]) {
        char piece = tolower(board::board[index]);

        switch (piece) {
            case 'q': {
                std::vector<checks::Check> rook = attacked::rchecks(index);
                attacks.insert(attacks.end(), rook.begin(), rook.end());
                std::vector<checks::Check> bishop = attacked::bchecks(index);
                attacks.insert(attacks.end(), bishop.begin(), bishop.end());
                break;
            }

            case 'r': {
                std::vector<checks::Check> temp = attacked::rchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'b': {
                std::vector<checks::Check> temp = attacked::bchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'n': {
                std::vector<checks::Check> temp = attacked::nchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'p': {
                std::vector<checks::Check> temp = attacked::pchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }

            case 'k': {
                std::vector<checks::Check> temp = attacked::kchecks(index);
                attacks.insert(attacks.end(), temp.begin(), temp.end());
                break;
            }
        }
    }

    return attacks;
}



std::vector<checks::Check> attacked::pchecks(int loc) {
    std::vector<checks::Check> attacked;
    for (const int &offset: constants::PAWN_CAPTURE[!board::turn]) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked.push_back(checks::Check{i, loc, -10});
    }
    return attacked;
}

std::vector<checks::Check> attacked::nchecks(int loc) {
    std::vector<checks::Check> attacked;
    for (int offset : constants::KNIGHT) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked.push_back(checks::Check{i, loc, -10});
    }
    return attacked;
}

std::vector<checks::Check> attacked::bchecks(int loc) {
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

std::vector<checks::Check> attacked::rchecks(int loc) {
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

std::vector<checks::Check> attacked::kchecks(int loc) {
    std::vector<checks::Check> attacked;
    for (const int offset : constants::KING) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked.push_back(checks::Check{i, loc, -10});
    }
    return attacked;
}