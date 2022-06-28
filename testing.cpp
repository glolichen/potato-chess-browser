#include <iostream>

#include "board.h"
#include "hashing.h"

int main() {
    hashing::initZobristTables();
    board::decode("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    hashing::Hashes hash1 = hashing::getZobristHash();
    std::cout << hash1.hash1 << "\n";
    std::cout << hash1.hash2 << "\n";
    std::cout << hash1.hash3 << "\n";

    hashing::Hashes hash2 = hashing::getZobristHash();
    std::cout << hash2.hash1 << "\n";
    std::cout << hash2.hash2 << "\n";
    std::cout << hash2.hash3 << "\n";

    std::cout << (hash1 == hash2) << "\n";

    return 0;
}