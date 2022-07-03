#include <vector>

#include "attacked.h"
#include "board.h"
#include "checks.h"
#include "constants.h"

void attacked::getAttacked(std::vector<checks::Check>* attacked) {
    for (int index : board::pieces[!board::turn]) {
        char piece = tolower(board::board[index]);

        switch (piece) {
            case 'q': {
                attacked::rchecks(index, attacked);
                attacked::bchecks(index, attacked);
                break;
            }

            case 'r': {
                attacked::rchecks(index, attacked);
                break;
            }

            case 'b': {
                attacked::bchecks(index, attacked);
                break;
            }

            case 'n': {
                attacked::nchecks(index, attacked);
                break;
            }

            case 'p': {
                attacked::pchecks(index, attacked);
                break;
            }

            case 'k': {
                attacked::kchecks(index, attacked);
                break;
            }
        }
    }
}
std::vector<checks::Check> attacked::getAttackedForJS() {
    std::vector<checks::Check> attacked;
    attacked::getAttacked(&attacked);
    return attacked;
}

void attacked::pchecks(int loc, std::vector<checks::Check>* attacked) {
    for (int offset : constants::PAWN_CAPTURE[!board::turn]) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked->push_back({ i, loc, -10 });
    }
}
void attacked::nchecks(int loc, std::vector<checks::Check>* attacked) {
    for (int offset : constants::KNIGHT) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked->push_back({ i, loc, -10 });
    }
}
void attacked::bchecks(int loc, std::vector<checks::Check>* attacked) {
    char opponentKing = board::turn ? 'k' : 'K';
    for (int offset : constants::BISHOP) {
        int i = loc + offset;
        while (board::board[i] != -1) {
            attacked->push_back({ i, loc, -offset });

            if (board::board[i] && board::board[i] != opponentKing)
                break;

            i += offset;
        }
    }
}
void attacked::rchecks(int loc, std::vector<checks::Check>* attacked) {
    char opponentKing = board::turn ? 'k' : 'K';
    for (int offset : constants::ROOK) {
        int i = loc + offset;
        while (board::board[i] != -1) {
            attacked->push_back({ i, loc, -offset });

            if (board::board[i] && board::board[i] != opponentKing)
                break;

            i += offset;
        }
    }
}
void attacked::kchecks(int loc, std::vector<checks::Check>* attacked) {
    for (int offset : constants::KING) {
        int i = loc + offset;
        if (board::board[i] == -1)
            continue;

        attacked->push_back({ i, loc, -10 });
    }
}