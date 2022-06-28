#ifndef PIECES_H
#define PIECES_H

#include <unordered_map>

namespace pieces {
    extern std::unordered_map<char, int> PIECE_TABLE;

    extern int WHITE;
    extern int BLACK;

    extern int PAWN;
    extern int KNIGHT;
    extern int BISHOP;
    extern int ROOK;
    extern int QUEEN;
    extern int KING;

    int toWhite(int piece);
    int toBlack(int piece);

    bool isWhite(int piece);
    bool isBlack(int piece);
}

#endif
