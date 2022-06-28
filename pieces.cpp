#include <string>

#include "pieces.h"

// pieces are represented as integers
// the last 3 digits in binary will determine which piece they are
// the 4 digit from the last will determine what color it is

// eg: 1101 is a black piece because the fourth from the right is 1 which stands for black
// and it is a queen because its last three digits are 101 which means queen

std::unordered_map<char, int> pieces::PIECE_TABLE = { { 'p', 0 },
                                                      { 'n', 1 },
                                                      { 'b', 2 },
                                                      { 'r', 3 },
                                                      { 'q', 4 },
                                                      { 'k', 5 } };

int pieces::WHITE = std::stoi("0111", nullptr, 2);
int pieces::BLACK = std::stoi("1000", nullptr, 2);

int pieces::PAWN = std::stoi("001", nullptr, 2);
int pieces::KNIGHT = std::stoi("010", nullptr, 2);
int pieces::BISHOP = std::stoi("011", nullptr, 2);
int pieces::ROOK = std::stoi("100", nullptr, 2);
int pieces::QUEEN = std::stoi("101", nullptr, 2);
int pieces::KING = std::stoi("110", nullptr, 2);

int pieces::toWhite(int piece) {
    return pieces::WHITE & piece;
    /*  1101
    AND 0111
        0101 (answer) */
}
int pieces::toBlack(int piece) {
    return pieces::BLACK | piece;
    /*  0011
     OR 1000
        1011 (answer) */
}

bool pieces::isWhite(int piece) {
    return !(piece >> 3);
    /*  0011 >> 3
      = 0
      NOT 0 = 1 = true */
}
bool pieces::isBlack(int piece) {
    return piece >> 3;
    /*  1001 >> 3
      = 1 = true */
}