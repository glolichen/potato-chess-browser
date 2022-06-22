#ifndef SEARCH_H
#define SEARCH_H

#include <utility>
#include "moves.h"

namespace search {
    extern moves::Move topMove;
    extern int topPosition[144];

    int minimax(int depth, int alpha, int beta, int depthFromStart);

    int quiscence(int alpha, int beta);

    moves::Move search(int timeMS);

    bool evalIsMate(int eval);
}

#endif
