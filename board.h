#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>

namespace board {
    extern char board[];
    extern int king[];
    extern int enPassant;

    extern std::vector<int> pieces[2];

    extern bool K;
    extern bool Q;
    extern bool k;
    extern bool q;

    extern bool turn;

    extern int halfMoveClock;

    void decode(std::string fen);

    std::string encode();

    void printBoard();

    std::string truncateFEN(std::string fen);

    std::vector<std::string> split(std::string str, char splitOn);

    int SANToNotation(std::string san);

    std::string toXY(int coord);

    int toRegularBoard(int coord);

    int toNotation(int x, int y);
}

#endif
