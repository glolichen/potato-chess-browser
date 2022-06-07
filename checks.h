#ifndef CHECKS_H
#define CHECKS_H

namespace checks {
    struct Check {
        int coord; //Square of piece getting checked
        int original; //Square of piece checking the king
        int axis; //Axis of the check (offset from the piece to the king)
    };

    bool enPassantLegal();

    std::vector<checks::Check> getAttacked();

    std::vector<checks::Check> getPinned();

    std::vector<int> getBlocks(std::vector<checks::Check> attacked);
}

#endif
