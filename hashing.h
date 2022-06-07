#ifndef HASHING_H
#define HASHING_H

namespace hashing {
    void initZobristTable();
    unsigned long long getZobristHash();
    int toPiece(char piece);
}

#endif
