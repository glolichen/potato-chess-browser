#ifndef BISHOP_H
#define BISHOP_H

#include <vector>
#include "checks.h"
#include "moves.h"

namespace bishop {
    std::vector<moves::Move> bmoves(int loc, std::vector<checks::Check> pinned);
    std::vector<checks::Check> bchecks(int loc);
}
#endif
