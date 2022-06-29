#include <algorithm>
#include <cstdlib>
#include <string>

#include "board.h"
#include "moves.h"

int contains(int side, int square) {
    return find(board::pieces[side].begin(), board::pieces[side].end(), square) != board::pieces[side].end();
}

void del(int side, int square) {
    board::pieces[side].erase(remove(board::pieces[side].begin(), board::pieces[side].end(), square), board::pieces[side].end());
}

void moves::makeMove(moves::Move* move) {
    board::enPassant = -1;
    board::turn = !board::turn;

    if (move->signal == 'X') {
        board::K = false;
        board::Q = false;
    }
    if (move->signal == 'K')
        board::K = false;
    if (move->signal == 'Q')
        board::Q = false;

    if (move->signal == 'x') {
        board::k = false;
        board::q = false;
    }
    if (move->signal == 'k')
        board::k = false;
    if (move->signal == 'q')
        board::q = false;

    if (move->castle) {
        moves::castle(move->castle);
        return;
    }

    if (move->isEp) {
        board::board[move->dest] = board::board[move->source];
        board::board[move->source] = 0;

        int dir = 1;
        if (!board::turn)
            dir = -1;
        dir *= 12;

        board::board[move->dest + dir] = 0;

        del(board::turn, move->dest + dir);
        del(!board::turn, move->source);
        board::pieces[!board::turn].push_back(move->dest);

        return;
    }

    if (move->promote) {
        board::board[move->dest] = move->promote;
        board::board[move->source] = 0;

        del(!board::turn, move->source);
        del(board::turn, move->dest);
        board::pieces[!board::turn].push_back(move->dest);

        return;
    }

    if (board::turn && board::board[move->source] == 'P' && move->source - move->dest == 24) {
        if (board::board[move->dest - 1] == 'p')
            board::enPassant = move->dest + 12;
        if (board::board[move->dest + 1] == 'p')
            board::enPassant = move->dest + 12;
    } else if (!board::turn && board::board[move->source] == 'p' && move->dest - move->source == 24) {
        if (board::board[move->dest - 1] == 'P')
            board::enPassant = move->dest - 12;
        if (board::board[move->dest + 1] == 'P')
            board::enPassant = move->dest - 12;
    }

    if (tolower(board::board[move->source]) == 'k')
        board::king[!board::turn] = move->dest;

    del(!board::turn, move->source);
    del(board::turn, move->dest);
    board::pieces[!board::turn].push_back(move->dest);

    board::board[move->dest] = board::board[move->source];
    board::board[move->source] = 0;
}
void moves::unmakeMove(moves::Move* move) {
    board::enPassant = -1;
    board::turn = !board::turn;

    if (move->signal == 'X') {
        board::K = true;
        board::Q = true;
    }
    if (move->signal == 'K')
        board::K = true;
    if (move->signal == 'Q')
        board::Q = true;

    if (move->signal == 'x') {
        board::k = true;
        board::q = true;
    }
    if (move->signal == 'k')
        board::k = true;
    if (move->signal == 'q')
        board::q = true;

    if (move->castle) {
        moves::uncastle(move->castle);
    }

    if (move->isEp) {
        board::enPassant = move->dest;

        board::board[move->source] = board::board[move->dest];
        board::board[move->dest] = 0;

        int pawn = move->capture;

        int dir = -1;
        if (!board::turn)
            dir = 1;
        dir *= 12;

        board::board[move->dest + dir] = pawn;

        board::pieces[board::turn].push_back(move->source);
        board::pieces[!board::turn].push_back(move->dest + dir);
        del(board::turn, move->dest);

        return;
    }

    if (move->promote) {
        int pawn = board::turn ? 'p' : 'P';
        board::board[move->source] = pawn;
        board::board[move->dest] = move->capture;

        if (move->capture)
            board::pieces[!board::turn].push_back(move->dest);
        del(board::turn, move->dest);
        board::pieces[board::turn].push_back(move->source);

        return;
    }

    if (tolower(board::board[move->dest]) == 'k')
        board::king[board::turn] = move->source;

    del(board::turn, move->dest);
    board::pieces[board::turn].push_back(move->source);
    if (move->capture)
        board::pieces[!board::turn].push_back(move->dest);

    board::board[move->source] = board::board[move->dest];
    board::board[move->dest] = move->capture;
}

void moves::castle(int dir) {
    if (dir == 1) {
        if (board::turn) {
            board::board[114] = 0;
            board::board[117] = 0;
            board::board[116] = 'K';
            board::board[115] = 'R';

            board::king[0] = 116;
            del(0, 114);
            del(0, 117);
            board::pieces[0].push_back(116);
            board::pieces[0].push_back(115);
        } else {
            board::board[30] = 0;
            board::board[33] = 0;
            board::board[32] = 'k';
            board::board[31] = 'r';

            board::king[1] = 32;
            del(1, 30);
            del(1, 33);
            board::pieces[1].push_back(32);
            board::pieces[1].push_back(31);
        }
    } else if (dir == 2) {
        if (board::turn) {
            board::board[114] = 0;
            board::board[110] = 0;
            board::board[112] = 'K';
            board::board[113] = 'R';

            board::king[0] = 112;
            del(0, 114);
            del(0, 110);
            board::pieces[0].push_back(112);
            board::pieces[0].push_back(113);
        } else {
            board::board[30] = 0;
            board::board[26] = 0;
            board::board[28] = 'k';
            board::board[29] = 'r';

            board::king[1] = 28;
            del(1, 30);
            del(1, 26);
            board::pieces[1].push_back(28);
            board::pieces[1].push_back(29);
        }
    }
}
void moves::uncastle(int dir) {
    if (dir == 1) {
        if (!board::turn) {
            board::board[115] = 0;
            board::board[117] = 'R';

            del(0, 115);
            board::pieces[0].push_back(117);
        } else {
            board::board[31] = 0;
            board::board[33] = 'r';

            del(1, 31);
            board::pieces[1].push_back(33);
        }
    } else if (dir == 2) {
        if (!board::turn) {
            board::board[113] = 0;
            board::board[110] = 'R';

            del(0, 113);
            board::pieces[0].push_back(110);
        } else {
            board::board[29] = 0;
            board::board[26] = 'r';

            del(1, 29);
            board::pieces[1].push_back(26);
        }
    }
}

moves::SimpleMove moves::convertUCI(std::string UCIMove) {
    int oy = UCIMove[0];
    int ox = UCIMove[1] - '0';
    int ny = UCIMove[2];
    int nx = UCIMove[3] - '0';

    ox = 8 - ox;
    oy = oy - 'a';
    nx = 8 - nx;
    ny = ny - 'a';

    moves::SimpleMove result = {board::toNotation(ox, oy), board::toNotation(nx, ny)};
    return result;
}