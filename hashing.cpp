#include <iostream>
#include <map>
#include <random>

#include "board.h"
#include "hashing.h"

hashing::ull zobristTables[3][8][8][12];

void hashing::initZobristTables() {
    for (int i = 0; i < 3; i++) {
        std::random_device rd;
        std::mt19937_64 random(rd());
        std::uniform_int_distribution<hashing::ull> distribution;

        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                for (int m = 0; m < 12; m++)
                    zobristTables[i][j][k][m] = distribution(random);
            }
        }
    }
}

std::tuple<hashing::ull, hashing::ull, hashing::ull> hashing::getZobristHash() {
    ull hash1 = 0;
    ull hash2 = 0;
    ull hash3 = 0;
    ull* hashLocations[3] = { &hash1, &hash2, &hash3 };

    for (int i = 0; i < 3; i++) {
        for (int j = 2; j < 10; j++) {
            for (int k = 2; k < 10; k++) {
                int index = j * 12 + k;
                if (board::board[index])
                    *hashLocations[i] ^= zobristTables[i][j - 2][k - 2][hashing::toPiece(board::board[index])];
            }
        }
    }
    
    return { hash1, hash2, hash3 };
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
