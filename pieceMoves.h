#ifndef PIECEMOVES_H
#define PIECEMOVES_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace pieceMoves {
    std::vector<moves::Move> pmoves(int loc, std::vector<checks::Check> pinned);
    std::vector<moves::Move> nmoves(int loc, std::vector<checks::Check> pinned);
    std::vector<moves::Move> bmoves(int loc, std::vector<checks::Check> pinned);
    std::vector<moves::Move> rmoves(int loc, std::vector<checks::Check> pinned, bool isRook);
    std::vector<moves::Move> kmoves(int loc, std::vector<checks::Check> attacked);
}

#endif
