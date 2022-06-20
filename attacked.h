#ifndef ATTACKED_H
#define ATTACKED_H

#include <vector>
#include "checks.h"

namespace attacked {
    std::vector<checks::Check> getAttacked();
    std::vector<checks::Check> pchecks(int loc);
    std::vector<checks::Check> nchecks(int loc);
    std::vector<checks::Check> bchecks(int loc);
    std::vector<checks::Check> rchecks(int loc);
    std::vector<checks::Check> kchecks(int loc);
}

#endif
