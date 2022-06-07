#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "moves.h"
#include "checks.h"

namespace moveGen {
    std::vector<moves::Move> moveGen();
    std::vector<moves::Move> moveGenWithOrdering();
}

#endif
