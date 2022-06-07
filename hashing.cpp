#include <map>
#include <random>
#include <iostream>

#include "board.h"
#include "hashing.h"

unsigned long long zobristTable[8][8][12];

void hashing::initZobristTable() {
    std::random_device rd;
    std::mt19937_64 random(rd());
    std::uniform_int_distribution<unsigned long long> distribution;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 12; k++)
                zobristTable[i][j][k] = distribution(random);
        }
    }
}

unsigned long long hashing::getZobristHash() {
    unsigned long long result = 0;
    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 10; j++) {
            int index = i * 12 + j;
            if (board::board[index])
                result ^= zobristTable[i - 2][j - 2][hashing::toPiece(board::board[index])];
        }
    }
    return result;
}

int hashing::toPiece(char piece) {
    switch (piece) {
        case 'P':
            return 0;
        case 'N':
            return 1;
        case 'B':
            return 2;
        case 'R':
            return 3;
        case 'Q':
            return 4;
        case 'K':
            return 5;
        case 'p':
            return 6;
        case 'n':
            return 7;
        case 'b':
            return 8;
        case 'r':
            return 9;
        case 'q':
            return 10;
        case 'k':
            return 11;
        default:
            return -1;
    }
}
