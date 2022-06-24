#ifndef ATTACKED_H
#define ATTACKED_H

#include <vector>
#include "checks.h"

namespace attacked {
    std::vector<checks::Check> getAttacked();
    void pchecks(int loc, std::vector<checks::Check>* attacked);
    void nchecks(int loc, std::vector<checks::Check>* attacked);
    void bchecks(int loc, std::vector<checks::Check>* attacked);
    void rchecks(int loc, std::vector<checks::Check>* attacked);
    void kchecks(int loc, std::vector<checks::Check>* attacked);
}

#endif
