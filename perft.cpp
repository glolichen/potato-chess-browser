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

perft::ui perft::perft(int depth, bool first) {
    if (depth == 1) {
        std::vector<moves::Move> moves;
        moveGen::moveGen(&moves);
        return moves.size();
    }

    std::vector<moves::Move> moves;
    moveGen::moveGen(&moves);

    ui old = 0;
    ui positions = 0;

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
        std::vector<moves::Move> moves;
        moveGen::moveGen(&moves);
        for (moves::Move move : moves)
            entries.push_back({ move, 1 });
        return entries;
    }

    perft::perft(depth, true);
    return entries;
}