#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include "moves.h"

namespace search {
    struct SearchResult {
        moves::Move move;
        int depth;
        int eval;
        bool mateFound;
    };

    extern moves::Move topMove;
    extern int topPosition[144];

    int minimax(int depth, int alpha, int beta, int depthFromStart);

    int quiscence(int alpha, int beta);

    SearchResult search(int timeMS);

    bool evalIsMate(int eval);
}

#endif
