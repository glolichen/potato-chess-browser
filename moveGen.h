#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace moveGen {
    std::vector<moves::Move> moveGen();
    std::vector<moves::Move> moveGenWithOrdering();
}

#endif
