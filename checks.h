#ifndef CHECKS_H
#define CHECKS_H

namespace checks {
    struct Check {
        int coord; // Square of piece getting checked
        int original; // Square of piece checking the king
        int axis; // Axis of the check (offset from the piece to the king)
    };

    bool enPassantLegal();

    void getPinned(std::vector<checks::Check>* pinned);
    void getBlocks(std::vector<int>* blocks, std::vector<checks::Check>* attacked);
}

#endif
