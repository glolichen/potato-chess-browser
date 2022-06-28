#ifndef HASHING_H
#define HASHING_H

namespace hashing {
    using ull = unsigned long long;
    struct Hashes {
        ull hash1;
        ull hash2;
        ull hash3;
        bool operator==(const Hashes &other) const {
            return hash1 == other.hash1 && hash2 == other.hash2 && hash3 == other.hash3;
        }
    };
    void initZobristTables();
    Hashes getZobristHash();
    int toPiece(char piece);
}

#endif
