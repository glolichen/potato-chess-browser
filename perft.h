#ifndef PERFT_H
#define PERFT_H

#include <map>
#include <string>
#include <vector>

#include "moves.h"

namespace perft {
    using ul = unsigned long;
    struct Entry {
        moves::Move moves;
        ul possible;
    };
    ul perft(int depth, bool first);
    std::vector<Entry> runPerft(std::string fen, int depth);
}

#endif
