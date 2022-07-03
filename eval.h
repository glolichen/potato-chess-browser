#ifndef EVAL_H
#define EVAL_H

#include <vector>

namespace eval {
    void initPieceTables();

    int evaluate();

    bool insufMat(std::vector<char>* pieces);
    int kingSquareEval(bool endgame);
    int kingLocationEval(int EGWeight);
}

#endif
