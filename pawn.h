#ifndef PAWN_H
#define PAWN_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace pawn {
    std::vector<moves::Move> pmoves(int loc, std::vector<checks::Check> pinned);
    std::vector<checks::Check> pchecks(int loc);
}

#endif
