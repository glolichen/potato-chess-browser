#ifndef EVAL_H
#define EVAL_H

namespace eval {
    struct Table {
        int table[8][8];
    };

    void initPieceTables();

    int evaluate();

    int kingSquareEval(bool endgame);
    int kingLocationEval(int EGWeight);
}

#endif
