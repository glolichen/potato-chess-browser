#ifndef ATTACKED_H
#define ATTACKED_H

#include <vector>
#include <unordered_set>
#include "checks.h"

namespace attacked {
    void getAttacked(std::vector<checks::Check>* attacked, std::unordered_set<int>* attackedSet);
    std::vector<checks::Check> getAttackedForJS();
    void pchecks(int loc, std::vector<checks::Check>* attacked, std::unordered_set<int>* attackedSet);
    void nchecks(int loc, std::vector<checks::Check>* attacked, std::unordered_set<int>* attackedSet);
    void bchecks(int loc, std::vector<checks::Check>* attacked, std::unordered_set<int>* attackedSet);
    void rchecks(int loc, std::vector<checks::Check>* attacked, std::unordered_set<int>* attackedSet);
    void kchecks(int loc, std::vector<checks::Check>* attacked, std::unordered_set<int>* attackedSet);
}

#endif
