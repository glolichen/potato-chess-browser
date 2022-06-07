#ifndef KNIGHT_H
#define KNIGHT_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace knight {
    std::vector<moves::Move> nmoves(int loc, std::vector<checks::Check> pinned);
    std::vector<checks::Check> nchecks(int loc);
}

#endif
