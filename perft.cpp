#include <chrono>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "board.h"
#include "moveGen.h"
#include "moves.h"
#include "perft.h"

std::vector<perft::Entry> entries;

perft::ul perft::perft(int depth, bool first) {
    if (depth == 1)
        return moveGen::moveGen().size();

    std::vector<moves::Move> moves = moveGen::moveGen();

    ul old = 0;
    ul positions = 0;

    for (moves::Move move : moves) {
        makeMove(&move);
        positions += perft(depth - 1, false);
        unmakeMove(&move);

        if (first) {
            entries.push_back({ move, positions-old });
            old = positions;
        }
    }

    return positions;
}
std::vector<perft::Entry> perft::runPerft(std::string fen, int depth) {
    board::decode(fen);
    entries.clear();
    
    if (depth == 1) {
        for (moves::Move move : moveGen::moveGen())
            entries.push_back({ move, 1 });
        return entries;
    }

    perft::perft(depth, true);
    return entries;
}