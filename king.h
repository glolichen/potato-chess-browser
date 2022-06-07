#ifndef KING_H
#define KING_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace king {
    std::vector<moves::Move> kmoves(int loc, std::vector<checks::Check> attacked);
    std::vector<checks::Check> kchecks(int loc);
}

#endif
