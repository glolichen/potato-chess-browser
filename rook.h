#ifndef ROOK_H
#define ROOK_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace rook {
    std::vector<moves::Move> rmoves(int loc, std::vector<checks::Check> pinned, bool isRook);
    std::vector<checks::Check> rchecks(int loc);
}

#endif
